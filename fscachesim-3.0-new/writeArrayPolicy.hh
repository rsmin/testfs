

#ifndef _WRITEARRAYPOLICY_HH_
#define _WRITEARRAYPOLICY_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "IORequest.hh"
#include "Cache.hh"
#include "StoreCache.hh"
#include "diskActivity.hh"

//using DiskActivity::diskActivityHistory_t;
class writeArrayPolicy : public StoreCache {
private:
  Cache cache;
  diskActivity diskAct;
  diskActivity diskActWithoutSpindown;
  diskActivityHistory_t inDiskActivity;
  diskActivityHistory_t outLastDiskActivity;
  map<uint64_t, double> energyComsuptionDisk;
  double overallenergyConsumption;
  // Copy constructors - declared private and never defined

private:
  writeArrayPolicy(const writeArrayPolicy&);
  writeArrayPolicy& operator=(const writeArrayPolicy&);

protected:

  virtual void BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock,
			  list<IORequest>& outIOReqs);

public:
  // Constructors and destructors

  /**
   * Create a cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device, if any.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

   */
  writeArrayPolicy(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
   StoreCache(inName, inNextStore,inBlockSize, inSize),
   cache(inSize),
   diskActWithoutSpindown(),
   diskAct(){ ; };

  writeArrayPolicy(const char *inName,
  		   uint64_t inBlockSize,
  		   uint64_t inSize) :
     StoreCache(inName,inBlockSize, inSize),
     cache(inSize),
     diskActWithoutSpindown(),
     diskAct(){ ; };
 //    inDiskActivity(),
  //   outLastDiskActivity()

void cacheCleanPolicy();
  /**
   * Destroy the cache.
   */
  ~writeArrayPolicy() { ; };

  // Statistics management

 void energyCal();
 virtual void statisticsShow() const;
 virtual void beforeShow();
 void diskOperationEnd();
};

#endif /* _WRITEARRAYPOLICY_HH_ */
