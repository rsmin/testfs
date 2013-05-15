/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorFileMambo.hh,v 1.2 2002/02/12 21:50:56 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _IOREQUESTGENERATORFILEMAMBO_HH_
#define _IOREQUESTGENERATORFILEMAMBO_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

extern "C" {
#include "mambolib/iotrace.h"
}

#include "Char.hh"
#include "IORequestGeneratorFile.hh"
#include "Store.hh"

/**
 * Read I/O requests from a MAMBO format file and generate a request
 * stream.
 */
class IORequestGeneratorFileMambo : public IORequestGeneratorFile {
private:
  static Char::Counter staticDir;
  static uint64_t staticDirID;

  traceHeader_t traceHeader;

private:
  IORequestGeneratorFileMambo(const IORequestGeneratorFileMambo&);
  IORequestGeneratorFileMambo& operator=(const IORequestGeneratorFileMambo&);

  void headerProcess();

protected:
  virtual void IORequestQueue();

public:
  /**
   * Create a MAMBO request stream generator.
   */
  IORequestGeneratorFileMambo(Store *inStore,
			      const char *inFilename) :
    IORequestGeneratorFile(inStore, inFilename) {
    headerProcess();
    IORequestQueue();
  };

  /**
   * Destroy the generator.
   */
  virtual ~IORequestGeneratorFileMambo();
};

#endif /* _IOREQUESTGENERATORFILEMAMBO_HH_ */
