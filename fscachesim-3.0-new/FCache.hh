/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/FCache.hh,v 1.11 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _FCACHE_HH_
#define _FCACHE_HH_

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

#include "Block.hh"

/**
 * FCache object that supports insertions an its head and tail ends. A major
 * failing of this class is that it does not perform comprehensive bounds
 * checking during block insertions.
 */



class FCache {
private:


  typedef struct {

    uint64_t objID;
    uint64_t blockID;

    unsigned int Freq; 

  } fblock_t;

  typedef map<Block::block_t,
	      list<fblock_t>::iterator,
	      Block::LessThan> CacheIndex;


  /**
   * The cache.
   */
  list<fblock_t> cache;

  /**
   * An index into the cache, keyed by blocks.
   */
  CacheIndex cacheIndex;

  /**
   * The block count.
   */
  uint64_t blockCount;

  /**
   * The maximum block count.
   */
  uint64_t blockCountMax;

  fblock_t fb;

private:
  // Copy constructors - declared private and never defined

  FCache(const FCache&);
  FCache& operator=(const FCache&);

public:
  /**
   * Create a FCache object.
   *
   * @param inFCacheSize The maximum number of blocks the cache can hold.
   */
  FCache(uint64_t inFCacheSize) :
    cache(),
    cacheIndex(),
    blockCount(0),
    blockCountMax(inFCacheSize) { ; };

  /**
   * Destroy a FCache object.
   */
  ~FCache() { ; };

  /**
   * Get a block from the cache. Decrement the block count if the block is
   * found. Otherwise, do nothing.
   *
   * @param inBlock The block to get.
   */
  void blockGet(const Block::block_t& inBlock, unsigned int& freq);


  void blockGetAtHead(Block::block_t& inBlock, unsigned int& freq);

  /**
   * Put a block at the head of the cache.
   *
   * @warning Does not check if the cache is already full.
   *
   * @param outBlock The block to put.
   */
  void blockPutAtHead(const Block::block_t& inBlock, unsigned int freq);

  /**
   * Put a block at the tail of the cache.
   *
   * @warning Does not check if the cache is already full.
   *
   * @param outBlock The block to put.
   */
  void blockPutAtTail(const Block::block_t& inBlock, unsigned int freq);

  /**
   * Get the count of blocks in the cache.
   */
  uint64_t sizeGet() const { return (blockCountMax);};

  /**
   * Find out if inBlock is cached. If it is, return true. Otherwise,
   * return false.
   *
   * @param inBlock The block to find.
   */
  bool isCached(const Block::block_t& inBlock) const;

  /**
   * Find out if the cache is full. If it is, return true. Otherwise,
   * return false.
   */
  bool isFull() const;
};




inline void
FCache::blockGet(const Block::block_t& inBlock, unsigned int& freq)
{
  CacheIndex::iterator blockIter = cacheIndex.find(inBlock);
  if (blockIter != cacheIndex.end()) {
    cacheIndex.erase(blockIter);
    freq = blockIter->second->Freq;
    cache.erase(blockIter->second);
    blockCount--;
  }
  assert(blockCount >= 0);
};


inline void
FCache::blockGetAtHead(Block::block_t& outBlock, unsigned int& freq)
{
  if (!cache.empty()) {
    outBlock.objID = cache.begin()->objID;
    outBlock.blockID = cache.begin()->blockID;
    freq = cache.begin()->Freq;  
    cacheIndex.erase(outBlock);
    cache.pop_front();
    blockCount--;
  }
  assert(blockCount >= 0);
};


inline void
FCache::blockPutAtHead(const Block::block_t& inBlock, unsigned int freq)
{
  fb.objID = inBlock.objID;
  fb.blockID = inBlock.blockID;
  fb.Freq = freq;
  blockCount++;
  cache.push_front(fb);
  cacheIndex[inBlock] = cache.begin();
  assert(blockCount <= blockCountMax);
};

inline void
FCache::blockPutAtTail(const Block::block_t& inBlock, unsigned int freq)
{
  fb.objID = inBlock.objID;
  fb.blockID = inBlock.blockID;
  fb.Freq = freq;
  blockCount++;
  cache.push_back(fb);
  cacheIndex[inBlock] = --cache.end();
  assert(blockCount <= blockCountMax);
};

inline bool
FCache::isCached(const Block::block_t& inBlock) const
{
  return (cacheIndex.find(inBlock) != cacheIndex.end());
};

inline bool
FCache::isFull() const
{
  return (blockCount == blockCountMax);
};

#endif /* _FCACHE_HH_ */
