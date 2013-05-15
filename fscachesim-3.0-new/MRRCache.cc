#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "MemoryRegion.hh" 


#include "MRRCache.hh"


void MRRCache::CalcReg(RegCacheIndex::iterator& regionIter, 
                       const MemoryRegion::memory_region_t& inRegion, 
                       list<MemoryRegion::memory_region_t>& mr, 
                       uint64_t& regsize)
{
   int hit = -1;
   regsize = 0;

   MemoryRegion::memory_region_t newRegion;

   newRegion.objID = inRegion.objID;
   newRegion.blockID = inRegion.blockID;
   newRegion.len = inRegion.len;
   newRegion.H = 0;
 
   if (regionIter == cacheIndex.end())
   {
        if (cacheIndex.end() == cacheIndex.begin())
           goto end;

        regionIter--;
        if (inRegion.objID != regionIter->second->objID)
           goto end;

        if (regionIter->second->blockID + regionIter->second->len >= \
           newRegion.blockID + newRegion.len)
               hit = 1;
        else if (regionIter->second->blockID + regionIter->second->len > \
            newRegion.blockID)
        {
               sendtotail(regionIter);
               newRegion.blockID = regionIter->second->blockID + regionIter->second->len;
               newRegion.len -= newRegion.blockID - inRegion.blockID;
               hit = 0;
        }
        goto end;
   }

   if ((newRegion.objID == regionIter->second->objID)&&
             (newRegion.blockID == regionIter->second->blockID))
   {
         hit = 1;
         goto end;
   }


   if (regionIter != cacheIndex.begin())
   {
          regionIter--;

          if (inRegion.objID != regionIter->second->objID)
          {
             regionIter++;
             goto next;
          }

           if (regionIter->second->blockID + regionIter->second->len >= \
              newRegion.blockID + newRegion.len)
           {
               hit = 1;
               goto end;
           }

           if (regionIter->second->blockID + regionIter->second->len > \
              newRegion.blockID)
           {
               sendtotail(regionIter);
               newRegion.blockID = regionIter->second->blockID + regionIter->second->len;
               newRegion.len -= (newRegion.blockID - inRegion.blockID);
               hit = 0;
           }
           regionIter++;
   }

next:

   if (newRegion.objID != regionIter->second->objID)
       goto end;

   if (regionIter->second->blockID >= \
              newRegion.blockID + newRegion.len)
       goto end;

   if (regionIter->second->blockID == \
              newRegion.blockID)
   {
       hit = 1; 
       goto end;
   }

   MemoryRegion::memory_region_t regRegion;

   do {

           sendtotail(regionIter);
           hit = 0;
           
           if (regionIter->second->blockID == newRegion.blockID)
           {
              if (newRegion.len <= regionIter->second->len)
              {
                  hit = 2;
                  break;
               }
               else
               {
                  newRegion.blockID = regionIter->second->blockID + regionIter->second->len;
                  newRegion.len -=regionIter->second->len;
                  continue;
               }
           }

           regRegion.H = 0;
           regRegion.objID = newRegion.objID;
           regRegion.blockID = newRegion.blockID;
           regRegion.len = regionIter->second->blockID - regRegion.blockID;
           mr.push_back(regRegion);
           regsize += regRegion.len;
 
           newRegion.len -= regRegion.len;

           if (newRegion.len <= regionIter->second->len)
           {
               hit = 2;
               break;
           }
           newRegion.blockID = regionIter->second->blockID + regionIter->second->len;
           newRegion.len -= regionIter->second->len;

           regionIter++;
   }
   while ((regionIter != cacheIndex.end())&&
         (newRegion.objID == regionIter->second->objID)&&
         (regionIter->second->blockID < newRegion.blockID + newRegion.len));

end:
   if (hit == -1)
   {
      mr.push_back(newRegion);
      regmiss++;
      regsize += newRegion.len;
   }
   else if (hit == 1)
   {
      sendtotail(regionIter);
      reghit++;
   }
   else if (hit == 0 )
   {
     regparthit++;
     mr.push_back(newRegion);
     regsize += newRegion.len; 
   }
   else if (hit == 2  )
   {
     regparthit++;
   }
}


void MRRCache::EvictCache(uint64_t regsize)
{
   evictBatch(regsize);
}
