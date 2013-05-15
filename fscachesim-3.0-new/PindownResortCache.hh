#ifndef _PINDDOWNRESORTCACHE_HH_
#define _PINDDOWNRESORTCACHE_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "MemoryRegion.hh"
#include "PindownCache.hh"


class PindownResortCache : public PindownCache {


private:
  // Copy constructors - declared private and never defined

  PindownResortCache(const PindownResortCache&);
  PindownResortCache& operator=(const PindownResortCache&);

protected:

  virtual void EvictCache(uint64_t regsize); 

public:

  PindownResortCache(uint64_t inCacheSize) :
    PindownCache(inCacheSize)
    { ; };


  ~PindownResortCache() { ; };


};

#endif /* _PINDDOWNRESORTCACHE_HH_ */
