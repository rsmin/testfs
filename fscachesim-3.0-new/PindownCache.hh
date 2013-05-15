#ifndef _PINDDOWNCACHE_HH_
#define _PINDDOWNCACHE_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "MemoryRegion.hh"
#include "RegCache.hh"


class PindownCache : public RegCache {


private:
  // Copy constructors - declared private and never defined

  PindownCache(const PindownCache&);
  PindownCache& operator=(const PindownCache&);

protected:
  virtual void CalcReg(RegCacheIndex::iterator& regionIter, 
                       const MemoryRegion::memory_region_t& inRegion, 
                       list<MemoryRegion::memory_region_t>& mr, 
                       uint64_t& regsize);

  virtual void EvictCache(uint64_t regsize); 
  
public:

  PindownCache(uint64_t inCacheSize) :
    RegCache(inCacheSize)
    { ; };


  ~PindownCache() { ; };


};

#endif /* _PINDDOWNCACHE_HH_ */
