/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/tracestats.cc,v 1.14 2002/03/14 22:29:12 tmwong Exp $
  Description:  Generate LRU and frequency trace stats using fscachesim
                objects.
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
#include "StoreInfinite.hh"

// Command usage.

const char *globalProgArgs = "b:f:c:ms:w:";

const char *globalProgUsage = "[-m] " \
"[-b block_size] " \
"[-s cache_size] " \
"[-f file_prefix] " \
"[-c warmup_count] "\
"[-w warmup_time] " \
"trace_file";

// Default command-line argument values.

const int globalBlockSize = 4096;
const uint64_t globalCacheSizeMB = 0;

const int globalMBToB = 1048576;

const char *globalFreqFileSuffix = "freq";
const char *globalLRUFileSuffix = "lru-cumul";
const char *globalSummaryFileSuffix = "summary";

void
usage(char *inProgName,
      int inExitStatus)
{
  printf("Usage: %s %s\n", basename(inProgName), globalProgUsage);

  exit(inExitStatus);
}

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

int
main(int argc, char *argv[])
{
  char *filePrefix = "results";
  uint64_t blockSize = globalBlockSize;
  uint64_t cacheSizeMB = globalCacheSizeMB;
  uint64_t warmupCount = 0;
  double warmupTime = 0;

  bool useMamboFlag = false;

  // Process command-line args.

  bool errFlag = false;
  char opt;
  while (!errFlag && (opt = getopt(argc, argv, globalProgArgs)) != EOF) {
    switch (opt) {
    case 'b':
      blockSize = atol(optarg);
      break;
    case 'c':
      warmupCount = strtoul(optarg, NULL, 0);
      break;
    case 'f':
      filePrefix = optarg;
      break;
    case 'm':
      useMamboFlag = true;
      break;
    case 's':
      cacheSizeMB = strtoul(optarg, NULL, 0);
      break;
    case 'w':
      warmupTime = strtod(optarg, NULL);
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

  // Create a single (possibly infinite) cache for all I/Os to feed into.

  uint64_t cacheSize = cacheSizeMB * (globalMBToB / blockSize);
  StoreInfinite cache("cache", blockSize, cacheSize, false);

  generators->StatisticsAdd(&cache);

  for (int i = optind; i < argc; i++) {
    // Create I/O generator based on the input trace type.

    IORequestGeneratorFile *generator;
    if (useMamboFlag) {
      generator = new IORequestGeneratorFileMambo(&cache, argv[i]);
    }
    else {
      generator = new IORequestGeneratorFileGeneric(&cache, argv[i]);
    }
    generators->IORequestGeneratorAdd(generator);
  }

  // Run until we have no more I/Os to process.

  while (generators->IORequestDown());

  // Output the stats to separate files.

  {
    char buffer[1024];

    sprintf(buffer, "%s.%s", filePrefix, globalFreqFileSuffix);
    if (!stdoutRedirect(buffer)) {
      exit(EXIT_FAILURE);
    }
    cache.statisticsFreqShow();

    sprintf(buffer, "%s.%s", filePrefix, globalLRUFileSuffix);
    if (!stdoutRedirect(buffer)) {
      exit(EXIT_FAILURE);
    }
    cache.statisticsLRUCumulShow();

    sprintf(buffer, "%s.%s", filePrefix, globalSummaryFileSuffix);
    if (!stdoutRedirect(buffer)) {
      exit(EXIT_FAILURE);
    }
    cache.statisticsSummaryShow();
  }

  // Clean up after ourselves.

  delete generators;

  return (EXIT_SUCCESS);
}
