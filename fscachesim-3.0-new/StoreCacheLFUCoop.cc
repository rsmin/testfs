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
#include "LocalBlock.hh"
#include "FreqBlock.hh"
#include "IORequest.hh" 

#include "StoreCacheLFUCoop.hh"

using Block::block_t;
using FreqBlock::freq_time_t;
using FreqBlock::freq_block_t;



/**
 * This cache maintains one ejection queue. The head of the queue is the
 * eject-me-next block. Hence, for LRU and MRU we add blocks at the tail
 * and head respectively.
 */
bool
StoreCacheLFUCoop::ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{

  bool ret = false;

  // See if the block is cached.

  if (cache.blockGet(inBlock, demotetime)) {
    readHitsPerOrig[inIOReq.origGet()]++;
    readHits++;
    ret = true;
  }
  else {

    readMissesPerOrig[inIOReq.origGet()]++;
    readMisses++;
  }

  return ret;

}

void
StoreCacheLFUCoop::DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{

//  if (cooperative.isCached(inBlock))
//      return;

  demotetime++;

  if (!cache.blockPut(inBlock, demotetime))
  {
    demoteHitsPerOrig[inIOReq.origGet()]++;
    demoteHits++;
    demotetime--;
  }
  else {
      demoteMissesPerOrig[inIOReq.origGet()]++;
      demoteMisses++;
  }
}

void StoreCacheLFUCoop::beforeShow()
{
// cache.postShow();
 StoreCache::beforeShow();
}

void
StoreCacheLFUCoop::statisticsShow() const
{
  printf("{StoreCacheLRUCoop.%s\n", nameGet());

  if (mode != Analyze)
     printf("\t{size {total=%llu} }\n", cache.sizeGet() * blockSizeGet());





  StoreCache::statisticsShow();

 if (mode != Analyze)
 {
    printf("}\n");
 }


}
