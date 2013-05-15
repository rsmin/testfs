#ifndef _WRITECACHE_HH_
#define _WRITECACHE_HH_

using namespace std;

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define NDEBUG

#include <assert.h>
#include <list>
#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include "Block.hh"
//
/**
 * writeCache object that supports insertions an its head and tail ends. A major
 * failing of this class is that it does not perform comprehensive bounds
 * checking during block insertions.
 */



class writeCache {
private:

	const uint64_t read_latency;
	const uint64_t write_latency;
	const uint64_t spindown_latency;
	const uint64_t spinup_latency;

  typedef struct {

    uint64_t objID;
    uint64_t blockID;

  } block_t;

  typedef struct {
	uint64_t objID;
    uint64_t time;
    char status; //the disk status should be 'A' for active, 'I' for Idle and 'S' for spinning down
  } diskActivityHistory_t;

  typedef map<block_t,
	      list<block_t>::iterator,
	      Block::LessThan> cacheIndex;

 /*

   typedef map<diskActivityHistory_t,
	      list<diskActivityHistory_t>::iterator,
	      Block::LessThan> diskActivityHistoryIndex;
 */
  /**
   * The write back cache.
   */
  list<block_t> cache;

  cacheIndex cacheIndex;

 // Disk operation history  and index
  list<diskActivityHistory_t> diskActivityHistory;

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

  writeCache(const writeCache&);
  writeCache& operator=(const writeCache&);


public:
  /**
   * Create a LFUCache object.
   *
   * @param inLFUCacheSize The maximum number of blocks the cache can hold.
   */
   writeCache(uint64_t inWriteCacheSize) :
   cache(),
   cacheIndex(),
   diskActivityHistory(),
   blockCount(0),
   blockCountMax(inWriteCacheSize)
   { ; };

  ~writeCache() {;};


  void writeBlockGet(const Block::block_t& inBlock);

    /**
     * Get the block at the head of the cache. Copy the block into outBlock
     * and decrement the block count if the cache is not empty. Otherwise, do
     * nothing.
     *
     * @param outBlock An output block.
     */
    void writeBlockGetAtHead(Block::block_t& outBlock);

    /**
     * Put a block at the head of the cache.
     *
     * @warning Does not check if the cache is already full.
     *
     * @param outBlock The block to put.
     */
    void writeBlockPutAtHead(const Block::block_t& inBlock);

    /**
     * Put a block at the tail of the cache.
     *
     * @warning Does not check if the cache is already full.
     *
     * @param outBlock The block to put.
     */
    void writeBlockPutAtTail(const Block::block_t& inBlock);

    /**
     * Get the count of blocks in the cache.
     */
    uint64_t sizeGet() const { return (blockCountMax);};
    void sizeSet(uint64_t size);
    uint64_t numGet() const { return (blockCount);};


    /**
     * Find out if inBlock is cached. If it is, return true. Otherwise,
     * return false.
     *
     * @param inBlock The block to find.
     */
    bool writeIsCached(const Block::block_t& inBlock) const;

    /**
     * Find out if the cache is full. If it is, return true. Otherwise,
     * return false.
     */
    bool writeIsFull() const;
    bool writeIsEmpty() const;

    void putDiskActivityHistory
    	(const diskActivityHistory_t inDiskActivityHistory)

    void checkLastDiskActivity
    	(diskActivityHistory_t& outLastDiskActivity)

  };

inline void
writeCache::putDiskActivityHistory
	(const diskActivityHistory_t inDiskActivityHistory)
{
	diskActivityHistory.push_front(inDiskActivityHistory);
	diskActivityHistoryIndex[inDiskActivityHistory] = diskActivityHistory.begin();
}

inline void
writeCache::checkLastDiskActivity(diskActivityHistory_t& outLastDiskActivity)
{
	if (!diskActivityHistory.empty())
	outLastDiskActivity = *diskActivityHistory.begin();
};







  inline void
  writeCache::writeBlockGet(const Block::block_t& inBlock)
  {
    CacheIndex::iterator blockIter = cacheIndex.find(inBlock);
    if (blockIter != cacheIndex.end()) {
      cacheIndex.erase(blockIter);
      cache.erase(blockIter->second);
      blockCount--;
    }
    assert(blockCount >= 0);
  };

  inline void
  writeCache::writeBlockGetAtHead(Block::block_t& outBlock)
  {
    if (!cache.empty()) {
      outBlock = *cache.begin();
      cacheIndex.erase(cache.front());
      cache.pop_front();
      blockCount--;
    }
    assert(blockCount >= 0);
  };



  inline void
  writeCache::writeBlockPutAtHead(const Block::block_t& inBlock)
  {
    blockCount++;
    cache.push_front(inBlock);
    cacheIndex[inBlock] = cache.begin();
    assert(blockCount <= blockCountMax);
  };

  inline void
  writeCache::writeBlockPutAtTail(const Block::block_t& inBlock)
  {
    blockCount++;
    cache.push_back(inBlock);
    cacheIndex[inBlock] = --cache.end();
    assert(blockCount <= blockCountMax);
  };

  inline bool
  writeCache::writeIsCached(const Block::block_t& inBlock) const
  {
    return (cacheIndex.find(inBlock) != cacheIndex.end());
  };

  inline bool
  writeCache::writeIsFull() const
  {
    return (blockCount == blockCountMax);
  };

  inline void
  writeCache::sizeSet(uint64_t size)
  {
     blockCountMax = size;
    if (blockCountMax < blockCount)
    {
       printf(" cache set size error\n");
       abort();
    }
  }

  inline bool
  writeCache::writeIsEmpty() const
  {
    return (cache.empty());
  };

  #endif /* _CACHE_HH_ */


#endif /* _LFUCACHE_HH_ */
