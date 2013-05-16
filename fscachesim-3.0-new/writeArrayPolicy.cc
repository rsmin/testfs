/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.cc,v 1.5 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include "writeCache.hh"
#include "IORequest.hh"
#include "writeArrayPolicy.hh"
#include "diskActivity.hh"

using writeCache::block_t;
using writeCache::diskActivityHistory_t;



/**
 * This cache maintains one ejection queue. The head of the queue is the
 * eject-me-next block. Hence, for LRU and MRU we add blocks at the tail
 * and head respectively.
 */



void writeArrayPolicy::beforeShow()
{
// cache.postShow();
 StoreCache::beforeShow();
}

void
writeArrayPolicy::statisticsShow() const
{
  printf("{StoreCacheLRUCoop.%s\n", nameGet());

  if (mode != Analyze)
     printf("\t{size {total=%llu} }\n", cache.sizeGet() * blockSizeGet());

  StoreCache::statisticsShow();

 if (mode != Analyze)
 {
    printf("}\n");
 }
}

void
writeArrayPolicy::BlockCache(const IORequest& inIOReq,
			     const block_t& inBlock,
			     list<IORequest>& outIOReqs)
{
	diskActivityHistory_t& outLastDiskActivity;
	switch (inIOReq.opGet()) {
	    case IORequest::Read:
	    	//1. check disk last status
	    	checkLastDiskActivity(outLastDiskActivity)
	    	//2. update disk status

	    	//3. add the disk new status

	    	//4. make coresponding write

	    	break;
	    case IORequest::Write:
	    	//1. check the disk status
	    	//2. if active then write
	    	//3. if not active then write cache
	    	//4. if cache is full then write cache


	    	break;
	    default:
	    	break;
}
