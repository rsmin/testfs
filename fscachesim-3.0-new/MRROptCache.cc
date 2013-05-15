using namespace std;

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#define NDEBUG

#include <assert.h>
#include <list>
#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "MemoryRegion.hh"
#include "MRROptCache.hh"


void MRROptCache::CalcReg(RegCacheIndex::iterator& regionIter, 
                       const MemoryRegion::memory_region_t& inRegion, 
                       list<MemoryRegion::memory_region_t>& mr, 
                       uint64_t& regsize)
{
  list<MemoryRegion::memory_region_t> rmlist;
  RegCacheIndex::iterator pIter;
   uint64_t deregsize = 0;

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



   MemoryRegion::memory_region_t tmpMr;

   while ((regionIter != cacheIndex.end())&&(regionIter->second->objID == newRegion.objID)&&
            (regionIter->second->blockID >= newRegion.blockID)&&\
             (regionIter->second->blockID + regionIter->second->len <= newRegion.blockID + \
              newRegion.len ))
  {

        deregsize += regionIter->second->len;
        blockCount -= regionIter->second->len;

         tmpMr = *(regionIter->second);
         rmlist.push_back(tmpMr);

   }
   
   for (list<MemoryRegion::memory_region_t>::iterator i = rmlist.begin();
         i != rmlist.end(); i++)
   {
         pIter = cacheIndex.find(*i);    
         cache.erase(pIter->second); 
         cacheIndex.erase(*i);
   }
   rmlist.clear();

   if (deregsize)     
     totaltime += deregsize*DISREG_PERPAGE + DISREG_PERREG; 

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
}

void MRROptCache::EvictCache(uint64_t regsize)
{
   evictSL(regsize);
}
