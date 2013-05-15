/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequestGeneratorFile.hh,v 1.3 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _IOREQUESTGENERATORFILE_HH_
#define _IOREQUESTGENERATORFILE_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#include "IORequest.hh"
#include "IORequestGenerator.hh"
#include "Store.hh"

/**
 * Interface for I/O request generators that stream from a file. Classes
 * that inherit from this class must implement a stream-format-specific
 * request generator.
 */
class IORequestGeneratorFile : public IORequestGenerator {
protected:
  /**
   * The file from which to stream requests.
   */
  const char *filename;

  /**
   * The file handle for the file.
   */
  FILE *file;

private:
  IORequestGeneratorFile(const IORequestGeneratorFile&);
  IORequestGeneratorFile& operator=(const IORequestGeneratorFile&);

protected:

  /**
   * Queue a request in preparation for sending to a lower-level storage
   * device.
   */
  virtual void IORequestQueue() = 0;

public:
  /**
   * Create a request generator that streams from inFilename.
   *
   * @param inStore The lower-level storage device to which to stream I/O
   * requests.
   * @param inFilename The file from which to stream requests.
   */
  IORequestGeneratorFile(Store *inStore,
			 const char *inFilename);

  /**
   * Destroy the generator.
   */
  virtual ~IORequestGeneratorFile();

  /**
   * Get the name of the file from which requests are streamed.
   */
  const char *filenameGet() const {
    return (filename);
  };

  /**
   * Send a request to the lower-level storage device.
   */
  virtual bool IORequestDown();
};

#endif /* _IOREQUESTGENERATORFILE_HH_ */
