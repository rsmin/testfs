/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSLRU.hh,v 1.3 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHESLRU_HH_
#define _STORECACHESLRU_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "Block.hh"
#include "Cache.hh"
#include "StoreCache.hh"

/**
 * SLRU block cache [Karedla1994] with support incoming demotions sent down
 * from a higher-level block store or request generator.
 *
 * @warning Will not demotions to send to a lower-level store.
 */
class StoreCacheSLRU : public StoreCache {
private:
  Cache probCache;
  Cache protCache;

protected:
  /**
   * Per-originator counts of demotes that hit in the probationary cache.
   */
  Char::Counter probDemoteHitsPerOrig;

  /**
   * Per-originator counts of reads that hit in the probationary cache.
   */
  Char::Counter probReadHitsPerOrig;

  /**
   * Per-originator counts of demotes that hit in the protected cache.
   */
  Char::Counter protDemoteHitsPerOrig;

  /**
   * Per-originator counts of reads that hit in the protected cache.
   */
  Char::Counter protReadHitsPerOrig;

  /**
   * Per-originator counts of blocks that get transferred from the
   * probationary to the protected cache.
   */
  Char::Counter probToProtXfersPerOrig;

  /**
   * Per-originator counts of blocks that get transferred from the
   * protected to the probationary cache.
   */
  Char::Counter protToProbXfersPerOrig;

protected:
  // Documented at the definition.
  virtual void BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock,
			  list<IORequest>& outIOReqs);

public:
  /**
   * Create an SLRU cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device (can be NULL).
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the total cache, in blocks.
   * @param inProbSize The size of the probationary cache, in blocks.
   */
  StoreCacheSLRU(const char *inName,
		 Store *inNextStore,
		 uint64_t inBlockSize,
		 uint64_t inSize,
		 uint64_t inProbSize) :
    StoreCache(inName, inNextStore, inBlockSize, inSize),
    probCache(inProbSize),
    protCache(inSize - inProbSize) { ; };

  /**
   * Create an SLRU cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the total cache, in blocks.
   * @param inProbSize The size of the probationary cache, in blocks.
   */
  StoreCacheSLRU(const char *inName,
		 uint64_t inBlockSize,
		 uint64_t inSize,
		 uint64_t inProbSize) :
    StoreCache(inName, inBlockSize, inSize),
    probCache(inProbSize),
    protCache(inSize - inProbSize) { ; };

  /**
   * Destroy the cache.
   */
  ~StoreCacheSLRU() { ; };

  // Statistics management

  virtual void statisticsReset();

  virtual void statisticsShow() const;
};

#endif /* _STORECACHESLRU_HH_ */
