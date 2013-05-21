/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCacheSimple.cc,v 1.5 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include "IORequest.hh"
#include "writeArrayPolicy.hh"
#include "diskActivity.hh"
#include "Block.hh"


using Block::block_t;
using DiskActivity::diskActivityHistory_t;
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
	diskActivityHistory_t inDiskActivity;
	diskActivityHistory_t outLastDiskActivity;

	double const inIOReqDuration = inIOReq.lenGet()/diskAct.access_speedGet();

	inDiskActivity.objID = inIOReq.objIDGet();
	inDiskActivity.time = inIOReq.timeIssuedGet();
	inDiskActivity.status = 'A';
	inDiskActivity.duration = inIOReqDuration;
	//1. check disk last status
	outLastDiskActivity = diskAct.checkLastDiskActivity(inDiskActivity.objID);



	switch (inIOReq.opGet()) {
	    case IORequest::Read:
	    	diskAct.writeDiskWithSpinDown(inDiskActivity);
	    	diskActWithoutSpindown.writeDiskWithoutSpinDown(inDiskActivity);
	    break;
	    case IORequest::Write:

	    	//1. check the disk status
	    	//2. if active then write
	    	//3. if not active then write cache
	    	//4. if cache is full then write cache
	    	 if (cache.isFull()) {
	    		 diskAct.writeDiskWithSpinDown(inDiskActivity);
	    		 diskActWithoutSpindown.writeDiskWithoutSpinDown(inDiskActivity);

	    			 block_t cacheBlock;
	    			 diskActivityHistory_t cacheDisActivity;
	    			 int writeCacheCount = 0;
	    			 while(!cache.isEmpty())
	    			 {
	    				 cache.blockGetAtHead(cacheBlock);
	    				 cacheDisActivity.objID=cacheBlock.objID;
	    				 cacheDisActivity.status = 'A';
	    				 cacheDisActivity.duration=cacheBlock.blockID*blockSize/diskAct.access_speedGet();
	    				 outLastDiskActivity = diskAct.checkLastDiskActivity(cacheBlock.blockID);
	    				 if (outLastDiskActivity.time+outLastDiskActivity.duration < inDiskActivity.time )
	    				 {
	    					 cacheDisActivity.time=inDiskActivity.time;
	    					 diskAct.writeDiskWithSpinDown(cacheDisActivity);
	    					 diskActWithoutSpindown.writeDiskWithoutSpinDown(cacheDisActivity);
	    				 }
	    				 else
	    				 {
	    					 cacheDisActivity.time = outLastDiskActivity.time+outLastDiskActivity.duration;
	    					 diskAct.putDiskActivityHistory(cacheDisActivity);
	    					 diskActWithoutSpindown.putDiskActivityHistory(cacheDisActivity);
	    				 }

	    			 }
	    	 }
	    	else
	    	{
	    		cache.blockPutAtHead(inBlock);
	    	}

	    	break;
	    default:
	    	break;
	}
}
