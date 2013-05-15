/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.hh,v 1.4 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHESIMPLE_HH_
#define _STORECACHESIMPLE_HH_

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

/**
 * Simple LRU/MRU block cache with support for demote operations.
 */
class StoreCacheSimple : public StoreCache {
public:
  // Policies

  /**
   * Cache demotion policy.
   */
  enum DemotePolicy_t {
    None, /**< No demotes. */
    Demand /**< Demote a block when the cache ejects the block. */
  };

  /**
   * Cache ejection policy.
   */
  enum EjectPolicy_t {
    LRU, /**< LRU: Eject blocks from the head of the cache. */
    MRU /**< MRU: Eject blocks from the tail of the cache. */
  };

private:
  Cache cache;
  EjectPolicy_t ejectPolicy;
  DemotePolicy_t demotePolicy;

  bool logRequestFlag;

private:
  // Copy constructors - declared private and never defined

  StoreCacheSimple(const StoreCacheSimple&);
  StoreCacheSimple& operator=(const StoreCacheSimple&);

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
  StoreCacheSimple(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize,
		   EjectPolicy_t inEjectPolicy,
		   DemotePolicy_t inDemotePolicy) :
    StoreCache(inName, inNextStore, inBlockSize, inSize),
    cache(inSize),
    ejectPolicy(inEjectPolicy),
    demotePolicy(inDemotePolicy) { ; };

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.
   * @param inEjectPolicy The cache ejection policy.
   * @param inDemotePolicy_t The cache demotion policy.
   */
  StoreCacheSimple(const char *inName,
		   uint64_t inBlockSize,
		   uint64_t inSize,
		   EjectPolicy_t inEjectPolicy,
		   DemotePolicy_t inDemotePolicy) :
    StoreCache(inName, inBlockSize, inSize),
    cache(inSize),
    ejectPolicy(inEjectPolicy),
    demotePolicy(inDemotePolicy) { ; };

  /**
   * Destroy the cache.
   */
  ~StoreCacheSimple() { ; };

  // Statistics management

  virtual void statisticsShow() const;
  virtual bool isCached(const  Block::block_t& inBlock);
};

#endif /* _STORECACHESIMPLE_HH_ */
