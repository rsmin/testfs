/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreSharestats.hh,v 1.2 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORESHARESTATS_HH_
#define _STORESHARESTATS_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <list>
#include <map>

extern "C" {
#include "top-down-size-splay.h"
}

#include "Block.hh"
#include "Store.hh"

class StoreSharestats : public Store {
private:
  struct CharStarLessThan {
    bool operator()(const char *str1, const char *str2) const {
      return (strcmp(str1, str2) < 0);
    };
  };

  typedef map<const char *, Block::Counter *, CharStarLessThan> OrigMap;

  Block::Counter accessMap;

  OrigMap origToAccessMap;

private:
  // Copy constructors - declared private and never defined

  StoreSharestats(const StoreSharestats&);
  StoreSharestats& operator=(const StoreSharestats&);

public:
  StoreSharestats(const char *inName,
		       unsigned int inBlockSize) :
    Store(inName, NULL, inBlockSize),
    accessMap(),
    origToAccessMap() { ; };
  ~StoreSharestats();

  // Process incoming I/O requests

  virtual bool IORequestDown(const IORequest& inIOReq);

  // Statistics management

  virtual void statisticsReset();

  virtual void statisticsShow() const;
};

#endif /* _STORESHARESTATS_HH_ */
