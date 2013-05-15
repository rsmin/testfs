/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSeg.cc,v 1.4 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <math.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "StoreCacheSeg.hh"

using Block::block_t;

int
StoreCacheSeg::blockGetCascade(const block_t& inBlock)
{
  int retval = cacheSegCount;

  for (int i = 0; i < cacheSegCount; i++) {
    if (cacheSegs[i]->isCached(inBlock)) {
      cacheSegs[i]->blockGet(inBlock);
      segHits[i]++;
      retval = i;
      break;
    }
  }

  return (retval);
}

void
StoreCacheSeg::blockPutAtSegCascade(const block_t& inBlock,
				    int inSeg)
{
  block_t cascadeEjectBlock = inBlock;
  bool cascadeEjectFlag = true;

  for (int i = inSeg; i >= 0 && cascadeEjectFlag; i--) {
    block_t ejectBlock = {0, 0};

    cascadeEjectFlag = false;
    if (cacheSegs[i]->isFull()) {
      cacheSegs[i]->blockGetAtHead(ejectBlock);
      cascadeEjectFlag = true;
    }

    // Cache the incoming block at the tail of the queue.

    cacheSegs[i]->blockPutAtTail(cascadeEjectBlock);
    cascadeEjectBlock = ejectBlock;
  }
}

StoreCacheSeg::StoreCacheSeg(const char *inName,
			     uint64_t inBlockSize,
			     uint64_t inSize,
			     int inSegCount,
			     bool inNormalizeFlag) :
  StoreCache(inName, inBlockSize, inSize),
  cacheSegCount(inSegCount),
  ghost(inName, inSize, inNormalizeFlag),
  segSizesUniformFlag(true)
{
  uint64_t cacheSegSize = inSize / cacheSegCount;
  uint64_t cacheSizeRemain = inSize;

  cacheSegs = new Cache *[cacheSegCount];
  segHits = new uint64_t[cacheSegCount];
  for (int i = 0; i < cacheSegCount; i++) {
    uint64_t thisSegSize = (i < cacheSegCount - 1 ?
			    cacheSegSize :
			    cacheSizeRemain);

    cacheSegs[i] = new Cache(thisSegSize);
    segHits[i] = 0;

    cacheSizeRemain -= thisSegSize;
  }
  if (cacheSizeRemain != 0) {
    abort();
  }
}

// For the exponential size segments, each segment is inSegMultiplier times
// the size of the previous segment.

StoreCacheSeg::StoreCacheSeg(const char *inName,
			     uint64_t inBlockSize,
			     uint64_t inSize,
			     int inSegCount,
			     int inSegMultiplier,
			     bool inNormalizeFlag) :
  StoreCache(inName, inBlockSize, inSize),
  cacheSegCount(inSegCount),
  ghost(inName, inSize, inNormalizeFlag),
  segSizesUniformFlag(false)
{
  cacheSegs = new Cache *[cacheSegCount];
  segHits = new uint64_t[cacheSegCount];

  // Track how much cache space remains after allocating space for each
  // segment. We will sweep remaining space into the tail segment.

  uint64_t cacheSizeRemain = inSize;

  // Each cache segment size is a fraction of the cache size. Determine the
  // denominator of the fractions. For example, if each segment is twice
  // the size of the previous segment, and there are four segments, the
  // denominator is:
  //
  // 8 4 2 1 = 15
  //
  // Thus, we divide the cache size into fifteen shares, and distribute
  // shares accordingly.

  int cacheShareCount = 0;
  for (int i = 0; i < cacheSegCount; i++) {
    cacheShareCount += ((int)pow(inSegMultiplier, i));
  }
  uint64_t cacheShareSize = inSize / cacheShareCount;

  for (int i = 0; i < cacheSegCount; i++) {
    // Assign shares to this segment. The last (tail) segment gets the most
    // shares, and also any slop hasn't already been allocated due to
    // round-off.

    uint64_t thisSegSize = (i < cacheSegCount - 1 ?
			    ((int)(pow(inSegMultiplier, i))) * cacheShareSize :
			    cacheSizeRemain);

    cacheSegs[i] = new Cache(thisSegSize);
    segHits[i] = 0;
    cacheSizeRemain -= thisSegSize;
  }

  // Make sure we assigned all of the cache.

  if (cacheSizeRemain != 0) {
    abort();
  }
}

