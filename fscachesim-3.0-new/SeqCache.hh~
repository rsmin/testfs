/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/SeqCache.hh,v 1.11 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _SeqCache_HH_
#define _SeqCache_HH_

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

    uint64_t firstBlockID;

    uint64_t SeqLen;

  } seq_block_t;

/**
 * SeqCache object that supports insertions an its head and tail ends. A major
 * failing of this class is that it does not perform comprehensive bounds
 * checking during block insertions.
 */
class SeqCache {
private:
  typedef map<Block::block_t,
	      list<seq_block_t>::iterator,
	      Block::LessThan> CacheIndex;

  /**
   * The cache.
   */
  list<seq_block_t> cache;

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

  SeqCache(const SeqCache&);
  SeqCache& operator=(const SeqCache&);

public:
  /**
   * Create a SeqCache object.
   *
   * @param inSeqCacheSize The maximum number of blocks the cache can hold.
   */
  SeqCache(uint64_t inCacheSize) :
    cache(),
    cacheIndex(),
    blockCount(0),
    blockCountMax(inCacheSize) 
    {;};

  SeqCache() :
    cache(),
    cacheIndex(),
    blockCount(0),
    blockCountMax(-1) { ; };

  /**
   * Destroy a SeqCache object.
   */
  ~SeqCache() { ; };

  /**
   * Get a block from the cache. Decrement the block count if the block is
   * found. Otherwise, do nothing.
   *
   * @param inBlock The block to get.
   */
  seq_block_t blockGet(const Block::block_t& inBlock);

  bool blockFind(const Block::block_t& inBlock, seq_block_t& outBlock);

  /**
   * Get the block at the head of the cache. Copy the block into outBlock
   * and decrement the block count if the cache is not empty. Otherwise, do
   * nothing.
   *
   * @param outBlock An output block.
   */
  void blockGetAtHead(seq_block_t& outBlock);

  /**
   * Put a block at the head of the cache.
   *
   * @warning Does not check if the cache is already full.
   *
   * @param outBlock The block to put.
   */
  void blockPutAtHead(const seq_block_t& inBlock);

  /**
   * Put a block at the tail of the cache.
   *
   * @warning Does not check if the cache is already full.
   *
   * @param outBlock The block to put.
   */
  void blockPutAtTail(const seq_block_t& inBlock);

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




inline seq_block_t
SeqCache::blockGet(const Block::block_t& inBlock)
{
  seq_block_t outBlock;

  CacheIndex::iterator blockIter = cacheIndex.find(inBlock);
  if (blockIter != cacheIndex.end()) {
    cacheIndex.erase(blockIter);
    outBlock = *blockIter->second;
    cache.erase(blockIter->second);
    blockCount--;
  }
  assert(blockCount >= 0);
  return outBlock;
};

inline bool
SeqCache::blockFind(const Block::block_t& inBlock, seq_block_t& outBlock)
{
  bool ret = false;

     CacheIndex::iterator blockIter = cacheIndex.find(inBlock);
     if (blockIter != cacheIndex.end()) {
        outBlock = *blockIter->second;
        ret = true;
     }  
  
  return ret;
}

inline void
SeqCache::blockGetAtHead(seq_block_t& outBlock)
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
SeqCache::blockPutAtHead(const seq_block_t& inBlock)
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
SeqCache::blockPutAtTail(const seq_block_t& inBlock)
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
SeqCache::isCached(const Block::block_t& inBlock) const
{
  return (cacheIndex.find(inBlock) != cacheIndex.end());
};

inline bool
SeqCache::isFull() const
{
  return (blockCount == blockCountMax);
};


inline bool
SeqCache::isEmpty() const
{
  return (cache.empty());
};

#endif /* _SeqCache_HH_ */
