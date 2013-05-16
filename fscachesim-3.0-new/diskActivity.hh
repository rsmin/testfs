#ifndef _DISKACTIVITY_HH_
#define _DISKACTIVITY_HH_

#include <list>
#include "IORequest.hh"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

class diskActivity{

private:
	const uint64_t read_speed /*MB per s*/;
	const uint64_t write_speed;
	const uint64_t spindown_latency;
	const uint64_t spinup_latency;

	const uint64_t idle_power;
	const uint64_t active_power;
	const uint64_t spin_power;
	const uint64_t spindown_power;
	const uint64_t spinup_power;

	uint64_t spinWaitTimeLen;

	uint64_t idleTimeLen;
	uint64_t spinTimeLen;
	uint64_t activeTimeLen;

	uint64_t spinTimeLen_min =
			((spindown_power-idle_power)*spindown_latency
		+	(spinup_power-idle_power)*spinup_latency)
		/(idle_power-spin_power);

private:
  // Copy constructors - declared private and never defined

	diskActivity(const diskActivity&);
	diskActivity& operator=(const diskActivity&);

	//history structure
	typedef struct {
		uint64_t objID;
	    uint64_t time;
	    char status; //the disk status should be 'A' for active, 'I' for Idle and 'S' for spinning down
	  } diskActivityHistory_t;

  // Disk operation history
	list<diskActivityHistory_t> diskActivityHistory;


public:

void putDiskActivityHistory
	(const diskActivityHistory_t inDiskActivityHistory)

void checkLastDiskActivity
	(diskActivityHistory_t& outLastDiskActivity)

};

inline void
writeCache::putDiskActivityHistory
	(const diskActivityHistory_t inDiskActivityHistory)
{
	diskActivityHistory.push_front(inDiskActivityHistory);
	diskActivityHistoryIndex[inDiskActivityHistory] = diskActivityHistory.begin();
}

inline void
writeCache::checkLastDiskActivity(diskActivityHistory_t& outLastDiskActivity)
{
	if (!diskActivityHistory.empty())
	outLastDiskActivity = *diskActivityHistory.begin();
};




};
