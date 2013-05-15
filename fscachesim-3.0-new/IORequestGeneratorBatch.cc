/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorBatch.cc,v 1.5 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <stdio.h>

#include "IORequestGeneratorBatch.hh"

IORequestGeneratorBatch::~IORequestGeneratorBatch()
{
  if (recordsPerDot > 0) {
    fprintf(stderr, "\nProcessed %llu requests.\n", requestsIssued);
  }
}

bool
IORequestGeneratorBatch::IORequestDown()
{
  // Sort the IORequestGenerators by next-I/O issue time, and issue the
  // earliest request.

  generators.sort(IORequestGeneratorLessThan());
  const IORequest *req = generators.front()->IORequestGet();

  // If the next request passes the threshold for warmups, reset any
  // tracked stats.

  if (!warmupDoneFlag && req != NULL) {
    switch (warmupType) {
    case WarmupCount:
      if (requestsIssued >= warmupCount) {
	statisticsReset();
	warmupDoneFlag = true;
	fprintf(stderr,
		"\nWarmups done after %llu request(s)\n",
		requestsIssued);
      }
      break;
    case WarmupTime:
      if (req->timeIssuedGet() >= warmupTime) {
	statisticsReset();
	warmupDoneFlag = true;
	fprintf(stderr,
		"\nWarmups done at %f second(s) after %llu request(s)\n",
		warmupTime,
		requestsIssued);
      }
      break;
    default:
      // Warmups are always "done" if we never asked for any!

      abort();
    }
  }

  if (recordsPerDot > 0 && (requestsIssued % recordsPerDot == 0)) {
    fprintf(stderr, ".");
    fflush(stderr);
  }

  bool retval;
  if (retval = generators.front()->IORequestDown()) { /*issue the request*/
    requestsIssued++;
  }
  return (retval);
}

void
IORequestGeneratorBatch::statisticsReset()
{
  for (list<Statistics *>::iterator i =  statistics.begin();
       i != statistics.end();
       i++) {
    (*i)->statisticsReset();
  }
}

void
IORequestGeneratorBatch::statisticsShow() const
{
  for (list<Statistics *>::const_iterator i =  statistics.begin();
       i != statistics.end();
       i++) {
    (*i)->statisticsShow();
  }
}

void
IORequestGeneratorBatch::beforeShow()
{
  for (list<Statistics *>::const_iterator i =  statistics.begin();
       i != statistics.end();
       i++) {
    (*i)->beforeShow();
  }
}
