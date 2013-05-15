#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "MemoryRegion.hh"


#include "RegSimpleCache.hh"


void RegSimpleCache::CalcReg(RegCacheIndex::iterator& regionIter, 
                       const MemoryRegion::memory_region_t& inRegion, 
                       list<MemoryRegion::memory_region_t>& mr, 
                       uint64_t& regsize)
{
   bool hit = false;
   regsize = 0;

   if (cacheIndex.begin() == cacheIndex.end())
     goto end;
   
   if (regionIter == cacheIndex.end())
      regionIter--;
       
   if ((inRegion.objID == regionIter->second->objID)&&
          (regionIter->second->blockID == inRegion.blockID)&&
          (regionIter->second->len >= inRegion.len))
              hit = true;
   else
   {
  
      if (regionIter != cacheIndex.begin())
      {
          regionIter--;

         if ((inRegion.objID == regionIter->second->objID)&&
             (regionIter->second->blockID == inRegion.blockID)&&
             (regionIter->second->len >= inRegion.len))
           hit = true;
      }
   }
 end:
   if (hit)
   {
      sendtotail(regionIter);
      reghit++;
   }
   else
   {
      mr.push_back(inRegion);
      regmiss++;
      regsize = inRegion.len;
   }
   
}

void RegSimpleCache::EvictCache(uint64_t regsize) 
{
  evictBatch(regsize);
} 
