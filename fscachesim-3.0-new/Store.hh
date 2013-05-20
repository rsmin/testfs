/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Store.hh,v 1.3 2002/02/15 18:17:30 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _STORE_HH_
#define _STORE_HH_

using namespace std;

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "IORequest.hh"
#include "Statistics.hh"
#include "Block.hh"
/**
 * Interface for objects that model storage devices. Classes that inherit
 * from Store must implement all abstract methods.
 */
class Store : public Statistics {
public:
  // Policies

  /**
   * Cache demotion policy.
   */
  enum Mode_t {
    Simulate, 
    Analyze
  };
protected:

  Mode_t mode; 
  /**
   * The lower-level storage device, if any.
   */
  Store *nextStore;

  /**
   * The size of blocks in the storage device.
   */
  uint64_t blockSize;

  /**
   * The number of received demotions that hit in the storage device.
   */
  uint64_t demoteHits;

  /**
   * The number of received demotions that missed in the storage device.
   */
  uint64_t demoteMisses;

  /**
   * The number of received reads that hit in the storage device.
   */
  uint64_t readHits;
  /**
   * The number of received reads that missed in the storage device.
   */
  uint64_t readMisses;

  uint64_t cooperativeHits;
//Added by Xiaodong
  uint64_t writeMisses;
  uint64_t writeHits;

private:
  // Copy constructors - declared private and never defined

  Store(const Store&);
  Store& operator=(const Store&);

public:
  // Constructors and destructors

  /**
   * Create a block store.
   *
   * @param inName A string name for the store.
   * @param inNextStore A lower-level storage device (can be NULL).
   * @param inBlockSize The size of each block, in bytes.
   */
  Store(const char *inName,
	Store *inNextStore,
	uint64_t inBlockSize);

  /**
   * Create a block store.
   *
   * @param inName A string name for the store.
   * @param inBlockSize The size of each block, in bytes.
   */
  Store(const char *inName,
	uint64_t inBlockSize);

  /**
   *Destroy a block store.
   */
  virtual ~Store() { ; };

  /**
   * Get the block size.
   */
  uint64_t blockSizeGet() const { return (blockSize); };

  // I/O request handlers

  /**
   * Receive an incoming I/O request sent down from a higher-level storage
   * device or request generator.
   *
   * @param inIOReq The I/O request from the higher-level storage device or
   * request generator.
   *
   * @return true if the request was handled successfully, false otherwise.
   */
  virtual bool IORequestDown(const IORequest &inIOReq) = 0;
  virtual void addUpStore(Store *inStore) {;};
  virtual bool isCached(const Block::block_t& inBlock) {return false;};
  virtual bool isUpCached(const Block::block_t& inBlock){return false;};

  // Statistics management

  virtual void statisticsReset() {
    demoteHits = 0;
    demoteMisses = 0;
    readHits = 0;
    readMisses = 0;
    cooperativeHits = 0;
  };

  void changemode(Mode_t inMode);
};

inline
Store::Store(const char *inName,
	     Store *inNextStore,
	     uint64_t inBlockSize) :
  Statistics(inName),
  nextStore(inNextStore),
  blockSize(inBlockSize),
  mode(Simulate)
{
  statisticsReset();
  if (inNextStore)
    inNextStore->addUpStore(this);
};

inline
Store::Store(const char *inName,
	     uint64_t inBlockSize) :
  Statistics(inName),
  nextStore(NULL),
  blockSize(inBlockSize),
  mode(Simulate)
{
  statisticsReset();
};

inline
void Store::changemode(Mode_t inMode)
{
   mode = inMode;
}

#endif /* _STORE_HH_ */
