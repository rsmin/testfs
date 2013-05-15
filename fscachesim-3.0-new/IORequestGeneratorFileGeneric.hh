/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorFileGeneric.hh,v 1.3 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _IOREQUESTGENERATORFILEGENERIC_HH_
#define _IOREQUESTGENERATORFILEGENERIC_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "IORequestGeneratorFile.hh"
#include "Store.hh"

/**
 * Read I/O requests from a generic format file and generate a request
 * stream. Generic files contain a single request per line of the form:
 *
 * (timeIssued objectID offset-into-obj length-of-req)
 *
 * with units:
 *
 * (min:sec ID bytes bytes)
 */
class IORequestGeneratorFileGeneric : public IORequestGeneratorFile {
private:
  // Copy constructors - declared private and never defined

  IORequestGeneratorFileGeneric(const IORequestGeneratorFileGeneric&);
  IORequestGeneratorFileGeneric& operator=(const IORequestGeneratorFileGeneric&);

protected:
  virtual void IORequestQueue();

public:
  /**
   * Create a generic request stream generator.
   */
  IORequestGeneratorFileGeneric(Store *inStore,
				const char *inFilename) :
    IORequestGeneratorFile(inStore, inFilename) {
    IORequestQueue();
  };

  /**
   * Destroy the generator.
   */
  virtual ~IORequestGeneratorFileGeneric() { ; };
};

#endif /* _IOREQUESTGENERATORFILEGENERIC_HH_ */
