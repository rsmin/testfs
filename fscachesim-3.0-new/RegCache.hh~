/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/RegCache.hh,v 1.11 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _REGCACHE_HH_
#define _REGCACHE_HH_

using namespace std;

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#define NDEBUG

#include <assert.h>
#include <list>
#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "MemoryRegion.hh"
#include "Char.hh"

/**
 * RegCache object that supports insertions an its head and tail ends. A major
 * failing of this class is that it does not perform comprehensive bounds
 * checking during block insertions.
 */

#define REG_PERREG 7.42
#define REG_PERPAGE 0.77

#define DISREG_PERREG 1.1
#define DISREG_PERPAGE 0.22

class RegCache {
protected:
  typedef map<MemoryRegion::memory_region_t,
	      list<MemoryRegion::memory_region_t>::iterator,
	      MemoryRegion::LessThan> RegCacheIndex;

  /**
   * The cache.
   */
  list<MemoryRegion::memory_region_t> cache;

  /**
   * An index into the cache, keyed by blocks.
   */
  RegCacheIndex cacheIndex;

  /**
   * The block count.
   */
  uint64_t blockCount;

  /**
   * The maximum block count.
   */
  uint64_t blockCountMax;

  uint64_t reghit;
  uint64_t regmiss;
  uint64_t regparthit;
  double totaltime;

  Char::Counter reqlens;
 
private:
  // Copy constructors - declared private and never defined

  RegCache(const RegCache&);
  RegCache& operator=(const RegCache&);

public:
  /**
   * Create a RegCache object.
   *
   * @param inRegCacheSize The maximum number of blocks the cache can hold.
   */
  RegCache(uint64_t inRegCacheSize) :
    cache(),
    cacheIndex(),
    blockCount(0),
    blockCountMax(inRegCacheSize),
    reghit(0),
    regparthit(0),
    regmiss(0),
    totaltime(0),
    log (false),
    L(0) { ; };

  /**
   * Destroy a RegCache object.
   */
  ~RegCache() { ; };

  virtual void statisticsShow();


  void RegionRef(const MemoryRegion::memory_region_t& inRegion);


  uint64_t sizeGet() const { return (blockCountMax);};
  uint64_t numGet() const { return (blockCount);};


  bool isFull() const;
  bool isEmpty() const;

  bool log;


protected:
  virtual void CalcReg(RegCacheIndex::iterator& regionIter, 
                       const MemoryRegion::memory_region_t& inRegion, 
                       list<MemoryRegion::memory_region_t>& mr, 
                       uint64_t& regsize) = 0;

  virtual void EvictCache(uint64_t regsize) = 0; 

  virtual void sendtotail(RegCacheIndex::iterator& regionIter);

  void addnewreg(MemoryRegion::memory_region_t& inRegion);

  void evictOne(uint64_t regsize);
  void evictBatch(uint64_t regsize);
  void evictSL(uint64_t regsize);

private:

  typedef multimap<double,
	      list<MemoryRegion::memory_region_t>::iterator> ResortIndex;

  typedef pair <double,
	      list<MemoryRegion::memory_region_t>::iterator> pair_resort;

  ResortIndex evictIndex;

  double L;
 
};

#endif /* _REGCACHE_HH_ */
