/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.hh,v 1.4 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHE4SCOOP_HH_
#define _STORECACHE4SCOOP_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

#include "Block.hh"
#include "Cache.hh"
#include "IORequest.hh"
#include "4QCache.hh"
#include "StoreCacheSimpleCoop.hh"

class StoreCache4SCoop : public StoreCacheSimpleCoop {

private:

  BlockBuffer cache;
  FQCache FQ;



private:
  // Copy constructors - declared private and never defined

  StoreCache4SCoop(const StoreCache4SCoop&);
  StoreCache4SCoop& operator=(const StoreCache4SCoop&);

  int blockGetCascade(LocalBlock::local_block_t& inBlock, 
       IORequest::IORequestOp_t op, bool keep = true);
  void blockPutAtSegCascade(LocalBlock::local_block_t& inBlock,
			    int inSeg, IORequest::IORequestOp_t op);

protected:
  virtual bool ReadFromCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);
  virtual void DemoteToCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock);  


public:
  /**
   * Create a segmented adaptive cache with uniform size segments.
   *
   * @param inName A string name for the cache.
   * @param inBlockSize The size of each block, in bytes.
   * @param inSize The size of the total cache, in blocks.

   */
  StoreCache4SCoop(const char *inName,
		uint64_t inBlockSize,
		uint64_t inSize):
  StoreCacheSimpleCoop(inName, inBlockSize, inSize),
  cache(9*inSize/10),
  FQ(inSize/10) {;};



  /**
   * Destroy the cache.
   */
  ~StoreCache4SCoop() { ;} ;  

  // Statistics management


  virtual void statisticsShow() const;
};

#endif /* _STORECACHE4SCOOP_HH_ */
