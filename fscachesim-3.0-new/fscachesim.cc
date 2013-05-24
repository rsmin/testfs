/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/fscachesim.cc,v 1.9 2002/03/14 22:29:12 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
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
#include "StoreCacheARC.hh"
#include "StoreCacheMQ.hh"
#include "StoreCacheLRUCoop.hh"
#include "StoreCacheSegCoop.hh"
#include "StoreCache2QCoop.hh"
#include "StoreCache2SCoop.hh"
#include "StoreCache4SCoop.hh" 
#include "StoreCacheLFUCoop.hh"

#include "writeArrayPolicy.hh"


// Command usage.

const char *globalProgArgs = "b:c:admo:w:";

const char *globalProgUsage = \
"[-b block_size] " \
"[-c warmup_count] " \
"[-d] " \
"[-m] " \
"[-o output_file_prefix] " \
"[-w warmup_time] " \
"[-a (trace analysis)] " \
"array_cache_type client_cache_size array_cache_size trace_files...";

const int globalMBToB = 1048576;

const int globalStoreCacheSegSegCount = 10;

const int globalStoreCacheSegSegMultiplier = 2;

const double writeCachetoArrayCacheRatio = 0.1; /*xiaodong this variable declare the write cache size ratio to array cache*/



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

  // Default output file prefix.

  const char *outFilePrefix = NULL;

  // Default warmups.

  uint64_t warmupCount = 0;
  double warmupTime = 0;

  // Default flags.

  bool useDemoteFlag = false;

  bool useMamboFlag = false;

  bool useArraySLRUFlag = false;

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
    case 'd':
      useDemoteFlag = true;
      break;
    case 'o':
      outFilePrefix = optarg;
      break;
    case 'm':
      useMamboFlag = true;
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

  uint64_t clientSizeMB = atol(argv[optind + 1]);
  uint64_t clientSize = clientSizeMB * (globalMBToB / blockSize);

  if (optind + 2 >= argc || atol(argv[optind + 2]) == 0) {
    usage(argv[0], EXIT_FAILURE);
  }

  uint64_t arraySizeMB = atol(argv[optind + 2]);
  uint64_t arraySize = arraySizeMB * (globalMBToB / blockSize);

//build a write array
	uint64_t writeCacheSize = arraySize*writeCachetoArrayCacheRatio; /*define writeCacheSize*/
		arraySize = arraySize-writeCacheSize;
	printf("writeCacheSize: [%llu]\n arraySize: [%llu]\n", writeCacheSize,arraySize);
		writeArrayPolicy *writeArray = new writeArrayPolicy("writeArray",
        		  	  	  	  	  	  	  	  blockSize,
        		  	  	  	  	  	  	  	  writeCacheSize);