StoreCacheSeg::~StoreCacheSeg()
{
  for (int i = 0; i < cacheSegCount; i++) {
    delete cacheSegs[i];
  }
  delete segHits;
  delete cacheSegs;
}

void
StoreCacheSeg::BlockCache(const IORequest& inIOReq,
			  const block_t& inBlock,
			  list<IORequest>& outIOReqs)
{
  const char* reqOrig = inIOReq.origGet();
  IORequest::IORequestOp_t op = inIOReq.opGet();

  // See if we have cached this block. We only update the ghost hit counts
  // when we receive a read request.

  int blockSeg = blockGetCascade(inBlock);
  if (blockSeg != cacheSegCount) {
    switch (op) {
    case IORequest::Demote:
      demoteHitsPerOrig[reqOrig]++;
      demoteHits++;
      break;
    case IORequest::Read:
      readHitsPerOrig[reqOrig]++;
      readHits++;
      ghost.probUpdate(inBlock);
      break;
    default:
      abort();
    }

    // Move the block to the MRU end of the actual LRU queue.

    blockPutAtSegCascade(inBlock, cacheSegCount - 1);
  }
  else {
    switch (op) {
    case IORequest::Demote:
      demoteMissesPerOrig[reqOrig]++;
      demoteMisses++;
      break;
    case IORequest::Read:
      readMissesPerOrig[reqOrig]++;
      readMisses++;
      ghost.probUpdate(inBlock);

      // The following code is commented out since, as implemented,
      // segmented caches are terminal devices.
#if 0
      outIOReqs.push_back(IORequest(reqOrig,
				    IORequest::Read,
				    inIOReq.timeIssuedGet(),
				    inIOReq.objIDGet(),
				    inBlock.blockID * blockSize,
				    blockSize));
#endif /* 0 */
      break;
    default:
      abort();
    }

    // Determine which bin the block will go into, and cache
    // it. Remember, the 0th segment is the closest to head of the queue
    // (i.e. the LRU end).

    int insertSeg = (int)(cacheSegCount * ghost.probGet(op));
    if (insertSeg == cacheSegCount) {
      insertSeg--;
    }
    blockPutAtSegCascade(inBlock, insertSeg);
  }
  ghost.blockPut(op, inBlock);
}

void
StoreCacheSeg::statisticsReset()
{
  for (int i = 0; i < cacheSegCount; i++) {
    segHits[i] = 0;
  }

  StoreCache::statisticsReset();
}

void
StoreCacheSeg::statisticsShow() const
{
  printf("{StoreCacheSeg.%s\n", nameGet());
if (mode != Analyze)
{

  printf("\t{segSizes=%s}\n", (segSizesUniformFlag ? "uniform" : "exp"));

  printf("\t{size ");
  uint64_t sizeTotal = 0;
  for (int i = 0; i < cacheSegCount; i++) {
    printf("{seg%d=%llu} ", i, cacheSegs[i]->sizeGet() * blockSizeGet());
    sizeTotal += cacheSegs[i]->sizeGet();
  }
  printf("{total=%llu} }\n", sizeTotal * blockSizeGet());

  printf("\t{segHits ");
  uint64_t segHitsTotal = 0;
  for (int i = 0; i < cacheSegCount; i++) {
    printf("{seg%d=%llu} ", i, segHits[i]);
    segHitsTotal += segHits[i];
  }
  printf("{total=%llu} }\n", segHitsTotal);

  ghost.statisticsShow();
}

  StoreCache::statisticsShow();
if (mode != Analyze)
{
  printf("}\n");
}
}
