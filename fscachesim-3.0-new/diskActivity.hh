#ifndef _DISKACTIVITY_HH_
#define _DISKACTIVITY_HH_
#include <map>
#include <list>
#include "IORequest.hh"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

class diskActivity{

private:
	const uint64_t access_speed /*Byte per s*/;
//	const uint64_t write_speed;
	const double spindown_latency;
	const double spinup_latency;

	const uint64_t idle_power;
	const uint64_t active_power;
	const uint64_t spin_power;
	const uint64_t spindown_power;
	const uint64_t spinup_power;

	const double spinTimeLen_min =
			((spindown_power-idle_power)*spindown_latency
		+	(spinup_power-idle_power)*spinup_latency)
		/(idle_power-spin_power);

	const double spinWaitTimeDelta;
	const double initSpintWaitTimeLen;
	double spinWaitTimeLen;


private:
  // Copy constructors - declared private and never defined

	diskActivity(const diskActivity&);
	diskActivity& operator=(const diskActivity&);

	typedef map<uint64_t, double> SpinWaitIndex; //to record objID & spinWaitTimeLen;
	SpinWaitIndex spinWaitIndex;

	//history structure
	typedef struct {
		uint64_t objID;
	    double time;
	    double duration;
	    char status;
	    /*the disk status should be 'A' for active;
	     * 'I' for Idle;
	     * 'S' for spinning downd
	     */
	  } diskActivityHistory_t;
	// Disk operation history
	list<diskActivityHistory_t> diskActivityHistory;



public:

void putDiskActivityHistory
	(const diskActivityHistory_t inDiskActivityHistory);

diskActivityHistory_t* checkLastDiskActivity
	(uint64_t objID);

uint64_t access_speedGet() const{return (access_speed);};
double spinTimeLen_minGet() const{return (spinTimeLen_min);};

double spinWaitTimeLenGet(uint64_t inObjID);
void setSpinWaitTimeLen(diskActivityHistory_t inDiskActivity);

bool isEmpty() const{return diskActivityHistory.empty();};
};




inline void
diskActivity::putDiskActivityHistory
	(const diskActivityHistory_t inDiskActivityHistory)
{
	diskActivityHistory.push_front(inDiskActivityHistory);
};





