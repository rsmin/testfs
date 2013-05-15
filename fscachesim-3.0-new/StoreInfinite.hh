/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreInfinite.hh,v 1.2 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STOREINFINITE_HH_
#define _STOREINFINITE_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

extern "C" {
#include "top-down-size-splay.h"
}

#include "Block.hh"
#include "Store.hh"
#include "Cache.hh"
#include "UInt64.hh"

/**
 * Infinite LRU block cache.
 */
class StoreInfinite : public Store {
private:

  // These data structures together form the 'cache'.

  Block::Counter blockTimestampMap;
  uint64_t blockTimestampClock;
  Tree *LRUTree;

  // For finite caches.

  Cache cache;

  // These keep stats on the cache.

  UInt64::Counter LRUMap; // LRU stack depth
  Block::Counter freqMap; // Block access frequency

  // Keep the frequency count or not?

  bool freqFlag;

private:
  // Copy constructors - declared private and never defined

  StoreInfinite(const StoreInfinite&);
  StoreInfinite& operator=(const StoreInfinite&);

public:
  /**
   * Create an infinite block cache.
   *
   * There is no lower-level device below an infinite cache (of course).
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.
   */
  StoreInfinite(const char *inName,
		uint64_t inBlockSize,
		uint64_t inSize,
		bool inFreqFlag) :
    Store(inName, NULL, inBlockSize),
    blockTimestampMap(),
    blockTimestampClock(0),
    LRUTree(NULL),
    cache(inSize),
    LRUMap(),
    freqMap(),
    freqFlag(inFreqFlag) { ; };

  /**
   * Destroy the infinite cache.
   */
  ~StoreInfinite() { ; };

  // I/O request handlers

  virtual bool IORequestDown(const IORequest& inIOReq);

  // Statistics management

  virtual void statisticsReset();

  virtual void statisticsShow() const;

  /**
   * Show block access frequencies.
   */
  void statisticsFreqShow() const;

  /**
   * Show hit rate against LRU cache size.
   */
  void statisticsLRUShow() const;

  /**
   * Show cumulative hit rate against LRU cache size.
   */
  void statisticsLRUCumulShow() const;

  /**
   * Show block hit/miss counts.
   */
  void statisticsSummaryShow() const;
};

#endif /* _STOREINFINITE_HH_ */
