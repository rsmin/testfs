/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/fscachesim.cc,v 1.9 2002/03/14 22:29:12 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <functional>  
#ifdef HAVE_STDINT_H 
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h> 

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include <string.h> 
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include "IORequest.hh"
#include "IORequestGeneratorBatch.hh"
#include "IORequestGeneratorFile.hh"
#include "IORequestGeneratorFileGeneric.hh"
#include "IORequestGeneratorFileMambo.hh"
#include "Store.hh"
#include "StoreCache.hh"
#include "StoreCacheSeg.hh"
#include "StoreCacheSimple.hh"
#include "StoreCache2Q.hh"
#include "StoreCacheLRU2Address.hh"
#include "PindownCache.hh"
#include "PindownBatchDeRegCache.hh"
#include "MRRCache.hh"
#include "MRRResortCache.hh"
#include "MRROptCache.hh"

int main()
{
  MRROptCache cache(2000);
  MemoryRegion::memory_region_t mr;
  
  cache.log = true;

  mr.objID = 3;
  mr.blockID = 200;
  mr.len = 180;
  cache.RegionRef(mr);
  cache.statisticsShow();


  mr.objID = 5;
  mr.blockID = 220;
  mr.len = 220;
  cache.RegionRef(mr);
  cache.statisticsShow();


  mr.objID = 5;
  mr.blockID = 500;
  mr.len = 600;
  cache.RegionRef(mr);
  cache.statisticsShow();


  mr.objID = 5;
  mr.blockID = 1200;
  mr.len = 200;
  cache.RegionRef(mr);
  cache.statisticsShow();


  mr.objID = 6;
  mr.blockID = 400;
  mr.len = 150;
  cache.RegionRef(mr);
  cache.statisticsShow();

  mr.objID = 5;
  mr.blockID = 180;
  mr.len = 1300;
  cache.RegionRef(mr);
  cache.statisticsShow();

  return 0;
}
