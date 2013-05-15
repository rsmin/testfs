
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

#include "2QCache.hh"

using Block::block_t;

int
TQCache::blockGetCascade(const Block::block_t& inBlock)
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
TQCache::blockPut(const Block::block_t& inBlock, int inSeg)
{

    block_t ejectBlock;

    assert((inSeg < 0)||(inSeg > 2));

    if (Q[inSeg]->isFull())
    {
      Q[inSeg]->blockGetAtHead(ejectBlock);
    
      if (inSeg == 1)
      {
         if (Q[0]->isFull()) 
         {
             block_t ejectBlock0;
             Q[0]->blockGetAtHead(ejectBlock0); 
         }
         Q[0]->blockPutAtTail(ejectBlock);     
      }
    }

    Q[inSeg]->blockPutAtTail(inBlock);
}


TQCache::TQCache(uint64_t inSize): 
size(inSize)
{

    Q[2] = new Cache((uint64_t)(1*inSize/5));
    Q[1] = new Cache((uint64_t)(4*inSize/5)); 
    Q[0] = new Cache((uint64_t)inSize/2);

}

TQCache::~TQCache()
{

  for (int i = 0; i < 3; i++) {
    if (Q[i])
    {
       delete Q[i];
       Q[i] = NULL;
    }
  }
}

bool
TQCache::blockGet(const Block::block_t& inBlock)
{
  bool ret = false;
  int blockSeg = blockGetCascade(inBlock);

  if (blockSeg > 0) {
      if (blockSeg == 1)
         blockSeg = 2;
      blockPut(inBlock, blockSeg);
      ret = true;
  }
  else {

      if (blockSeg == 0)
         blockSeg = 2;
      else
         blockSeg = 1;

      blockPut(inBlock, blockSeg); 
  }

  return ret;
}





