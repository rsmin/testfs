/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Statistics.hh,v 1.3 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STATISTICS_HH_
#define _STATISTICS_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "string.h"

/**
 * Interface for objects that keep block access statistics. Classes that
 * inherit from Statistics must implement all methods.
 */
class Statistics {
private:
  const char *name;

private:
  // Copy constructors - declared private and never defined

  Statistics(const Statistics&);
  Statistics& operator=(const Statistics&);

public:
  /**
   * Create an object that keeps block access statistics.
   */
  Statistics(const char *inName) {
    name = strdup(inName);
  };

  /**
   * Destroy the object.
   */
  virtual ~Statistics() {
    delete name;
  };

  /**
   * Get the name of the object.
   *
   * @return The name.
   */
  const char *nameGet() const {
    return (name);
  };

  /**
   * Reset block access statistics.
   */
  virtual void statisticsReset() = 0;

  /**
   * Show block access statistics.
   */
  virtual void statisticsShow() const = 0;
  virtual void beforeShow() {;} ;
};

#endif /* _STATISTICS_HH_ */
