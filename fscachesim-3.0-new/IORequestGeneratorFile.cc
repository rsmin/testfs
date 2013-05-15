/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorFile.cc,v 1.4 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <stdio.h>
#include <stdlib.h>

#include "Store.hh"

#include "IORequestGeneratorFile.hh"

// tmwong 27 SEP 2000: I will replace abort() calls with exceptions "soon".

IORequestGeneratorFile::IORequestGeneratorFile(Store *inStore,
					       const char *inFilename) :
  IORequestGenerator(inStore)
{
  filename = strdup(inFilename);

  // tmwong 19 JUL 2001: Special case "-" means read records from stdin.

  if (strcmp(filename, "-") == 0) {
    file = stdin;
  }
  else if ((file = fopen(filename, "r")) == NULL) {
    perror(filename);
    abort();
  }
}

IORequestGeneratorFile::~IORequestGeneratorFile()
{
  if (file != NULL && fclose(file) != 0) {
    perror(NULL);
    abort();
  }
  delete filename;
}

bool
IORequestGeneratorFile::IORequestDown()
{
  bool retval = false;

  if (nextRequest) {
    retval = store->IORequestDown(*nextRequest);
    IORequestQueue();
  }

  return (retval);
}
