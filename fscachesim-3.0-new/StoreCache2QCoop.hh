/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSeg.hh,v 1.3 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHE2QCOOP_HH_
#define _STORECACHE2QCOOP_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "Block.hh"
#include "Ghost.hh"
#include "StoreCacheSimpleCoop.hh"

/**
 * Segmented LRU cache with an adaptive cache-insertion policy
 * [Wong2002]. Incoming {read, demote} blocks are "cached" in a {read,
 * demote} ghost cache before being insterted in the actual cache. The
 * relative hit rates in the ghosts determine into which segment the
 * incoming block will be inserted.
 *
 * @warning As implemented, segmented caches are terminal storage devices:
 * no lower-level devices can be connected to them.
 */
class StoreCache2QCoop : public StoreCacheSimpleCoop {
public:
  // Policies

  /**
   * Cache demotion policy.
   */
  enum DemotePolicy_t {
    Demand, /**< Demote a block when the cache ejects the block. */
    Demand_without_Coop,
  };
private:


  Cache *Q[2][3];
  bool sq;

  DemotePolicy_t demotePolicy;

private:
  // Copy constructors - declared private and never defined

  StoreCache2QCoop(const StoreCache2QCoop&);
  StoreCache2QCoop& operator=(const StoreCache2QCoop&);

  int blockGetCascade(const Block::block_t& inBlock, IORequest::IORequestOp_t op);
  void blockPutAtSegCascade(const Block::block_t& inBlock,
			    int inSeg, IORequest::IORequestOp_t op);

protected:
  virtual bool ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);
  virtual void DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);  


public:
  /**
   * Create a segmented adaptive cache with uniform size segments.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the total cache, in blocks.

   */
  StoreCache2QCoop(const char *inName,
		uint64_t inBlockSize,
		uint64_t inSize,
                DemotePolicy_t inDemotePolicy);



  /**
   * Destroy the cache.
   */
  ~StoreCache2QCoop();

  // Statistics management


  virtual void statisticsShow() const;
};

#endif /* _STORECACHE2QCOOP_HH_ */
