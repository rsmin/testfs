/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCache.hh,v 1.3 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORECACHE_HH_
#define _STORECACHE_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>


#include "Block.hh"
#include "Cache.hh"
#include "BlockBuffer.hh"
#include "DemoteStat.hh"
#include "Char.hh"
#include "IORequest.hh"
#include "Store.hh"


/**
 * Interface for finite block caches. Classes that inherit from
 * StoreCache must implement all abstract methods.

 */

  
class StoreCache : public Store {

private:
  bool logRequestFlag;

protected:
  /**
   * Per-originator counts of demotes that hit in the cache.
   */
  Char::Counter demoteHitsPerOrig;

  /**
   * Per-originator counts of demotes that miss in the cache.
   */
  Char::Counter demoteMissesPerOrig;

  /**
   * Per-originator counts of reads that hit in the cache.
   */
  Char::Counter readHitsPerOrig;

  /**
   * Per-originator counts of reads that miss in the cache.
   */
  Char::Counter readMissesPerOrig;

  Char::Counter coopHitsPerOrig;

  uint64_t size;

private:

  BlockBuffer *request[STATNUM];


  BlockBuffer* req_dis_block[STATNUM][MAXDIS];
  uint64_t req_dis_access[STATNUM][MAXDIS][MAXACC][2];
  uint64_t req_dis_total_access[STATNUM][MAXDIS][MAXACC][2];

  uint64_t req_distance[STATNUM+1][MAXDIS];
  uint64_t req_total[STATNUM+1][2];
  uint64_t req_access[STATNUM][MAXACC][2];
  uint64_t req_total_access[STATNUM][MAXACC][2];
  double access_perc[STATNUM][MAXACC][2];


  uint64_t reqlens[MAXACC];
  double reqlens_perc[MAXACC];


  DemoteStat ds;
 
  uint64_t time, demotetime;


  void init(); 

  // Copy constructors - declared private and never defined

  StoreCache(const StoreCache&);
  StoreCache& operator=(const StoreCache&);

  void UpdateTotalRef();
  void  UpdateRef(BlockBuffer *history, uint64_t (*req_access)[MAXACC][2], bool cali = false);
  void  AccumulateRef(uint64_t (*req_access)[MAXACC][2], 
            uint64_t (*req_accu_access)[MAXACC][2]);
  void CalcPerc(uint64_t (*req_total_access)[MAXACC][2], 
            double (*access_perc)[MAXACC][2]);

  void simulationShow() const;
  void analyzeShow() const;
  void distanceShow()  const;
  void freqShow()  const;
  void freqSpecShow()  const;
  void demotefreqShow()  const;
  void crdemotedistShow() const;

  void reqlensShow() const;
  void CalcLensPerc();

protected:
  /**
   * Cache management function. All the work of inserting and ejecting
   * blocks is done in this function.
   *
   * @param inIOReq The I/O request containing the blocks.
   * @param inBlock A block from the I/O request.
   * @param outIOReqs A list of new I/O requests to send to the lower-level
   * storage device, if any.
   */
  virtual void BlockCache(const IORequest& inIOReq,
			  const Block::block_t& inBlock,
			  list<IORequest>& outIOReqs) = 0;

  void UpdateRefDistance(const IORequest& inIOReq, const Block::block_t& inBlock);
  void UpdateRead(const IORequest& inIOReq, const Block::block_t& inBlock);
  void UpdateDemote(const IORequest& inIOReq, const Block::block_t& inBlock);

  void UpdateReqLens(uint64_t lens);

public:
  // Constructors and destructors

  /**
   * Create a block cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device (can be NULL).
   * @param inBlockSize The size of each block, in bytes.
   */
  StoreCache(const char *inName,
	     Store *inNextStore,
	     uint64_t inBlockSize,
             uint64_t inSize) :
    Store(inName, inNextStore, inBlockSize),
    logRequestFlag(false),
    time(0),
    demotetime(0),
    size(inSize)
   { 
       init();
    
   };

  /**
   * Create a block cache.
   *
   * @param inName A string name for the cache.
   * @param inNextStore A lower-level storage device (can be NULL).
   * @param inBlockSize The size of each block, in bytes.
   */
  StoreCache(const char *inName,
	     uint64_t inBlockSize,
             uint64_t inSize) :
    Store(inName, inBlockSize),
    logRequestFlag(false), 
    time(0),
    demotetime(0),
    size(inSize)
   { 
      init();
    };


  /**
   * Destroy a block cache.
   */
  ~StoreCache();

  // I/O request handlers

  virtual bool IORequestDown(const IORequest& inIOReq);

  /**
   * Log incoming I/O requests. Initially set to not log.
   */
  void logRequestToggle() {
    logRequestFlag = (logRequestFlag ? false : true);
  };

  // Statistics management

  virtual void statisticsReset();

  virtual void statisticsShow() const;

  virtual void beforeShow();


};

#endif /* _STORECACHE_HH_ */
