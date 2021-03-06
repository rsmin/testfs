/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCache2Q.hh,v 1.4 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHE2Q_HH_
#define _STORECACHE2Q_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "Cache.hh"
#include "IORequest.hh"
#include "StoreCache.hh"
#include "2QCache.hh"

/**
 * Simple LRU/MRU block cache with support for demote operations.
 */
class StoreCache2Q : public StoreCache {


private:
  TQCache TQ;



private:
  // Copy constructors - declared private and never defined

  StoreCache2Q(const StoreCache2Q&);
  StoreCache2Q& operator=(const StoreCache2Q&);

protected:
  virtual void BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock,
			  list<IORequest>& outIOReqs);

public:
  // Constructors and destructors

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device, if any.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

   */
  StoreCache2Q(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCache(inName, inNextStore, inBlockSize, inSize),
    TQ(inSize){ ; };

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

   */
  StoreCache2Q(const char *inName,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCache(inName, inBlockSize, inSize),
    TQ(inSize) { ; };

  /**
   * Destroy the cache.
   */
  ~StoreCache2Q() { ; };

  // Statistics management

  virtual void statisticsShow() const;

};

#endif /* _STORECACHE2Q_HH_ */
