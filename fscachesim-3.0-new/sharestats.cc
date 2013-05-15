/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/sharestats.cc,v 1.6 2002/02/15 15:44:25 tmwong Exp $
  Description:  
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
#include "StoreSharestats.hh"

// Command usage.

const char *globalProgArgs = "b:f:m";

const char *globalProgUsage = "[-m] " \
"[-b block_size] " \
"[-f file_prefix] " \
"trace_file";

// Default command-line argument values.

const int globalBlockSize = 4096;

const char *globalShareFileSuffix = "share";

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

  bool useMamboFlag = false;

  // Process command-line args.

  bool errFlag = false;
  char opt;
  while (!errFlag && (opt = getopt(argc, argv, globalProgArgs)) != EOF) {
    switch (opt) {
    case 'b':
      blockSize = atol(optarg);
      break;
    case 'f':
      filePrefix = optarg;
      break;
    case 'm':
      useMamboFlag = true;
      break;
    default:
      errFlag = true;
    }
  }

  if (errFlag || optind >= argc) {
    usage(argv[0], EXIT_FAILURE);
  }

  // Set the warmup style for this experiment.

  IORequestGeneratorBatch *generators = new IORequestGeneratorBatch();

  // Create a single infinite cache for all I/Os to feed into.

  StoreSharestats cache("cache-sharestats", blockSize);
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

    sprintf(buffer, "%s.%s", filePrefix, globalShareFileSuffix);
    if (!stdoutRedirect(buffer)) {
      exit(EXIT_FAILURE);
    }
    cache.statisticsShow();
  }

  // Clean up after ourselves.

  delete generators;

  return (EXIT_SUCCESS);
}
