#include <map>
#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include "diskActivity.hh"
#include "Block.hh"
#include <math.h>


//using DiskActivity::diskActivityHistory_t;

diskActivityHistory_t diskActivity::checkLastDiskActivity(uint64_t inObjID)
{
	diskActivityHistory_t outLastDiskAct;
	list<diskActivityHistory_t>::iterator iter = diskActivityHistory.begin();

	while(iter->objID != inObjID && iter != diskActivityHistory.end())
	{
		iter++;
	}
	if (iter->objID == inObjID)
	{
		outLastDiskAct.objID = iter->objID ;
		outLastDiskAct.status = iter->status;
		outLastDiskAct.time=iter->time;
		outLastDiskAct.duration=iter->duration;
	}
	else{
		outLastDiskAct.objID=inObjID;
		outLastDiskAct.status = 'A';
		outLastDiskAct.time=0;
		outLastDiskAct.duration=0;
	}
	return outLastDiskAct;
};

double diskActivity::spinWaitTimeLenGet(uint64_t inObjID){
	map<uint64_t,double>::iterator iter= spinWaitIndex.find(inObjID);
	if(iter == spinWaitIndex.end()) {
		return -1;
		}
	else {
		return iter->second;
		}
}
void diskActivity::setSpinWaitTimeLen(diskActivityHistory_t inDiskActivity)
{
	uint64_t inObjID = inDiskActivity.objID;
	diskActivityHistory_t lastActivity = checkLastDiskActivity(inObjID);

	double WaitTimeDuration = inDiskActivity.time - lastActivity.time - lastActivity.duration;

	map<uint64_t,double>::iterator iter= spinWaitIndex.find(inObjID);
	if (WaitTimeDuration >= spinTimeLen_min)
	{
		if(iter == spinWaitIndex.end()) {
			spinWaitIndex[inObjID] =
					initSpintWaitTimeLen+fmod(WaitTimeDuration,spinTimeLen_min)*spinWaitTimeDelta;
		}
		else{
			spinWaitIndex[inObjID] =
					spinWaitIndex[inObjID]+fmod(WaitTimeDuration,spinTimeLen_min)*spinWaitTimeDelta;
		}
	}
	else
	{
		if(iter == spinWaitIndex.end()) {
			spinWaitIndex[inObjID] =
					initSpintWaitTimeLen-fmod(spinTimeLen_min,WaitTimeDuration)*spinWaitTimeDelta;
		}
		else{
			spinWaitIndex[inObjID] =
					spinWaitIndex[inObjID]-fmod(spinTimeLen_min,WaitTimeDuration)*spinWaitTimeDelta;
		}
	}

	if (spinWaitIndex[inObjID] > spinTimeLen_min)
		spinWaitIndex[inObjID] = spinTimeLen_min;
	if (spinWaitIndex[inObjID] < 0)
		spinWaitIndex[inObjID] = 0;


}

void diskActivity::writeDiskWithoutSpinDown(diskActivityHistory_t inDiskActivity){
	diskActivityHistory_t outLastDiskAct;
	outLastDiskAct = checkLastDiskActivity(inDiskActivity.objID);

		if(inDiskActivity.time<=(outLastDiskAct.time+outLastDiskAct.duration))
		{
			//set spin wait time before record the activity.
			inDiskActivity.time = outLastDiskAct.time+outLastDiskAct.duration;
			setSpinWaitTimeLen(inDiskActivity);
			putDiskActivityHistory(inDiskActivity);

		}
		else //set idle time and spindown time length
		{
			//without spindown mode setup
			diskActivityHistory_t onlyIdleDiskActivity;
			onlyIdleDiskActivity.objID = outLastDiskAct.objID;
			onlyIdleDiskActivity.time = outLastDiskAct.time+outLastDiskAct.duration;
			onlyIdleDiskActivity.status='I';
			onlyIdleDiskActivity.duration=inDiskActivity.time-onlyIdleDiskActivity.time;
			setSpinWaitTimeLen(inDiskActivity);
			putDiskActivityHistory(onlyIdleDiskActivity);
			putDiskActivityHistory(inDiskActivity);
		}

}

void diskActivity::writeDiskWithSpinDown(diskActivityHistory_t inDiskActivity){
	diskActivityHistory_t outLastDiskAct;
	outLastDiskAct = checkLastDiskActivity(inDiskActivity.objID);

	diskActivityHistory_t defaultIdleDiskActivity, spindownDiskActivity;

	if (inDiskActivity.time<=(outLastDiskAct.time+outLastDiskAct.duration)){
		//set spin wait time before record the activity.
		inDiskActivity.time = outLastDiskAct.time+outLastDiskAct.duration;
		setSpinWaitTimeLen(inDiskActivity);
		putDiskActivityHistory(inDiskActivity);
	}

	else if (inDiskActivity.time<=outLastDiskAct.time+outLastDiskAct.duration+spinWaitTimeLenGet(inDiskActivity.objID))
	{
		defaultIdleDiskActivity.objID = outLastDiskAct.objID;
		defaultIdleDiskActivity.time = outLastDiskAct.time+outLastDiskAct.duration;
		defaultIdleDiskActivity.status ='I';
		defaultIdleDiskActivity.duration = inDiskActivity.time-defaultIdleDiskActivity.time;
		setSpinWaitTimeLen(inDiskActivity);
		putDiskActivityHistory(defaultIdleDiskActivity);
		putDiskActivityHistory(inDiskActivity);
	}
	else
	{
		setSpinWaitTimeLen(inDiskActivity);
		defaultIdleDiskActivity.objID = outLastDiskAct.objID;
		defaultIdleDiskActivity.time = outLastDiskAct.time+outLastDiskAct.duration;
		defaultIdleDiskActivity.status ='I';
		defaultIdleDiskActivity.duration = spinWaitTimeLenGet(inDiskActivity.objID);

		spindownDiskActivity.objID = inDiskActivity.objID;
		spindownDiskActivity.time = defaultIdleDiskActivity.time+defaultIdleDiskActivity.duration;
		spindownDiskActivity.status='S';
		spindownDiskActivity.duration=inDiskActivity.time-spindownDiskActivity.time;

		putDiskActivityHistory(defaultIdleDiskActivity);
		putDiskActivityHistory(spindownDiskActivity);
		putDiskActivityHistory(inDiskActivity);
	}

}

void diskActivity::diskActivityHistoryPrint()
{
	list<diskActivityHistory_t>::iterator iter;

	for (iter= diskActivityHistory.end(); iter!=diskActivityHistory.begin();iter--)
	{
		printf("disk objID: %llu, status: %c, time: %lf, duration: %lf\n",
				iter->objID,iter->status,iter->time,iter->duration);

	}
}


