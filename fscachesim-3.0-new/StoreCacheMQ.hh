/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSeg.hh,v 1.3 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _StoreCacheMQ_HH_
#define _StoreCacheMQ_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "Block.hh"
#include "StoreCache.hh"
#include "FCache.hh"
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


class StoreCacheMQ : public StoreCache {


private:



  /**
   * The cache segments.
   */
  FCache **cacheSegs;
  
  /**
   * The number of cache segments.
   */
  int cacheSegCount;



  /**
   * The number of hits in each segment.
   */
  uint64_t *segHits;

private:
  // Copy constructors - declared private and never defined

  StoreCacheMQ(const StoreCacheMQ&);
  StoreCacheMQ& operator=(const StoreCacheMQ&);

  int blockGetCascade(const Block::block_t& inBlock, unsigned int& freq);
  void blockPutAtSegCascade(const Block::block_t& inBlock, unsigned int freq,
			    int inSeg);


  int posGet(unsigned int freq);




public:
  /**
   * Create a segmented adaptive cache with uniform size segments.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the total cache, in blocks.
   * @param inSegCount The number of segments.
   * @param inNormalizeFlag If true, normalize the ghost cache probabilities.
   */
  StoreCacheMQ(const char *inName,
		uint64_t inBlockSize,
		uint64_t inSize,
		int inSegCount);

  /**
   * Create a segmented adaptive cache with exponential size segments.  The
   * next segment is inSegMultiplier times the size of the previous
   * segment. The tail segment (the eject-me-last end) is the largest.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device (can be NULL).
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the total cache, in blocks.
   * @param inSegCount The number of segments.
   * @param inSegMultiplier The multipler for segment sizes. 
   * @param inNormalizeFlag If true, normalize the ghost cache probabilities.
   */
  StoreCacheMQ(const char *inName,
		uint64_t inBlockSize,
		uint64_t inSize,
		int inSegCount,
		int inSegMultiplier);

  /**
   * Destroy the cache.
   */
  ~StoreCacheMQ();

  virtual void BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock,
			  list<IORequest>& outIOReqs);

  // Statistics management

  virtual void statisticsReset();

  virtual void statisticsShow() const;
};

#endif /* _StoreCacheMQ_HH_ */
