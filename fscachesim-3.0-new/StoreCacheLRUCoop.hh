/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.hh,v 1.4 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHELRUCOOP_HH_
#define _STORECACHELRUCOOP_HH_

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
#include "StoreCacheSimpleCoop.hh"

/**
 * Simple LRU/MRU block cache with support for demote operations.
 */
class StoreCacheLRUCoop : public StoreCacheSimpleCoop {
public:
  // Policies

  /**
   * Cache demotion policy.
   */
  enum DemotePolicy_t {
    Demand, /**< Demote a block when the cache ejects the block. */
    Demand_without_Coop,
    Demand_NoCacheRead,
    Demand_NoCacheRead_without_Coop,
    None
  };
protected:
  Cache cache;
  DemotePolicy_t demotePolicy;

private:
  // Copy constructors - declared private and never defined

  StoreCacheLRUCoop(const StoreCacheLRUCoop&);
  StoreCacheLRUCoop& operator=(const StoreCacheLRUCoop&);

protected:
  virtual bool ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);
  virtual void DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);  

  virtual void PutReadBlock(const Block::block_t& inBlock,bool haslastBlock,
                            const Block::block_t& lastBlock) ;
  virtual void PutDemoteBlock(const Block::block_t& inBlock,bool haslastBlock,
                             const Block::block_t& lastBlock) ;  

public:
  // Constructors and destructors

  /**
   * Create a LRU COOP cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device, if any.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

  StoreCacheLRUCoop(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize,
                   DemotePolicy_t inDemotePolicy) :
    StoreCacheSimpleCoop(inName, inNextStore, inBlockSize),
    cache(inSize),
    demotePolicy(inDemotePolicy)  { ; };

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

   */
  StoreCacheLRUCoop(const char *inName,
		   uint64_t inBlockSize,
		   uint64_t inSize,
                   DemotePolicy_t inDemotePolicy) :
   StoreCacheSimpleCoop(inName, inBlockSize, inSize),
   cache(inSize),
   demotePolicy(inDemotePolicy){ ; };

  /**
   * Destroy the cache.
   */
  ~StoreCacheLRUCoop() { ; };

  // Statistics management

  virtual void statisticsShow() const;
};

#endif /* _STORECACHELRUCOOP_HH_ */
