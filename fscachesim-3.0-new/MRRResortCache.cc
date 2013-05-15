#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "MemoryRegion.hh"


#include "MRRResortCache.hh"



void MRRResortCache::EvictCache(uint64_t regsize)
{
  evictSL(regsize);
}


