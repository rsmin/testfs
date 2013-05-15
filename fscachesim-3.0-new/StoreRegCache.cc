/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCache.cc,v 1.4 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/
 
#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */ 
#include <stdio.h>
#include <stdlib.h>


#include "IORequest.hh"
#include "Store.hh"
#include "StoreRegCache.hh" 

StoreRegCache::StoreRegCache(const char *inName,
	     uint64_t inBlockSize,
             uint64_t inSize) :
    Store(inName, inBlockSize), size(inSize), totaltime(0)
{ 
     if (!strcmp(inName,"MRR"))
        cache = new MRRCache(inSize); 
     else if (!strcmp(inName,"MRRResort"))
        cache = new MRRResortCache(inSize);
     else if (!strcmp(inName,"MRROpt"))
        cache = new MRROptCache(inSize);
     else if (!strcmp(inName,"Pindown"))
        cache = new PindownCache(inSize);
     else if (!strcmp(inName,"PindownBatchDeReg"))
        cache = new PindownBatchDeRegCache(inSize);
     else if (!strcmp(inName,"PindownResort"))
        cache = new PindownResortCache(inSize);
     else if (!strcmp(inName,"RegSimple"))
        cache = new RegSimpleCache(inSize);
     else if (!strcmp(inName,"NoCache"))
       cache = NULL;
     else
     {
         printf("There is no cache name of %s\n", inName);
         exit(-1);
     }
}


bool
StoreRegCache::IORequestDown(const IORequest& inIOReq)
{
  MemoryRegion::memory_region_t mr = {inIOReq.objIDGet(), inIOReq.blockOffGet(blockSize),  \
                                inIOReq.blockLenGet(blockSize),0};

 if (cache)
    cache->RegionRef(mr);
 else
 {
   totaltime += mr.len*(0.22+0.77)+7.42+1.1;
 }
  return (true);
}


void
StoreRegCache::statisticsShow() const
{
  
  printf("{StoreRegCache.%s\n", nameGet());
  if (cache)
     cache->statisticsShow();
  else
  {
     printf("totaltime = %lf\n", totaltime);
  }
  printf("}\n");
}



