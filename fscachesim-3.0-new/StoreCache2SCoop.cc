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

#include "StoreCache2SCoop.hh"

using Block::block_t;


int
StoreCache2SCoop::blockGetCascade(const block_t& inBlock,IORequest::IORequestOp_t op, bool keep)
{
  int retval = -1;

  if (cache.isCached(inBlock))
  {
      retval = 3;
      cache.blockGet(inBlock);
  }
  else 
  {
     if (keep)
     {
       if (TQ.blockGet(inBlock))
         retval = 2;
     }
     else  
        retval = TQ.blockGetCascade(inBlock);
  }
  return (retval);
}

void
StoreCache2SCoop::blockPutAtSegCascade(const block_t& inBlock,
         int inSeg, IORequest::IORequestOp_t op)
{
    assert((inSeg < 0)||(inSeg > 3));

    if (inSeg == 3)
    {
         if (cache.isFull())
         {
             block_t ejectBlock;
             cache.blockGetAtHead(ejectBlock);
         }
         cache.blockPutAtTail(inBlock);
    }
    else
      TQ.blockPut(inBlock,inSeg);
         
}



bool
StoreCache2SCoop::ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{
  bool ret = false;

  const char* reqOrig = inIOReq.origGet();
  IORequest::IORequestOp_t op = inIOReq.opGet();
  

  int blockSeg = blockGetCascade(inBlock, op);
  if (blockSeg > 0) {
      readHitsPerOrig[reqOrig]++;
      readHits++; 

      if (blockSeg == 3)
      {
         blockPutAtSegCascade(inBlock,1,op);          //need to tune;
      }

      ret = true;
  }
  else {

      readMissesPerOrig[reqOrig]++;
      readMisses++;


  }

  return ret;
}

void
StoreCache2SCoop::DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{

  const char* reqOrig = inIOReq.origGet();
  IORequest::IORequestOp_t op = inIOReq.opGet();
  bool upCached = true;

     if (!cooperative.isCached(inBlock))
       upCached = false;
 

  int blockSeg = blockGetCascade(inBlock, op, false);
  if (blockSeg > 0) {
      demoteHitsPerOrig[reqOrig]++;
      demoteHits++;

      assert(blockSeg == 3);

       if (!upCached)
       {
           blockPutAtSegCascade(inBlock, 3, op);  
       }
       else
           blockPutAtSegCascade(inBlock, blockSeg, op);

  }
  else {
       
     demoteMissesPerOrig[reqOrig]++;
     demoteMisses++;
 
     if (!upCached)
     {
        blockPutAtSegCascade(inBlock, 3, op); 
     }
     else
        blockPutAtSegCascade(inBlock, 2, op);  // need to tune ------


  }
}



void
StoreCache2SCoop::statisticsShow() const
{
  printf("{StoreCache2SCoop.%s\n", nameGet());

if (mode != Analyze)
{
  uint64_t size;
  size = cache.sizeGet()+TQ.sizeGet(); 
 
  printf("\t{size {total=%llu} }\n", size * blockSizeGet());


}
  StoreCache::statisticsShow();
if (mode != Analyze)
{
  printf("}\n");
}
}
