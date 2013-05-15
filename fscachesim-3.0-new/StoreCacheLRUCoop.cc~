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
#include "IORequest.hh"

#include "StoreCacheLRUCoop.hh"

using Block::block_t;
using LocalBlock::local_block_t;



/**
 * This cache maintains one ejection queue. The head of the queue is the
 * eject-me-next block. Hence, for LRU and MRU we add blocks at the tail
 * and head respectively.
 */
bool
StoreCacheLRUCoop::ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{

  bool ret = false;
  Block::block_t lastBlock;
  bool haslastBlock = false;

  // See if the block is cached.

  if (cache.isCached(inBlock)) {


    cache.blockGet(inBlock);
    readHitsPerOrig[inIOReq.origGet()]++;
    readHits++;
    ret = true;
  }
  else {

    // Eject the front block if the cache is full.

    if (cache.isFull()) { 
      cache.blockGetAtHead(lastBlock); 
      haslastBlock = true;

    }
      readMissesPerOrig[inIOReq.origGet()]++;
      readMisses++;
  }
  PutReadBlock(inBlock, haslastBlock, lastBlock);
  return ret;

}

void
StoreCacheLRUCoop::DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{

  Block::block_t lastBlock;
  bool haslastBlock = false;

  if (cache.isCached(inBlock)) {

    // Eject the block (we will re-cache it later).
    if (demotePolicy != None)
    {
           cache.blockGet(inBlock);
           cache.blockPutAtTail(inBlock); 
    }

    demoteHitsPerOrig[inIOReq.origGet()]++;
    demoteHits++;
  }
  else {

    // Eject the front block if the cache is full. 

    if (cache.isFull()) {

          cache.blockGetAtHead(lastBlock);
          haslastBlock = true;
    }

      demoteMissesPerOrig[inIOReq.origGet()]++;
      demoteMisses++;
      PutDemoteBlock(inBlock, haslastBlock, lastBlock);
  }


 }

void
StoreCacheLRUCoop::PutReadBlock(const Block::block_t& inBlock,bool haslastBlock,
                            const Block::block_t& lastBlock) 
{
   if (demotePolicy == None) 
      cache.blockPutAtTail(inBlock); 
   else
   {
      if ((demotePolicy == Demand_NoCacheRead)||(demotePolicy == Demand_NoCacheRead_without_Coop))
      {
          if (haslastBlock)
              cache.blockPutAtHead(lastBlock);
      }
      else
          cache.blockPutAtHead(inBlock); 
   }
}

void
StoreCacheLRUCoop::PutDemoteBlock(const Block::block_t& inBlock,bool haslastBlock,
                            const Block::block_t& lastBlock) 
{
  if (demotePolicy != None) 
  {

      if ((demotePolicy == Demand_without_Coop)||(demotePolicy == Demand_NoCacheRead_without_Coop))
      {

            if (cooperative.isCached(inBlock))
            {
               if (haslastBlock)
                   cache.blockPutAtHead(lastBlock);
            }
            else 
               cache.blockPutAtTail(inBlock);
       }
       else
       {
           cache.blockPutAtTail(inBlock);
       }
  }
  else 
  {
        if (haslastBlock)
            cache.blockPutAtHead(lastBlock);
  }
}

void
StoreCacheLRUCoop::statisticsShow() const
{
  printf("{StoreCacheLRUCoop.%s\n", nameGet());

if (mode != Analyze)
{

  printf("\t{size {total=%llu} }\n", cache.sizeGet() * blockSizeGet());


  if (demotePolicy != None)
      printf("\t{demotePolicy=%s}\n", (demotePolicy == Demand ? "Demote anyway" : "Demote only if no cooperative"));
}


  StoreCache::statisticsShow();

if (mode != Analyze)
{
  printf("}\n");
}
}
