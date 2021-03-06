

#ifndef _STORECACHELFUCOOP_HH_
#define _STORECACHELFUCOOP_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h> 
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "IORequest.hh"
#include "StoreCacheSimpleCoop.hh"
#include "LFUCache.hh"



class StoreCacheLFUCoop : public StoreCacheSimpleCoop {
protected:
  LFUCache cache;

private:
  // Copy constructors - declared private and never defined

  StoreCacheLFUCoop(const StoreCacheLFUCoop&);
  StoreCacheLFUCoop& operator=(const StoreCacheLFUCoop&);

protected:
  virtual bool ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);
  virtual void DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);  



public:
  // Constructors and destructors

  /**
   * Create a LFU COOP cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device, if any.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

  StoreCacheLFUCoop(const char *inName,
		   Store *inNextStore,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
    StoreCacheSimpleCoop(inName, inNextStore, inBlockSize),
    cache(inSize)  { ; };

  /**
   * Create a simple block cache.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the cache, in blocks.

   */
  StoreCacheLFUCoop(const char *inName,
		   uint64_t inBlockSize,
		   uint64_t inSize) :
   StoreCacheSimpleCoop(inName, inBlockSize, inSize),
   cache(inSize) { ; };

  /**
   * Destroy the cache.
   */
  ~StoreCacheLFUCoop() { ; };

  // Statistics management

  virtual void statisticsShow() const;
  virtual void beforeShow();
};

#endif /* _STORECACHELFUCOOP_HH_ */
