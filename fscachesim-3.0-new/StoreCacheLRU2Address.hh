/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheLRU2ADDRESS.hh,v 1.4 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHELRU2ADDRESS_HH_
#define _STORECACHELRU2ADDRESS_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "ACache.hh"
#include "IORequest.hh"
#include "StoreCache.hh"

/**
 * Simple LRU/MRU block cache with support for demote operations.
 */
class StoreCacheLRU2ADDRESS : public StoreCache {
public:


private:
  ACache cache;

  bool logRequestFlag;

private:
  // Copy constructors - declared private and never defined

  StoreCacheLRU2ADDRESS(const StoreCacheLRU2ADDRESS&);
  StoreCacheLRU2ADDRESS& operator=(const StoreCacheLRU2ADDRESS&);

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
   * @param inEjectPolicy The cache ejection policy.
   * @param inDemotePolicy_t The cache demotion policy.
   */
  StoreCacheLRU2ADDRESS(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCache(inName, inNextStore, inBlockSize, inSize),
    cache(inSize) { ; };

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.
   * @param inEjectPolicy The cache ejection policy.
   * @param inDemotePolicy_t The cache demotion policy.
   */
  StoreCacheLRU2ADDRESS(const char *inName,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCache(inName, inBlockSize, inSize),
    cache(inSize) { ; };

  /**
   * Destroy the cache.
   */
  ~StoreCacheLRU2ADDRESS() { ; };

  // Statistics management

  virtual void statisticsShow() const;
  virtual bool isCached(const  Block::block_t& inBlock);
};

#endif /* _STORECACHELRU2ADDRESS_HH_ */
