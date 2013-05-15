/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreInfinite.cc,v 1.2 2002/02/18 20:56:19 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#define NDEBUG

#include <assert.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "IORequest.hh"
#include "UInt64.hh"

#include "StoreInfinite.hh"

using Block::block_t;

bool
StoreInfinite::IORequestDown(const IORequest& inIOReq)
{
  block_t block = {inIOReq.objIDGet(), inIOReq.blockOffGet(blockSize)};
  uint64_t reqBlockLen = inIOReq.blockLenGet(blockSize);

  for (uint64_t i = 0; i < reqBlockLen; i++) {
    // See if the block is cached.

    Block::Counter::iterator blockIter = blockTimestampMap.find(block);
    if (blockIter != blockTimestampMap.end()) {
      uint64_t blockTimestamp = blockIter->second;
      uint64_t blockLRUDepth;
      UInt64::Counter::iterator LRUIter;

      readHits++;

      // Splay to find the LRU depth.

      LRUTree = Splay_splay(blockTimestamp, LRUTree);
      blockLRUDepth = node_size(LRUTree->left);
      LRUTree = Splay_delete(blockTimestamp, LRUTree);

      // node_size() returns the count between the current sector and the
      // LRU one - the opposite of what we want.

      blockLRUDepth = blockTimestampMap.size() - blockLRUDepth;

      // Increment the touch count at this depth.

      LRUIter = LRUMap.find(blockLRUDepth);
      LRUMap[blockLRUDepth] = (LRUIter != LRUMap.end() ?
			       ++(LRUIter->second) :
			       1);

      // If we're using a finite cache, update the LRU queue.

      if (cache.sizeGet() != 0) {
	cache.blockGet(block);
	cache.blockPutAtTail(block);
      }
    }
    else {
      readMisses++;

      // If we're using a finite cache...

      if (cache.sizeGet() != 0) {
	if (cache.isFull()) {
	  block_t ejectBlock;

	  // ... eject the head block if necessary...

	  cache.blockGetAtHead(ejectBlock);
	  
	  // ... and remove it from the splay tree.

	  Block::Counter::iterator blockIter =
	    blockTimestampMap.find(ejectBlock);
	  assert(blockIter != blockTimestampMap.end());

	  uint64_t ejectBlockTimestamp = blockIter->second;
	  LRUTree = Splay_delete(ejectBlockTimestamp, LRUTree);

	  blockTimestampMap.erase(ejectBlock);
	  assert(blockTimestampMap.size() == (cache.sizeGet() - 1));
	}

	// Update the LRU queue.

	cache.blockPutAtTail(block);
      }
    }

    LRUTree = Splay_insert(blockTimestampClock, LRUTree);
    blockTimestampMap[block] = blockTimestampClock;

    // Increment the access count for this sector.

    if (freqFlag) {
      blockIter = freqMap.find(block);
      freqMap[block] = (blockIter != freqMap.end() ?
			++(blockIter->second) :
			1);
    }

    // Increment the sector access clock.

    blockTimestampClock++;

    block.blockID++;
  }

  return (true);
}

void
StoreInfinite::statisticsReset()
{
  // A reset for a cache doesn't wipe the infinite cache clean - it just
  // clears the LRU stack depth and freq. statistics.

  LRUMap.clear();
  freqMap.clear();

  // And of course, reset the parent.

  Store::statisticsReset();
}

void
StoreInfinite::statisticsShow() const
{
  if (freqFlag) {
    printf("Block access frequency:\n");
    statisticsFreqShow();
  }
  printf("LRU stack depth hits:\n");
  statisticsLRUShow();
  statisticsSummaryShow();
}

void
StoreInfinite::statisticsFreqShow() const
{
  for (Block::Counter::const_iterator i = freqMap.begin();
       i != freqMap.end();
       i++) {
    printf("%llu,%llu %llu\n", i->first.objID, i->first.blockID, i->second);
  }
  fflush(stdout);
}

void
StoreInfinite::statisticsLRUShow() const
{
  for (UInt64::Counter::const_iterator i = LRUMap.begin();
       i != LRUMap.end();
       i++) {
    // Convert the x-axis to MB instead of block size - the latter is not
    // an intuitive measure of size. The ordering of divisions is
    // important: it avoids integer round-off problems.

    printf("%llu %llu\n", ((i->first * (blockSize / 1024)) / 1024), i->second);
  }
  fflush(stdout);
}

void
StoreInfinite::statisticsLRUCumulShow() const
{
  uint64_t cumul = 0;
  uint64_t cumulTotal = readMisses;

  // cumulTotal doesn't start at zero since we need to account for
  // compulsory misses.

  for (UInt64::Counter::const_iterator i = LRUMap.begin();
       i != LRUMap.end();
       i++) {
    cumulTotal += i->second;
  }
  for (UInt64::Counter::const_iterator i = LRUMap.begin();
       i != LRUMap.end();
       i++) {
    // Convert the x-axis to MB instead of block size - the latter is not
    // an intuitive measure of size. The ordering of divisions is
    // important: it avoids integer round-off problems.

    cumul += i->second;
    printf("%llu %4.3f\n",
	   ((i->first * (blockSize / 1024)) / 1024),
	   ((double)cumul / cumulTotal));
  }
  fflush(stdout);
  fprintf(stderr, "\n");
  fprintf(stderr, "Cumul = %llu\n", cumul);
  fprintf(stderr, "Cumul total = %llu\n", cumulTotal);
}

void
StoreInfinite::statisticsSummaryShow() const
{
  printf("Block hits %llu\n", readHits);
  printf("Block misses %llu\n", readMisses);
  fflush(stdout);
}
