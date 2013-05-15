#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "MemoryRegion.hh"


#include "PindownBatchDeRegCache.hh"



void PindownBatchDeRegCache::EvictCache(uint64_t regsize)
{
  evictBatch(regsize);
}