writeArray->changemode(runMode);
generators->StatisticsAdd(writeArray);


  // Create an array cache to receive all I/O requests that miss in the
  // clients.

  const char *arrayType = argv[optind];
  Store *array;
  if (!strcmp("LRU", arrayType)) {
    array = new StoreCacheSimple("array",
				 blockSize,
				 arraySize,
				 StoreCacheSimple::LRU,
			         StoreCacheSimple::None);
    fprintf(stderr,
	    "LRU: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    arraySizeMB);
  }
  else if (!strcmp("MRULRU", arrayType)) {
    array = new StoreCacheSimple("array",
				 blockSize,
				 arraySize,
				 StoreCacheSimple::MRU,
				 StoreCacheSimple::None);
    fprintf(stderr,
	    "MRULRU: "
	    "Simple array, size %llu MB eject policy MRULRU\n",
	    arraySizeMB);
  }

  else if (!strcmp("2Q", arrayType)) {
    array = new StoreCache2Q("array",
				 blockSize,
				 arraySize);
    fprintf(stderr,
	    "2Q: "
	    "Simple array, size %llu MB eject policy 2Q\n",
	    arraySizeMB);
  }
  else if (!strcmp("ARC", arrayType)) {
    array = new StoreCacheARC("array",
				 blockSize,
				 arraySize);
    fprintf(stderr,
	    "ARC: "
	    "Simple array, size %llu MB eject policy ARC\n",
	    arraySizeMB);
  }
  else if (!strcmp("MQ", arrayType)) {
    array = new StoreCacheMQ("array",
				 blockSize,
				 arraySize, 5);
    fprintf(stderr,
	    "MQ: "
	    "Simple array, size %llu MB eject policy MQ\n",
	    arraySizeMB);
  }

  else if (!strcmp("NSEGEXP", arrayType)) {
    array = new StoreCacheSeg("array",
			      blockSize,
			      arraySize,
			      globalStoreCacheSegSegCount,
			      globalStoreCacheSegSegMultiplier,
			      true);
    fprintf(stderr,
	    "NSEGEXP: "
	    "Segmented normalized adaptive array, size %llu MB exp segs %d\n",
	    arraySizeMB,
	    globalStoreCacheSegSegCount);
  }
  else if (!strcmp("NSEGUNI", arrayType)) {
    array = new StoreCacheSeg("array",
			      blockSize,
			      arraySize,
			      globalStoreCacheSegSegCount,
			      true);
    fprintf(stderr,
	    "NSEGUNI: "
	    "Segmented normalized adaptive array, size %llu MB uni segs %d \n",
	    arraySizeMB,
	    globalStoreCacheSegSegCount);
  }
  else if (!strcmp("RSEGEXP", arrayType)) {
    array = new StoreCacheSeg("array",
			      blockSize,
			      arraySize,
			      globalStoreCacheSegSegCount,
			      globalStoreCacheSegSegMultiplier,
			      false);
    fprintf(stderr,
	    "RSEGEXP: "
	    "Segmented raw adaptive array, size %llu MB exp segs %d\n",
	    arraySizeMB,
	    globalStoreCacheSegSegCount);
  }
  else if (!strcmp("RSEGUNI", arrayType)) {
    array = new StoreCacheSeg("array",
			      blockSize,
			      arraySize,
			      globalStoreCacheSegSegCount,
			      false);
    fprintf(stderr,
	    "RSEGUNI: "
	    "Segmented raw adaptive array, size %llu MB uni segs %d \n",
	    arraySizeMB,
	    globalStoreCacheSegSegCount);
  }
  else if (!strcmp("LRUClientCoop", arrayType)) {
    array = new StoreCacheLRUCoop("array",
			      blockSize,
			      arraySize,
			      StoreCacheLRUCoop::None);
    fprintf(stderr,
	    "LRUClientCoop: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    arraySizeMB);
  }
  else if (!strcmp("LRUDemoteAnyCoop", arrayType)) {
    array = new StoreCacheLRUCoop("array",
			      blockSize,
			      arraySize,
			      StoreCacheLRUCoop::Demand);
    fprintf(stderr,
	    "LRUDemoteAnyCoop: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    arraySizeMB);
  }
  else if (!strcmp("LRUDemoteWithoutCoop", arrayType)) {
    array = new StoreCacheLRUCoop("array",
			      blockSize,
			      arraySize,
			      StoreCacheLRUCoop::Demand_without_Coop);
    fprintf(stderr,
	    "LRUDemoteWithoutCoop: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    arraySizeMB);
  }
  else if (!strcmp("LRUDemoteAnyCoopNoCacheRead", arrayType)) {
    array = new StoreCacheLRUCoop("array",
			      blockSize,
			      arraySize,
			      StoreCacheLRUCoop::Demand_NoCacheRead);
    fprintf(stderr,
	    "LRUDemoteAnyCoopNoCacheRead: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    arraySizeMB);
  }
  else if (!strcmp("LRUDemoteWithoutCoopNoCacheRead", arrayType)) {
    array = new StoreCacheLRUCoop("array",
			      blockSize,
			      arraySize,
			      StoreCacheLRUCoop::Demand_NoCacheRead_without_Coop);
    fprintf(stderr,
	    "LRUDemoteWithoutCoopNoCacheRead: "
	    "Simple array, size %llu MB eject policy LRU\n",
	    arraySizeMB);
  }
  else if (!strcmp("SEGDemoteCoop", arrayType)) {
    array = new StoreCacheSegCoop("array",
			      blockSize,
			      arraySize,
			      globalStoreCacheSegSegCount,
			      globalStoreCacheSegSegMultiplier);
    fprintf(stderr,
	    "SEGDemoteCoop: "
	    "Segmented normalized adaptive array, size %llu MB exp segs %d\n",
	    arraySizeMB,
	    globalStoreCacheSegSegCount);
  }

  else if (!strcmp("2QDemoteAnyCoop", arrayType)) {
    array = new StoreCache2QCoop("array",
			      blockSize,
			      arraySize,
			      StoreCache2QCoop::Demand);
    fprintf(stderr,
	    "2QDemoteAnyCoop: "
	    "Simple array, size %llu MB eject policy 2Q\n",
	    arraySizeMB);
  }
  else if (!strcmp("2QDemoteWithoutCoop", arrayType)) {
    array = new StoreCache2QCoop("array",
			      blockSize,
			      arraySize,
			      StoreCache2QCoop::Demand_without_Coop);
    fprintf(stderr,
	    "2QDemoteWithoutCoop: "
	    "Simple array, size %llu MB eject policy 2Q\n",
	    arraySizeMB);
  }
  else if (!strcmp("2SDemote", arrayType)) {
    array = new StoreCache2SCoop("array",
			      blockSize,
			      arraySize);
    fprintf(stderr,
	    "2SDemote: "
	    "Simple array, size %llu MB eject policy 2S\n",
	    arraySizeMB);
  }
  else if (!strcmp("4SDemote", arrayType)) {
    array = new StoreCache4SCoop("array",
			      blockSize,
			      arraySize);
    fprintf(stderr,
	    "4SDemote: "
	    "Simple array, size %llu MB eject policy 4S\n",
	    arraySizeMB);
  }
  else if (!strcmp("LFUDemoteCoop", arrayType)) {
    array = new StoreCacheLFUCoop("array",
			      blockSize,
			      arraySize);
    fprintf(stderr,
	    "LFUDemoteCoop: "
	    "Simple array, size %llu MB eject policy LFU\n",
	    arraySizeMB);
  }
  else {
    fprintf(stderr,
	    "%s: Unrecognized array cache type\n",
	    basename (argv[0]));
    usage(argv[0], EXIT_FAILURE);
  }


  array->changemode(runMode);
  generators->StatisticsAdd(array);

  // Create a client cache for each I/O request stream.

  for (int i = (optind + 3); i < argc; i++) {
    char buffer[40];

    sprintf(buffer, "%s", basename(argv[i]));
    StoreCacheSimple *client =
      new StoreCacheSimple(buffer,
			   array,
			   blockSize,
			   clientSize,
			   StoreCacheSimple::LRU,
			   (useDemoteFlag ?
			    StoreCacheSimple::Demand :
			    StoreCacheSimple::None));
    client->changemode(runMode);

    /* Xiaodong Create write cache to process all write requests
       *
       */

      fprintf(stderr,
    	    "writeArray: "
    	    "Simple array, size %llu MB process policy WritDisk\n",
    	    writeCacheSize);
    generators->StatisticsAdd(client);


    // Create I/O generator based on the input trace type.
    //* xiaodong add write I/O generator

    IORequestGeneratorFile *generator;
    IORequestGeneratorFile *wgenerator;
    if (useMamboFlag) {
      generator = new IORequestGeneratorFileMambo(client, argv[i]);
      wgenerator = new IORequestGeneratorFileMambo(writeArray, argv[i]);
    }
    else {
      generator = new IORequestGeneratorFileGeneric(client, argv[i]);
      wgenerator = new IORequestGeneratorFileGeneric(writeArray, argv[i]);

    }

    generators->IORequestGeneratorAdd(generator);
    generators->IORequestGeneratorAdd(wgenerator);

  }

  // Prepare output files.

  if (outFilePrefix) {
    char filename[1024];

    if (warmupCount > 0) {
      sprintf(filename,
	      "%s-%s-%s-%llu-%llu-%lluc",
	      outFilePrefix,
	      (useDemoteFlag ? "DEMOTE" : "NONE"),
	      arrayType,
	      clientSizeMB,
	      arraySizeMB,
	      warmupCount);
    }
    else {
      sprintf(filename,
	      "%s-%s-%s-%llu-%llu-%.0lfs",
	      outFilePrefix,
	      (useDemoteFlag ? "DEMOTE" : "NONE"),
	      arrayType,
	      clientSizeMB,
	      arraySizeMB,
	      warmupTime);
    }
    if (!stdoutRedirect(filename)) {
      exit(EXIT_FAILURE);
    }
  }

  // Run until we have no more I/Os to process.

  while (generators->IORequestDown());

  // Dump out the statistics.

  //clean up the write cache
  writeArray->cacheCleanPolicy();

  generators->beforeShow();
  generators->statisticsShow();


  // Clean up after ourselves.

  delete generators;

  return (EXIT_SUCCESS);
}
