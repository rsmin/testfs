#ifndef _2QCACHE_HH_
#define _2QCACHE_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif 

#include "Block.hh"
#include "Cache.hh"
#include "LocalBlock.hh"
#include "BlockBuffer.hh"

class TQCache {

private:
   Cache *Q[3];
   uint64_t size;

public:
   TQCache(uint64_t inSize);

   int blockGetCascade(const Block::block_t& inBlock);
   void blockPut(const Block::block_t& inBlock, int inSeg);

   bool blockGet(const Block::block_t& inBlock);

   uint64_t sizeGet() const {return size;};

  ~TQCache(); 

};


#endif /* _2QCACHE_HH_ */
