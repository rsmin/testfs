
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

#include "ARC.hh"

using Block::block_t;

int
ARC::blockGetCascade(const Block::block_t& inBlock)
{
  int retval = -1;

  
  for (int i = 0; i < 4; i++) {
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
ARC::blockPut(const Block::block_t& inBlock, int inSeg)
{

    block_t ejectBlock;

    if ((inSeg != 2)&&(inSeg != 3))
    {
       printf(" block put error : seg = %d\n", inSeg);
       abort();
    }

    if (Q[inSeg]->isFull())
    {
      Q[inSeg]->blockGetAtHead(ejectBlock);

         if (Q[inSeg-2]->isFull()) 
         {
             block_t ejectBlock0;
             Q[inSeg-2]->blockGetAtHead(ejectBlock0); 
         }
         Q[inSeg-2]->blockPutAtTail(ejectBlock);     

    }
    Q[inSeg]->blockPutAtTail(inBlock);
}


ARC::ARC(uint64_t inSize): 
size(inSize)
{

    Q[3] = new Cache((uint64_t)(3*inSize/4));
    Q[2] = new Cache((uint64_t)(1*inSize/4)); 
    Q[1] = new Cache((uint64_t)inSize/2);
    Q[0] = new Cache((uint64_t)inSize/2);

}

ARC::~ARC()
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
ARC::blockGet(const Block::block_t& inBlock)
{
  bool ret = false;
  int blockSeg = blockGetCascade(inBlock);

  if (blockSeg > 1) {
      if (blockSeg == 2)
         blockSeg = 3;
      blockPut(inBlock, blockSeg);
      ret = true;
  }
  else {
    
      
      if (blockSeg >= 0)
      {
         Adaption(blockSeg);
         blockSeg = 3;
      }
      else
         blockSeg = 2;

      blockPut(inBlock, blockSeg); 
  }

  return ret;
}

bool
ARC::Adaption(uint64_t blockSeg)
{

  if ((blockSeg == 0)||(blockSeg == 1))
  {
     uint64_t nSeg;
     int shrinkSize;

     if (blockSeg == 0)
        nSeg = 1;
     else
        nSeg = 0;
        
     if (Q[blockSeg]->numGet() >= Q[nSeg]->numGet())
         shrinkSize = 1;
     else
         shrinkSize = Q[nSeg]->numGet()/Q[blockSeg]->numGet();

     block_t ejectBlock;
     for (int i = 0; i < shrinkSize; i++)
     {
         
          Q[nSeg+2]->blockGetAtHead(ejectBlock);
          Q[nSeg]->blockPutAtTail(ejectBlock);
     }

     Q[nSeg+2]->sizeSet(Q[nSeg+2]->sizeGet()-shrinkSize);
     Q[blockSeg+2]->sizeSet(Q[blockSeg+2]->sizeGet()+shrinkSize);
     
  }
  else
  {
     printf(" adaption error\n");
     abort();
  }
}



