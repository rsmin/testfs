/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorFileMambo.cc,v 1.2 2002/02/15 15:44:25 tmwong Exp $
  Author:       A. Acharya <acha@cs.umd.edu>, T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <assert.h>
#include <stdio.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#define __USE_GNU 1
#include <string.h>

#include "IORequest.hh"
#include "Char.hh"

#include "IORequestGeneratorFileMambo.hh"

#define MAMBO_MAX_LINE_LEN 1024

uint64_t IORequestGeneratorFileMambo::staticDirID = 1;
Char::Counter IORequestGeneratorFileMambo::staticDir;

// tmwong 27 SEP 2000: I will replace abort() calls with exceptions "soon".

void
IORequestGeneratorFileMambo::headerProcess()
{
  char line[MAMBO_MAX_LINE_LEN];
  int len;

  /* parse the header */

  fscanf(file, "%d", &(traceHeader.numOfProcesses));
  assert(traceHeader.numOfProcesses > 0);

  fscanf(file, "%d", &(traceHeader.numOfFiles));
  assert(traceHeader.numOfFiles > 0);

  fscanf(file, "%d\n", &(traceHeader.numOfRecords));
  assert(traceHeader.numOfRecords > 0);

  len = sizeof(string_t)*traceHeader.numOfFiles;
  traceHeader.fileNames = new string_t[len];
  assert(traceHeader.fileNames);

  for (int i = 0; i < traceHeader.numOfFiles; i++) {
    int linelen;

    fscanf(file, "%d %s\n", &linelen, line);
    linelen++;
    assert(linelen > 1);

    /* round up linelen to the nearest  alignment */
    linelen = ((linelen + (SALIGN - 1))/SALIGN) * SALIGN;
    
    traceHeader.fileNames[i].length = linelen;
    traceHeader.fileNames[i].name   = new char[linelen];
    assert(traceHeader.fileNames[i].name);
    memcpy(traceHeader.fileNames[i].name,line,linelen);
    len += linelen;

    // Add files from the data set to the global directory if they are not
    // already listed.

    if (staticDir.find(traceHeader.fileNames[i].name) == staticDir.end()) {
      staticDir[traceHeader.fileNames[i].name] = staticDirID++;
    }
  }

  traceHeader.offsetToTraceRecords = len;
}

void
IORequestGeneratorFileMambo::IORequestQueue()
{
  if (nextRequest) {
    delete nextRequest;
    nextRequest = NULL;
  }

  if (file != NULL) {
    bool gotEOF = false;

    // Keep scanning records until we get a read record.

    do {
      int rc;
      tracerec_t record;

      rc = fscanf(file,
		  "%d %d %d %d %lf %lf %ld %ld",
		  &record.op,
		  &record.numRecords,
		  &record.pid,
		  &record.fileId,
		  &record.wallClock,
		  &record.processClock,
		  &record.offset,
		  &record.length);

      if (rc == EOF) {
	// nextRequest might be non-NULL.

	nextRequest = NULL;
	if (ferror(file)) {
	  abort();
	}

	if (fclose(file) != 0) {
	  abort();
	}
	file = NULL;
	gotEOF = true;
      }
      else if (rc < 8) {
	// Got an incomplete record.

	abort();
      }
      else if (record.op == READ && record.length != 0) {
	// Validate the record.

	if (record.op == LISTIO_HEADER) {
	  assert(record.numRecords >= 1);
	}
	else {
	  assert(record.numRecords == 1);
	}
	assert(record.pid >= 0);
	assert(record.fileId >= 0);
	assert(record.wallClock >= 0.0L);
	assert(record.processClock >= 0.0L);
	assert(record.offset >= 0L);
	assert(record.length >= 0);

	// Ignore files called 'others', since Mambo uses the name as a 
	// throwaway file for miscellaneous accesses.

	if (strcmp(traceHeader.fileNames[record.fileId].name, "others") != 0) {
	  // Convert the fileID to a unique ID.

	  uint64_t uniqueFileID =
	    staticDir[traceHeader.fileNames[record.fileId].name];
	  nextRequest = new IORequest(basename(filename),
				      IORequest::Read,
				      record.wallClock,
				      uniqueFileID,
				      record.offset,
				      record.length);
	}
      }
    } while (!gotEOF && !nextRequest);
  }
}

IORequestGeneratorFileMambo::~IORequestGeneratorFileMambo()
{
  for (int i = 0; i < traceHeader.numOfFiles; i++) {
    delete traceHeader.fileNames[i].name;
  }
  delete traceHeader.fileNames;
}
