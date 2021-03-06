/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.hh,v 1.4 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHESIMPLECOOP_HH_
#define _STORECACHESIMPLECOOP_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "LocalBlock.hh"
#include "Cache.hh"
#include "BlockBuffer.hh"
#include "IORequest.hh"
#include "StoreCache.hh"

/**
 * Simple LRU/MRU block cache with support for demote operations.
 */

class CoopCache   {

private:

list<Store *> upCache;

public:
  CoopCache() { ; };
  ~CoopCache() { ; };
 
  void addStore(Store *inStore) {upCache.push_front(inStore);};
  bool isCached(const Block::block_t& inBlock);

};

inline 
bool CoopCache::isCached(const Block::block_t& inBlock)
{
  bool ret = false;
  list<Store *>::iterator iter;

  for (iter = upCache.begin(); iter != upCache.end(); iter++)
  { 
    if ((*iter)->isCached(inBlock))
    {
       ret = true;
       break;
    }
  }
  return ret;
};


class StoreCacheSimpleCoop : public StoreCache {

protected:

    CoopCache cooperative;
    uint64_t demotetime;

private:
  // Copy constructors - declared private and never defined

  StoreCacheSimpleCoop(const StoreCacheSimpleCoop&);
  StoreCacheSimpleCoop& operator=(const StoreCacheSimpleCoop&);

protected:
  virtual void BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock_t,
			  list<IORequest>& outIOReqs);

  virtual bool ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock) = 0;
  virtual void DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock) = 0;  

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
  StoreCacheSimpleCoop(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCache(inName, inNextStore, inBlockSize, inSize),
    demotetime(0)
    { ; };

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

   */
  StoreCacheSimpleCoop(const char *inName,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCache(inName, inBlockSize, inSize),
    demotetime(0) 
     { ; };

  /**
   * Destroy the cache.
   */
  ~StoreCacheSimpleCoop() { ; };

  virtual void addUpStore(Store *inStore) {cooperative.addStore(inStore);};
  virtual bool isUpCached(const Block::block_t& inBlock) {return cooperative.isCached(inBlock);};


};

#endif /* _STORECACHESIMPLECOOP_HH_ */
