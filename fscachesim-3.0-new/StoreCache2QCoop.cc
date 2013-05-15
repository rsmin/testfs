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
#include "LocalBlock.hh"
#include "IORequest.hh"

#include "StoreCache2QCoop.hh"

using Block::block_t;
using LocalBlock::local_block_t;

int
StoreCache2QCoop::blockGetCascade(const block_t& inBlock,IORequest::IORequestOp_t op)
{
  int retval = -1;
  int j;

  if (sq)
   j = (op == IORequest::Read ? 0:1);
  else
   j = 0;
  
  for (int i = 0; i < 3; i++) {
   if (Q[j][i])
   {
    if (Q[j][i]->isCached(inBlock)) {
      Q[j][i]->blockGet(inBlock);
      retval = i;
      break;
    }
   }
  }

  return (retval);
}

void
StoreCache2QCoop::blockPutAtSegCascade(const block_t& inBlock,
				    int inSeg, IORequest::IORequestOp_t op)
{

    block_t ejectBlock;
    int j;

  if (sq)
   j = (op == IORequest::Read ? 0:1);
  else
   j = 0;

    if (Q[j][inSeg]&&(Q[j][inSeg]->isFull()))
    {
      Q[j][inSeg]->blockGetAtHead(ejectBlock);
    
      if (inSeg == 1)
      {
         if (Q[j][0]&&(Q[j][0]->isFull()) )
         {
             block_t ejectBlock0;
             Q[j][0]->blockGetAtHead(ejectBlock0); 
         }
         Q[j][0]->blockPutAtTail(ejectBlock);     
      }
    }
    Q[j][inSeg]->blockPutAtTail(inBlock);
}


StoreCache2QCoop::StoreCache2QCoop(const char *inName,
			     uint64_t inBlockSize,
			     uint64_t inSize,
                             DemotePolicy_t inDemotePolicy) :
  StoreCacheSimpleCoop(inName, inBlockSize, inSize),
  demotePolicy(inDemotePolicy) 
{

    Q[0][2] = new Cache((uint64_t)(3*inSize/4));
    Q[0][1] = new Cache((uint64_t)(1*inSize/4));
    Q[0][0] = new Cache((uint64_t)inSize/2);

    Q[1][2] = NULL;
    Q[1][1] = NULL;
    Q[1][0] = NULL;

    bool sq=false;
}

StoreCache2QCoop::~StoreCache2QCoop()
{
  for (int j = 0; j < 2; j++)
  for (int i = 0; i < 3; i++) {
    if (Q[j][i])
    {
       delete Q[j][i];
       Q[j][i] = NULL;
    }
  }
}

bool
StoreCache2QCoop::ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{
  bool ret = false;

  const char* reqOrig = inIOReq.origGet();
  IORequest::IORequestOp_t op = inIOReq.opGet();

  // See if we have cached this block. We only update the ghost hit counts
  // when we receive a read request.

  int blockSeg = blockGetCascade(inBlock, op);
  if (blockSeg > 0) {
      readHitsPerOrig[reqOrig]++;
      readHits++;

      if (blockSeg == 1)
         blockSeg = 2;
      blockPutAtSegCascade(inBlock, blockSeg,op);
      ret = true;
  }
  else {

      readMissesPerOrig[reqOrig]++;
      readMisses++;

      if (blockSeg == 0)
         blockSeg = 2;
      else
         blockSeg = 1;

      blockPutAtSegCascade(inBlock, blockSeg,op);
  }

  return ret;
}

void
StoreCache2QCoop::DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock)
{

  const char* reqOrig = inIOReq.origGet();
  IORequest::IORequestOp_t op = inIOReq.opGet();

  // See if we have cached this block. We only update the ghost hit counts
  // when we receive a read request.

  int blockSeg = blockGetCascade(inBlock, op);
  if (blockSeg > 0) {
      demoteHitsPerOrig[reqOrig]++;
      demoteHits++;

      if (blockSeg == 1)
         blockSeg = 2;
      blockPutAtSegCascade(inBlock, blockSeg, op);
  }
  else {


      demoteMissesPerOrig[reqOrig]++;
      demoteMisses++;
 

     if (!((demotePolicy == Demand_without_Coop)&&(cooperative.isCached(inBlock))))
     {
        blockPutAtSegCascade(inBlock, 1, op); 
     }

  }
}



void
StoreCache2QCoop::statisticsShow() const
{
  printf("{StoreCache2QCoop.%s\n", nameGet());

if (mode != Analyze)
{
  uint64_t size = 0;
 
  for (int j = 0; j < 2; j++)
  for (int i = 1; i < 3; i++) {
   if (Q[j][i])
     size += Q[j][i]->sizeGet();
  }

  printf("\t{size {total=%llu} }\n", size * blockSizeGet());


  printf("\t{demotePolicy=%s}\n", (demotePolicy == Demand ? "Demote anyway" : "Demote only if no cooperative"));
}
  StoreCache::statisticsShow();
if (mode != Analyze)
{
  printf("}\n");
}
}
