
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

#include "LocalBlock.hh"
#include "IORequest.hh"

#include "4QCache.hh"

using Block::block_t;
using LocalBlock::local_block_t;

int 
FQCache::blockGetCascade(LocalBlock::local_block_t& inBlock)
{
  int retval = -1;

  
  for (int i = 0; i < 3; i++) {
   if (Q[i])
   {
    if (Q[i]->isCached(inBlock)) {
      Q[i]->blockGet(inBlock);
      retval = i;
      break;
    }
   }
  }

  return (retval);
}



void
FQCache::blockPut(LocalBlock::local_block_t& inBlock, int inSeg)
{

    LocalBlock::local_block_t ejectBlock;

    if (inSeg < 0);
         return;

    if (Q[inSeg]->isFull())
    {
         Q[inSeg]->blockGetAtHead(ejectBlock);
         if (inSeg < 3)
         {
            if (inSeg > 1)
               ejectBlock.refNum = ejectBlock.refNum/2;
            blockPut(ejectBlock, inSeg-1);  
         }   
    }

    Q[inSeg]->blockPutAtTail(inBlock);
}


FQCache::FQCache(uint64_t inSize): size(inSize)
{
    Q[3] = new BlockBuffer(1*inSize/2);
    Q[2] = new BlockBuffer(1*inSize/4); 
    Q[1] = new BlockBuffer(1*inSize/4);
    Q[0] = new BlockBuffer(inSize/2);
}

FQCache::~FQCache()
{

  for (int i = 0; i < 4; i++) {
    if (Q[i])
    {
       delete Q[i];
       Q[i] = NULL;
    }
  }
}

bool
FQCache::blockGet(LocalBlock::local_block_t& inBlock)
{
  bool ret = false;
  

  int blockSeg = blockGetCascade(inBlock);

  if (blockSeg > 0) {
      if (blockSeg != 3)
      {
         inBlock.refNum++;
         blockSeg = inBlock.refNum/2 + 1;
         if (blockSeg > 3)
            blockSeg = 3;
       }
      blockPut(inBlock, blockSeg);
      ret = true;
  }
  else {

      if (blockSeg == 0)
      {
         inBlock.refNum++;
         blockSeg = 2;
      }
      else 
      {
        inBlock.refNum = 1;
        blockSeg = 1;
      }
      blockPut(inBlock, blockSeg); 
  }

  return ret;
}





