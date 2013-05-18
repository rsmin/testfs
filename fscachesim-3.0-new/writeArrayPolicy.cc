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
using diskActivity::diskActivityHistory_t;



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
	diskActivityHistory_t* outLastDiskActivity;

	double const inIOReqDuration = inIOReq.lenGet()/diskAct.access_speedGet();

	inDiskActivity.objID = inIOReq.objIDGet();
	inDiskActivity.time = inIOReq.timeIssuedGet();
	inDiskActivity.status = 'A';
	inDiskActivity.duration = inIOReqDuration;
	//1. check disk last status
	outLastDiskActivity = diskAct.checkLastDiskActivity(inDiskActivity.objID);



	switch (inIOReq.opGet()) {
	    case IORequest::Read:
	    	if(inDiskActivity.time<=(outLastDiskActivity->time+outLastDiskActivity->duration))
	    	{
	    		//set spin wait time before record the activity.
	 			diskAct.setSpinWaitTimeLen(inDiskActivity);
	    		diskActWithoutSpindown.setSpinWaitTimeLen(inDiskActivity);
	    		diskAct.putDiskActivityHistory(inDiskActivity);
	    		diskActWithoutSpindown.putDiskActivityHistory(inDiskActivity);
	    	}
	    	else //set idle time and spindown time length
	    	{
	    		//without spindown mode setup
	    		diskActivityHistory_t onlyIdleDiskActivity;
	    		onlyIdleDiskActivity.objID = outLastDiskActivity->objID;
	    		onlyIdleDiskActivity.time = outLastDiskActivity->time+outLastDiskActivity->duration;
	    		onlyIdleDiskActivity.status='I';
	    		onlyIdleDiskActivity.duration=inDiskActivity.time-onlyIdleDiskActivity.time;
	    		diskActWithoutSpindown.setSpinWaitTimeLen(inDiskActivity);
	    		diskActWithoutSpindown.putDiskActivityHistory(onlyIdleDiskActivity);
	    		diskActWithoutSpindown.putDiskActivityHistory(inDiskActivity);
	    		//spindown mode setup
	    		diskActivityHistory_t defaultIdleDiskActivity, spindownDiskActivity;
	    		defaultIdleDiskActivity = onlyIdleDiskActivity;
	    		if(defaultIdleDiskActivity.duration > diskAct.spinWaitTimeLenGet(defaultIdleDiskActivity.objID))
	    		{
	    			diskAct.setSpinWaitTimeLen(inDiskActivity);
	    			defaultIdleDiskActivity.duration = diskAct.spinWaitTimeLenGet(defaultIdleDiskActivity.objID);
	    			//idle time before spin down
	    			diskAct.putDiskActivityHistory(defaultIdleDiskActivity);
	    			//spin down setting up
	    			spindownDiskActivity.objID = defaultIdleDiskActivity.objID;
	    			spindownDiskActivity.time = defaultIdleDiskActivity.time+defaultIdleDiskActivity.duration;
	    			spindownDiskActivity.status='S';
	    			spindownDiskActivity.duration=inDiskActivity.time-spindownDiskActivity.time;
	    			if (spindownDiskActivity.duration<=0)
	    				spindownDiskActivity.duration = 0;
	    			diskAct.putDiskActivityHistory(spindownDiskActivity);
	    			//record the incoming request activity
	    			diskAct.putDiskActivityHistory(inDiskActivity);
	    		}
	    		else
	    		{
	    			diskAct.setSpinWaitTimeLen(inDiskActivity);
		    		diskAct.putDiskActivityHistory(defaultIdleDiskActivity);
		    		diskAct.putDiskActivityHistory(inDiskActivity);
	    		}
	    	}
	    }
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
}
