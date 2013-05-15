

#ifndef _LFUCACHE_HH_
#define _LFUCACHE_HH_

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
#include "FreqBlock.hh"


class LFUCache {
private:


  typedef map<FreqBlock::check_time_t,
	      list<FreqBlock::freq_block_t>::iterator,
              FreqBlock::LessThan> TimeIndex;

  typedef map<FreqBlock::freq_time_t,
	      list<FreqBlock::freq_block_t>::iterator,
	      FreqBlock::LessThan> LFUIndex;

  typedef map<Block::block_t,
	      list<FreqBlock::freq_block_t>::iterator,
	      Block::LessThan> CacheIndex;

  typedef map<Block::block_t,
	      list<FreqBlock::history_t>::iterator,
	      Block::LessThan> HistoryIndex;

  /**
   * The cache.
   */
  list<FreqBlock::freq_block_t> cache;

  /**
   * An index into the cache, keyed by blocks.
   */
  LFUIndex lfuIndex;
  CacheIndex cacheIndex;
  TimeIndex timeIndex;

 // history  and index
  list<FreqBlock::history_t> history;
 
  HistoryIndex historyIndex;

  /**
   * The block count.
   */
  uint64_t blockCount;

  /**
   * The maximum block count.
   */
  uint64_t blockCountMax;
  uint64_t lasttime;


private:
  // Copy constructors - declared private and never defined

  LFUCache(const LFUCache&);
  LFUCache& operator=(const LFUCache&);

  void updateoldBlock(uint64_t time);
  bool blockRemove(list<FreqBlock::freq_block_t>::iterator blockIter);
  bool blockRemove(const Block::block_t& inBlock);
  bool blockRemove(LFUIndex::iterator lfuIter);

public:
  /**
   * Create a LFUCache object.
   *
   * @param inLFUCacheSize The maximum number of blocks the cache can hold.
   */
  LFUCache(uint64_t inLFUCacheSize);

  ~LFUCache() {;};


  bool blockGet(const Block::block_t& inBlock, const uint64_t& time);

  bool blockPut(const Block::block_t& inBlock, const uint64_t& time );



  uint64_t sizeGet() const { return (blockCountMax);};


  bool isCached(const Block::block_t& inBlock) const;

  bool isFull() const;

  void postShow() ;
};



#endif /* _LFUCACHE_HH_ */
