/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/fscachesim.cc,v 1.9 2002/03/14 22:29:12 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <functional>  
#ifdef HAVE_STDINT_H 
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h> 

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include <string.h> 
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include "IORequest.hh"
#include "IORequestGeneratorBatch.hh"
#include "IORequestGeneratorFile.hh"
#include "IORequestGeneratorFileGeneric.hh"
#include "IORequestGeneratorFileMambo.hh"
#include "Store.hh"
#include "StoreCache.hh"
#include "StoreCacheSeg.hh"
#include "StoreCacheSimple.hh"
#include "StoreCache2Q.hh"
#include "StoreCacheLRU2Address.hh"

#include "StoreRegCache.hh"

// Command usage.

const char *globalProgArgs = "b:c:admo:w:";

const char *globalProgUsage = \
"[-b block_size] " \
"[-c warmup_count] " \
"[-w warmup_time] " \
"[-a (trace analysis)] " \
"reg_cache_type buffer_cache_size reg_cache_size trace_files...";

const int globalMBToB = 1048576;

const int globalStoreCacheSegSegCount = 10;

const int globalStoreCacheSegSegMultiplier = 2;

bool
stdoutRedirect(const char *inFilename)
{
  int newStdoutFD;

  if ((newStdoutFD = open(inFilename, O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0) {
    perror(inFilename);
 
    return (false);
  }
  if (dup2(newStdoutFD, fileno(stdout)) < 0) {
    perror(inFilename);
 
    return (false);
  }
  if (close(newStdoutFD) < 0) {
    perror(inFilename);
 
    return (false);
  }
 
  return (true);
}

void
usage(char *inProgName,
      int inExitStatus)
{
  printf("Usage: %s %s\n", basename(inProgName), globalProgUsage);
  exit(inExitStatus);
}

int
main(int argc,
     char *argv[])
{
  // Default block size.

  uint64_t blockSize = 4096;

  // Default warmups.

  uint64_t warmupCount = 0;
  double warmupTime = 0;

  // Default flags.


  Store::Mode_t runMode = Store::Simulate;

  // Process command-line args.

  bool errFlag = false;
  char opt;
  while (!errFlag && (opt = getopt(argc, argv, globalProgArgs)) != EOF) {
    switch (opt) {
    case 'b':
      blockSize = atol(optarg);
      break;
    case 'c':
      warmupCount = atol(optarg);
      break;
    case 'w':
      warmupTime = strtod(optarg, NULL);
      break;
    case 'a':
      runMode = Store::Analyze;
      break;
    default:
      errFlag = true;
    }
  }

  if (errFlag || optind >= argc) {
    usage(argv[0], EXIT_FAILURE);
  }

  // Set the warmup style for this experiment.

  IORequestGeneratorBatch *generators;
  if (warmupCount > 0) {
    generators = new IORequestGeneratorBatch(warmupCount);
  }
  else if (warmupTime > 0) {
    generators = new IORequestGeneratorBatch(warmupTime);
  }
  else {
    generators = new IORequestGeneratorBatch();
  }

  // Get the cache sizes.

  if (optind + 1 >= argc || atol(argv[optind + 1]) < 0) {
    usage(argv[0], EXIT_FAILURE);
  }

  uint64_t buffercacheSizeMB = atol(argv[optind + 1]);
  uint64_t buffercacheSize = buffercacheSizeMB * (globalMBToB / blockSize);


  if (optind + 2 >= argc ) {
    usage(argv[0], EXIT_FAILURE);
  }

  uint64_t regcacheSizeMB = atol(argv[optind + 2]);
  uint64_t regcacheSize = regcacheSizeMB * (globalMBToB / blockSize);


  // Create an reg cache to receive all I/O requests that miss in the
  // buffer cache.

  const char *regCacheType = argv[optind];
  Store *regCache = NULL;

  if (regcacheSize == 0)
    goto buffer;

  if (!strcmp("LRU", regCacheType)) {
    regCache = new StoreCacheSimple("Reg",
				 blockSize,
				 regcacheSize,
				 StoreCacheSimple::LRU,
			         StoreCacheSimple::None);
    fprintf(stderr,
	    "LRU: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    regcacheSizeMB);
  }

  else if (!strcmp("2Q", regCacheType)) {
    regCache = new StoreCache2Q("Reg",
				 blockSize,
				 regcacheSize);
    fprintf(stderr,
	    "2Q: "
	    "2Q array, size %llu MB eject policy 2Q\n",
	    regcacheSizeMB);
  }

  else {
    fprintf(stderr,
	    "%s: Unrecognized array cache type\n",
	    basename (argv[0]));
    usage(argv[0], EXIT_FAILURE);
  }


  regCache->changemode(runMode);
  generators->StatisticsAdd(regCache);

  // Create a buffer cache for each I/O request stream.

buffer:

  Store *bufferCache;

  if (regCache)
  {
	  bufferCache = new StoreCacheLRU2ADDRESS("Buffer",
                                 regCache,
				 blockSize,
				 buffercacheSize);
  }
  else
  {
	  bufferCache = new StoreCacheLRU2ADDRESS("Buffer",
				 blockSize,
				 buffercacheSize);      
          bufferCache->changemode(runMode);
  }


  generators->StatisticsAdd(bufferCache);

  // Create a client cache for each I/O request stream.

  for (int i = (optind + 3); i < argc; i++) {
    char buffer[40];

    sprintf(buffer, "%s", basename(argv[i]));
    StoreCacheSimple *client =
      new StoreCacheSimple(buffer,
			   bufferCache,
			   blockSize,
			   0,
			   StoreCacheSimple::LRU,
			    StoreCacheSimple::None);

    // Create I/O generator based on the input trace type.

    IORequestGeneratorFile *generator;

    generator = new IORequestGeneratorFileGeneric(client, argv[i]);
    generators->IORequestGeneratorAdd(generator);

  }



  // Run until we have no more I/Os to process.

  while (generators->IORequestDown());

  // Dump out the statistics.
  generators->beforeShow();

  generators->statisticsShow();

  // Clean up after ourselves.

  delete generators;

  return (EXIT_SUCCESS);
}
