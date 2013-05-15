/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSLRU.cc,v 1.5 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "StoreCacheSLRU.hh"

using Block::block_t;

/**
 * Receive an incoming I/O request sent down from a higher-level block
 * store or request generator.
 * 
 * When a block first arrives (regardless of whether they were read in or
 * demoted down), it goes into the probationary cache. If some subsequent
 * request hits the block, the block moves it into the protected cache.
 */
void
StoreCacheSLRU::BlockCache(const IORequest& inIOReq,
			   const block_t& inBlock,
			   list<IORequest>& outIOReqs)
{
  if (protCache.isCached(inBlock)) {

    // Block is in the protected cache.

    // Eject the block (we will re-cache it later).

    protCache.blockGet(inBlock);

    switch (inIOReq.opGet()) {
    case IORequest::Demote:
      protDemoteHitsPerOrig[inIOReq.origGet()]++;
      demoteHitsPerOrig[inIOReq.origGet()]++;
      demoteHits++;
      break;
    case IORequest::Read:
      protReadHitsPerOrig[inIOReq.origGet()]++;
      readHitsPerOrig[inIOReq.origGet()]++;
      readHits++;
      break;
    default:
      abort();
    }

    // Re-cache the block at the end of the protected cache.

    protCache.blockPutAtTail(inBlock);
  }
  else if (probCache.isCached(inBlock)) {

    // Block is in the probationary cache.

    // Eject the block (we will re-cache it later).

    probCache.blockGet(inBlock);

    // Move a block to the probationary cache if the protected cache is
    // full.

    if (protCache.isFull()) {
      block_t protToProbBlock;

      protCache.blockGetAtHead(protToProbBlock);
      probCache.blockPutAtTail(protToProbBlock);
      protToProbXfersPerOrig[inIOReq.origGet()]++;
    }

    switch (inIOReq.opGet()) {
    case IORequest::Demote:
      probDemoteHitsPerOrig[inIOReq.origGet()]++;
      demoteHitsPerOrig[inIOReq.origGet()]++;
      demoteHits++;
      break;
    case IORequest::Read:
      probReadHitsPerOrig[inIOReq.origGet()]++;
      readHitsPerOrig[inIOReq.origGet()]++;
      readHits++;
      break;
    default:
      abort();
    }

    protCache.blockPutAtTail(inBlock);
    probToProtXfersPerOrig[inIOReq.origGet()]++;
  }
  else {

    // Block isn't cached.

    // Eject the front block of the probationary cache if it is full.

    if (probCache.isFull()) {
      block_t ejectBlock;

      probCache.blockGetAtHead(ejectBlock);
    }

    switch (inIOReq.opGet()) {
    case IORequest::Demote:
      demoteMissesPerOrig[inIOReq.origGet()]++;
      demoteMisses++;
      break;
    case IORequest::Read:
      readMissesPerOrig[inIOReq.origGet()]++;
      readMisses++;

      // Create a new IORequest to pass on to the next-level node.

      if (nextStore) {
	outIOReqs.push_back(IORequest(inIOReq.origGet(),
				      IORequest::Read,
				      0,
				      inIOReq.objIDGet(),
				      inBlock.blockID * blockSize,
				      blockSize));
      }
      break;
    default:
      abort();
    }

    // Cache the block at the end of the probationary cache.

    probCache.blockPutAtTail(inBlock);
  }
}

void
StoreCacheSLRU::statisticsReset()
{
  protDemoteHitsPerOrig.clear();
  protReadHitsPerOrig.clear();

  probDemoteHitsPerOrig.clear();
  probReadHitsPerOrig.clear();

  probToProtXfersPerOrig.clear();
  protToProbXfersPerOrig.clear();

  StoreCache::statisticsReset();
}

void
StoreCacheSLRU::statisticsShow() const
{
  printf("{StoreCacheSLRU.%s\n", nameGet());

  printf("\t{size {total=%llu} {prob=%llu} {prob=%llu} }\n",
	 (probCache.sizeGet() + protCache.sizeGet()) * blockSizeGet(),
	 probCache.sizeGet() * blockSizeGet(),
	 protCache.sizeGet() * blockSizeGet());

  printf("\t{probReadHitsPerOrig ");
  for (Char::Counter::const_iterator i = probReadHitsPerOrig.begin();
       i != probReadHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{probDemoteHitsPerOrig ");
  for (Char::Counter::const_iterator i = probDemoteHitsPerOrig.begin();
       i != probDemoteHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{protReadHitsPerOrig ");
  for (Char::Counter::const_iterator i = protReadHitsPerOrig.begin();
       i != protReadHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{protDemoteHitsPerOrig ");
  for (Char::Counter::const_iterator i = protDemoteHitsPerOrig.begin();
       i != protDemoteHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{probToProtXfersPerOrig ");
  for (Char::Counter::const_iterator i = probToProtXfersPerOrig.begin();
       i != probToProtXfersPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");
  printf("\t{protToProbXfersPerOrig ");
  for (Char::Counter::const_iterator i = protToProbXfersPerOrig.begin();
       i != protToProbXfersPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  StoreCache::statisticsShow();

  printf("}\n");
}
