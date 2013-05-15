#ifndef _MRRRESORTCache_HH_
#define _MRRRESORTCache_HH_

using namespace std;

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <assert.h>
#include <map>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "MemoryRegion.hh"
#include "MRRCache.hh"


class MRRResortCache : public MRRCache {

private:
  // Copy constructors - declared private and never defined

  MRRResortCache(const MRRResortCache&);
  MRRResortCache& operator=(const MRRResortCache&);

protected:

  virtual void EvictCache(uint64_t regsize);

public:

  MRRResortCache(uint64_t inCacheSize) :
    MRRCache(inCacheSize)
    { ; };


  ~MRRResortCache() { ; };


};

#endif /* _MRRRESORTCache_HH_ */
