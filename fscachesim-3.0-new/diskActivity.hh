#ifndef _DISKACTIVITY_HH_
#define _DISKACTIVITY_HH_

#include <map>
#include <list>
#include "IORequest.hh"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

using namespace std;

namespace DiskActivity{
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

class diskActivity{

private:
	 double access_speed; /*Byte per s*/;
//	const uint64_t write_speed;
	 double spindown_latency;
	 double spinup_latency;

	 double idle_power;
	 double active_power;
	 double spin_power;
	 double spindown_power;
	 double spinup_power;
	 double spinWaitTimeDelta;
	 double initSpintWaitTimeLen;

	 double spinTimeLen_min;

	double spinWaitTimeLen;

	map<uint64_t, double> spinWaitIndex; //to record objID & spinWaitTimeLen;
	//	SpinWaitIndex spinWaitIndex;

	// Disk operation history
	list<diskActivityHistory_t> diskActivityHistory;

private:
  // Copy constructors - declared private and never defined

	diskActivity(const diskActivity&);
	diskActivity& operator=(const diskActivity&);

public:
	diskActivity():
		access_speed(10000000), /*Byte per s*/
	//	const uint64_t write_speed;
		spindown_latency(0.0004),
		spinup_latency(0.0002),
		idle_power(0.1),
		active_power(5),
		spin_power(0.002),
		spindown_power(1),
		spinup_power(1),
		spinWaitTimeDelta(0.1),
		initSpintWaitTimeLen(0.1),
		spinTimeLen_min(
					((spindown_power-idle_power)*spindown_latency
				+	(spinup_power-idle_power)*spinup_latency)
				/(idle_power-spin_power)),
		spinWaitTimeLen(),
		diskActivityHistory(),
		spinWaitIndex(){;};

	~diskActivity(){;};

void putDiskActivityHistory
	(const diskActivityHistory_t inDiskActivityHistory);

diskActivityHistory_t checkLastDiskActivity
	(uint64_t objID);

void writeDiskWithSpinDown(diskActivityHistory_t inDiskActivity);
void writeDiskWithoutSpinDown(diskActivityHistory_t inDiskActivity);

double access_speedGet() const{return (access_speed);};
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

}

#endif /* _DISKACTIVITY_HH_ */


