/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/ACache.hh,v 1.11 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _ACACHE_HH_
#define _ACACHE_HH_

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

  typedef struct {

    uint64_t objID;

    uint64_t blockID;

    uint64_t blockAddr;

  } a_block_t;

/**
 * ACache object that supports insertions an its head and tail ends. A major
 * failing of this class is that it does not perform comprehensive bounds
 * checking during block insertions.
 */
class ACache {
private:
  typedef map<Block::block_t,
	      list<a_block_t>::iterator,
	      Block::LessThan> CacheIndex;

  /**
   * The cache.
   */
  list<a_block_t> cache;

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

private:
  // Copy constructors - declared private and never defined

  ACache(const ACache&);
  ACache& operator=(const ACache&);

public:
  /**
   * Create a ACache object.
   *
   * @param inACacheSize The maximum number of blocks the cache can hold.
   */
  ACache(uint64_t inCacheSize) :
    cache(),
    cacheIndex(),
    blockCount(0),
    blockCountMax(inCacheSize) 
    {;};

  ACache() :
    cache(),
    cacheIndex(),
    blockCount(0),
    blockCountMax(-1) { ; };

  /**
   * Destroy a ACache object.
   */
  ~ACache() { ; };

  /**
   * Get a block from the cache. Decrement the block count if the block is
   * found. Otherwise, do nothing.
   *
   * @param inBlock The block to get.
   */
  uint64_t blockGet(const Block::block_t& inBlock);

  /**
   * Get the block at the head of the cache. Copy the block into outBlock
   * and decrement the block count if the cache is not empty. Otherwise, do
   * nothing.
   *
   * @param outBlock An output block.
   */
  void blockGetAtHead(a_block_t& outBlock);

  /**
   * Put a block at the head of the cache.
   *
   * @warning Does not check if the cache is already full.
   *
   * @param outBlock The block to put.
   */
  void blockPutAtHead(const a_block_t& inBlock);

  /**
   * Put a block at the tail of the cache.
   *
   * @warning Does not check if the cache is already full.
   *
   * @param outBlock The block to put.
   */
  void blockPutAtTail(const a_block_t& inBlock);

  /**
   * Get the count of blocks in the cache.
   */
  uint64_t sizeGet() const { return (blockCountMax);};
  uint64_t numGet() const { return (blockCount);};


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
  bool isEmpty() const;
};




inline uint64_t
ACache::blockGet(const Block::block_t& inBlock)
{
  uint64_t blockAddr; 

  CacheIndex::iterator blockIter = cacheIndex.find(inBlock);
  if (blockIter != cacheIndex.end()) {
    cacheIndex.erase(blockIter);
    blockAddr = blockIter->second->blockAddr;
    cache.erase(blockIter->second);
    blockCount--;
  }
  assert(blockCount >= 0);
  return blockAddr;
};

inline void
ACache::blockGetAtHead(a_block_t& outBlock)
{
  if (!cache.empty()) {

    outBlock = *cache.begin();
    Block::block_t tmpBlock;
   
    tmpBlock.blockID = outBlock.blockID;
    tmpBlock.objID = outBlock.objID;

    cacheIndex.erase(tmpBlock);
    cache.pop_front();
    blockCount--;
  }
  assert(blockCount >= 0);
};



inline void
ACache::blockPutAtHead(const a_block_t& inBlock)
{
  Block::block_t tmpBlock;
  
  tmpBlock.blockID = inBlock.blockID;
  tmpBlock.objID = inBlock.objID;

  blockCount++;
  cache.push_front(inBlock);
  cacheIndex[tmpBlock] = cache.begin();
  assert(blockCount <= blockCountMax);
};

inline void
ACache::blockPutAtTail(const a_block_t& inBlock)
{
  Block::block_t tmpBlock;
  
  tmpBlock.blockID = inBlock.blockID;
  tmpBlock.objID = inBlock.objID;

  blockCount++;
  cache.push_back(inBlock);
  cacheIndex[tmpBlock] = --cache.end();
  assert(blockCount <= blockCountMax);
};

inline bool
ACache::isCached(const Block::block_t& inBlock) const
{
  return (cacheIndex.find(inBlock) != cacheIndex.end());
};

inline bool
ACache::isFull() const
{
  return (blockCount == blockCountMax);
};


inline bool
ACache::isEmpty() const
{
  return (cache.empty());
};

#endif /* _ACACHE_HH_ */
