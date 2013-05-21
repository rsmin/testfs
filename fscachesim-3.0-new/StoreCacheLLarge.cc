/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheLarge.cc,v 1.5 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "StoreCacheLarge.hh"

using Block::block_t;

/**
 * This cache maintains one ejection queue. The head of the queue is the
 * eject-me-next block. Hence, for LRU and MRU we add blocks at the tail
 * and head respectively.
 */



void
StoreCacheLarge::BlockCache(const IORequest& inIOReq,
			     const block_t& inBlock,
			     list<IORequest>& outIOReqs)
{
  // See if the block is cached.

  a_block_t aBlock;

  if (cache.isCached(inBlock)) {

    aBlock.blockAddr = cache.blockGet(inBlock);

    switch (inIOReq.opGet()) {
    case IORequest::Demote:
      demoteHitsPerOrig[inIOReq.origGet()]++;
      demoteHits++;
      break;
    case IORequest::Read:
      readHitsPerOrig[inIOReq.origGet()]++;
      readHits++;
      break;
    default:
    	printf(" StoreCacheLLarge.cc error");
      abort();
    }

  }
  else {


    // Eject the front block if the cache is full.

    if (cache.isFull()) {
      cache.blockGetAtHead(aBlock); 
    }
    else
    {
      aBlock
.blockAddr = cache.numGet() + 1;
    }

    switch (inIOReq.opGet()) {
    case IORequest::Demote:
      demoteMissesPerOrig[inIOReq.origGet()]++;
      demoteMisses++;
      break;
    case IORequest::Read:
      readMissesPerOrig[inIOReq.origGet()]++;
      readMisses++;
      break;
    default:
    	printf(" StoreCacheLLarge.cc error 2");
      abort();
    }
  }

    if (nextStore) {

    nextStore->IORequestDown(IORequest(inIOReq.origGet(),
				      IORequest::Read,
				      inIOReq.timeIssuedGet(),
				      aBlock.blockAddr,
				      0,
				      blockSize));

    }


    aBlock.blockID = inBlock.blockID;
    aBlock.objID = inBlock.objID;


    cache.blockPutAtTail(aBlock);

}

void
StoreCacheLarge::statisticsShow() const
{
  printf("{StoreCacheLarge.%s\n", nameGet());

  if (mode != Analyze)
  {
       printf("\t{size {total=%llu} }\n", cache.sizeGet() * blockSizeGet());
  }
  StoreCache::statisticsShow();
  if (mode != Analyze)
  {
    printf("}\n");
  }
}
 
bool
StoreCacheLarge::isCached(const Block::block_t& inBlock)
{
  return cache.isCached(inBlock);
}
