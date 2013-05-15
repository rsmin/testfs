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


#include "IORequest.hh"

#include "StoreCacheMQ.hh"
#include "util.hh"

using Block::block_t;


int
StoreCacheMQ::blockGetCascade(const block_t& inBlock, unsigned int& freq)
{
  int retval = -1;

  for (int i = 0; i < cacheSegCount+1 ; i++) {
    if (cacheSegs[i]->isCached(inBlock)) {
      	cacheSegs[i]->blockGet(inBlock, freq);
      segHits[i]++;
      retval = i;
      break;
    }
  }

  return (retval);
}

void
StoreCacheMQ::blockPutAtSegCascade(const block_t& inBlock, unsigned int freq,
				    int inSeg)
{
  block_t cascadeEjectBlock = inBlock;
  unsigned int cascadeejectfreq = freq; 
  bool cascadeEjectFlag = true;


  for (int i = inSeg; i >= 0 && cascadeEjectFlag; i--) {
    block_t ejectBlock = {0, 0};
    unsigned int ejectfreq;

    cascadeEjectFlag = false;
    if (cacheSegs[i]->isFull()) {
      cacheSegs[i]->blockGetAtHead(ejectBlock, ejectfreq);
      cascadeEjectFlag = true;
    }

    // Cache the incoming block at the tail of the queue.

    cacheSegs[i]->blockPutAtTail(cascadeEjectBlock, cascadeejectfreq);
    cascadeEjectBlock = ejectBlock;
    cascadeejectfreq = ejectfreq;
  }
}

StoreCacheMQ::StoreCacheMQ(const char *inName,
			     uint64_t inBlockSize,
			     uint64_t inSize,
			     int inSegCount) :
  StoreCache(inName, inBlockSize,inSize),
  cacheSegCount(inSegCount)
{
  uint64_t cacheSegSize = inSize / cacheSegCount;
  uint64_t cacheSizeRemain = inSize;

  cacheSegs = new FCache *[cacheSegCount+1];
  segHits = new uint64_t[cacheSegCount];
  for (int i = 1; i < cacheSegCount+1; i++) {
    uint64_t thisSegSize = (i < cacheSegCount ?
			    cacheSegSize :
			    cacheSizeRemain);

    cacheSegs[i] = new FCache(thisSegSize);
    segHits[i] = 0;

    cacheSizeRemain -= thisSegSize;
  }
  if (cacheSizeRemain != 0) {
    abort();
  }
  cacheSegs[0] = new FCache(inSize/2);
}

// For the exponential size segments, each segment is inSegMultiplier times
// the size of the previous segment.

StoreCacheMQ::StoreCacheMQ(const char *inName,
			     uint64_t inBlockSize,
			     uint64_t inSize,
			     int inSegCount,
			     int inSegMultiplier) :
  StoreCache(inName, inBlockSize,inSize),
  cacheSegCount(inSegCount)
{
  cacheSegs = new FCache *[cacheSegCount+1];
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

  for (int i = 1; i < cacheSegCount+1; i++) {
    // Assign shares to this segment. The last (tail) segment gets the most
    // shares, and also any slop hasn't already been allocated due to
    // round-off.

    uint64_t thisSegSize = (i < cacheSegCount - 1 ?
			    ((int)(pow(inSegMultiplier, i))) * cacheShareSize :
			    cacheSizeRemain);


    cacheSegs[i] = new FCache(thisSegSize);
    segHits[i] = 0;
    cacheSizeRemain -= thisSegSize;
  }

  // Make sure we assigned all of the cache.

  if (cacheSizeRemain != 0) {
    abort();
  }
  cacheSegs[0] = new FCache(inSize/2);
}

StoreCacheMQ::~StoreCacheMQ()
{
  for (int i = 0; i < cacheSegCount+1; i++) {
    delete cacheSegs[i];
  }
  delete segHits;
  delete cacheSegs;
}

void
StoreCacheMQ::BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock,
			  list<IORequest>& outIOReqs)
{

   if (inIOReq.opGet() != IORequest::Read)
	return;

  unsigned int freq;

  const char* reqOrig = inIOReq.origGet();

  // See if we have cached this block. We only update the ghost hit counts
  // when we receive a read request.

  int blockSeg = blockGetCascade(inBlock, freq);
  if (blockSeg > 0) {
      readHitsPerOrig[reqOrig]++;
      readHits++;

      freq++;

  }
  else {
      if (blockSeg == 0)
          freq++;
      else
          freq = 1;

      readMissesPerOrig[reqOrig]++;
      readMisses++;

    }

  blockPutAtSegCascade(inBlock, freq, posGet(freq));


}



int StoreCacheMQ::posGet(unsigned int freq)
{
  int pos;

     if (freq > 0) 
       pos = log2((uint64_t)freq) + 1;
     else 
       abort();

     if (pos  > cacheSegCount)
        pos  = cacheSegCount;
     return pos;

}

void
StoreCacheMQ::statisticsReset()
{
  for (int i = 0; i < cacheSegCount; i++) {
    segHits[i] = 0;
  }

  StoreCache::statisticsReset();
}

void
StoreCacheMQ::statisticsShow() const
{
  printf("{StoreCacheSeg.%s\n", nameGet());
if (mode != Analyze)
{
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


}

  StoreCache::statisticsShow();
if (mode != Analyze)
{
  printf("}\n");
}
}
