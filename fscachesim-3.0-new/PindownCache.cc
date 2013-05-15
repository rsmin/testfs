#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "MemoryRegion.hh"


#include "PindownCache.hh"


void PindownCache::CalcReg(RegCacheIndex::iterator& regionIter, 
                       const MemoryRegion::memory_region_t& inRegion, 
                       list<MemoryRegion::memory_region_t>& mr, 
                       uint64_t& regsize)
{
   bool hit = false;
   regsize = 0;

   if (regionIter == cacheIndex.end())
   {
     if (cacheIndex.end() != cacheIndex.begin())
     {
       regionIter--;
       if ((inRegion.objID == regionIter->second->objID)&&
        (regionIter->second->blockID + regionIter->second->len >= inRegion.blockID + inRegion.len))
              hit = true;
     }
   }
   else
   {
      if ((inRegion.objID == regionIter->second->objID)&&
             (inRegion.blockID == regionIter->second->blockID))
         hit = true;
      else if (regionIter != cacheIndex.begin())
      {
        regionIter--;
        if ((inRegion.objID == regionIter->second->objID)&&
        (regionIter->second->blockID + regionIter->second->len >= inRegion.blockID + inRegion.len))
              hit = true;
      }
   }


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

void PindownCache::EvictCache(uint64_t regsize) 
{
  evictOne(regsize);
} 
