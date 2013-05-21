/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorFileGeneric.cc,v 1.2 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>
#include <stdlib.h>
#define __USE_GNU 1
#include <string.h>

#include "IORequest.hh"

#include "IORequestGeneratorFileGeneric.hh"

// tmwong 27 SEP 2000: I will replace abort() calls with exceptions "soon".

void
IORequestGeneratorFileGeneric::IORequestQueue()
{
  if (nextRequest) {
    delete nextRequest;
    nextRequest = NULL;
  }

  if (file != NULL) {
    double timeIssued;
    int rc;
    uint64_t objID, off, len;
    char diskop /*xiaodong*/;

    rc = fscanf(file,
		"%lf %llu %llu %llu %c" /*xiaodong*/,
		&timeIssued,
		&objID,
		&off,
		&len,
		&diskop /*xiaodong*/);
    if (rc == EOF) {
      nextRequest = NULL;
      if (ferror(file)) {
    	  printf(" IORequestGeneratorFileGeneric.cc error 1");
	abort();
      }

      if (fclose(file) != 0) {
    	  printf(" IORequestGeneratorFileGeneric.cc error 2");
	abort();
      }
      file = NULL;
    }
    else if(diskop=='W')
    {
    	nextRequest = new IORequest(basename(filename),
    					  IORequest::Write,
    					  timeIssued,
    					  objID,
    					  off,
    					  len);
    } /* add write disk request *xiaodong*/
    else {


      nextRequest = new IORequest(basename(filename),
				  IORequest::Read,
				  timeIssued,
				  objID,
				  off,
				  len);

 /*     printf ("%lf %llu %llu %llu\n",
		timeIssued,
		objID,
		off,
		len);*/


    }
  }
}
