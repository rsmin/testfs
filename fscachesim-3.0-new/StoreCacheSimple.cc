/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.cc,v 1.5 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "StoreCacheSimple.hh"

using Block::block_t;

/**
 * This cache maintains one ejection queue. The head of the queue is the
 * eject-me-next block. Hence, for LRU and MRU we add blocks at the tail
 * and head respectively.
 */



void
StoreCacheSimple::BlockCache(const IORequest& inIOReq,
			     const block_t& inBlock,
			     list<IORequest>& outIOReqs)
{
  // See if the block is cached.
  block_t demoteBlock;

  if (cache.isCached(inBlock)) {

    // Eject the block (we will re-cache it later).

    cache.blockGet(inBlock);

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
      //since write flag is added, if default, then just break rather than abort() xiaodong;
      //abort();
      break;
    }
  }
  else {

    // Eject the front block if the cache is full.

    if (cache.isFull()) {
      

      cache.blockGetAtHead(demoteBlock);

      // If necessary, create a Demote I/O.

      if (nextStore && demotePolicy == Demand) {
/*	outIOReqs.push_back(IORequest(inIOReq.origGet(),
				      IORequest::Demote,
				      inIOReq.timeIssuedGet(),
				      demoteBlock.objID,
				      demoteBlock.blockID * blockSize,
				      blockSize));*/
       nextStore->IORequestDown(IORequest(inIOReq.origGet(),
				      IORequest::Demote,
				      inIOReq.timeIssuedGet(),
				      demoteBlock.objID,
				      demoteBlock.blockID * blockSize,
				      blockSize));

      }
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
/*	outIOReqs.push_back(IORequest(inIOReq.origGet(),
				      IORequest::Read,
				      inIOReq.timeIssuedGet(),
				      inIOReq.objIDGet(),
				      inBlock.blockID * blockSize,
				      blockSize));*/
       nextStore->IORequestDown(IORequest(inIOReq.origGet(),
				      IORequest::Read,
				      inIOReq.timeIssuedGet(),
				      inIOReq.objIDGet(),
				      inBlock.blockID * blockSize,
				      blockSize));

      }
      break;
    default:
    	//abort();
      break;
    }
  }

  switch (ejectPolicy) {
  case LRU:
    cache.blockPutAtTail(inBlock);
    break;
  case MRU:
    if (inIOReq.opGet() == IORequest::Demote) {
      // Demoted blocks always go at the eject-me-last end.

      cache.blockPutAtTail(inBlock);
    }
    else {
      cache.blockPutAtHead(inBlock);
//       cache.blockPutAtHead(demoteBlock);

    }
    break;
  default:
    // Wow - we should not get here!

    abort();
  }
}

void
StoreCacheSimple::statisticsShow() const
{
  printf("{StoreCacheSimple.%s\n", nameGet());

  if (mode != Analyze)
  {
       printf("\t{size {total=%llu} }\n", cache.sizeGet() * blockSizeGet());
       printf("\t{ejectPolicy=%s}\n", (ejectPolicy == LRU ? "LRU" : "MRU"));
       printf("\t{demotePolicy=%s}\n", (demotePolicy == None ? "None" : "Demote"));
  }
  StoreCache::statisticsShow();
  if (mode != Analyze)
  {
    printf("}\n");
  }
}
 
bool
StoreCacheSimple::isCached(const Block::block_t& inBlock)
{
  return cache.isCached(inBlock);
}
