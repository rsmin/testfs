#include <map>
#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#include "diskActivity.hh"
#include "Block.hh"

using diskActivity::diskActivityHistory_t;

diskActivityHistory_t* diskActivity::checkLastDiskActivity(uint64_t inObjID)
{
	diskActivityHistory_t outLastDiskActivity;
	list<diskActivityHistory_t>::iterator iter = diskActivityHistory.begin();

	while(iter->objID != inObjID || iter != diskActivityHistory.end())
	{
		iter++;
	}
	if (iter->objID = inObjID)
	return iter;
	else{
		outLastDiskActivity.objID=inObjID;
		outLastDiskActivity.status = 'A';
		outLastDiskActivity.time=0;
		outLastDiskActivity.duration=0;
	return outLastDiskActivity;
	}
};

double diskActivity::spinWaitTimeLenGet(uint64_t inObjID){
	SpinWaitIndex::iterator iter= spinWaitIndex.find(inObjID);
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
	diskActivityHistory_t* lastActivity = checkLastDiskActivity(inObjID);

	double WaitTimeDuration = inDiskActivity.time - lastActivity->time - lastActivity->duration;

	SpinWaitIndex::iterator iter= spinWaitIndex.find(inObjID);
	if (WaitTimeDuration >= spinTimeLen_min)
	{
		if(iter == spinWaitIndex.end()) {
			spinWaitIndex[inObjID] =
					initSpintWaitTimeLen+(WaitTimeDuration%spinTimeLen_min)*spinWaitTimeDelta;
		}
		else{
			spinWaitIndex[inObjID] =
					spinWaitIndex[inObjID]+(WaitTimeDuration%spinTimeLen_min)*spinWaitTimeDelta;
		}
	}
	else
	{
		if(iter == spinWaitIndex.end()) {
			spinWaitIndex[inObjID] =
					initSpintWaitTimeLen-(spinTimeLen_min%WaitTimeDuration)*spinWaitTimeDelta;
		}
		else{
			spinWaitIndex[inObjID] =
					spinWaitIndex[inObjID]-(spinTimeLen_min%WaitTimeDuration)*spinWaitTimeDelta;
		}
	}

	if (spinWaitIndex[inObjID] > spinTimeLen_min)
		spinWaitIndex[inObjID] = spinTimeLen_min;
	if (spinWaitIndex[inObjID] < 0)
		spinWaitIndex[inObjID] = 0;

}
