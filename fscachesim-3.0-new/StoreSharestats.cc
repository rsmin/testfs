/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreSharestats.cc,v 1.3 2002/02/18 22:19:31 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "StoreSharestats.hh"

using namespace Block;

StoreSharestats::~StoreSharestats()
{
  for (OrigMap::iterator i = origToAccessMap.begin();
       i != origToAccessMap.end();
       i++) {
    delete i->second;
  };
}

bool
StoreSharestats::IORequestDown(const IORequest& inIOReq)
{
  block_t block = {inIOReq.objIDGet(), inIOReq.blockOffGet(blockSize)};
  const char *orig = inIOReq.origGet();
  uint64_t reqBlockLen = inIOReq.blockLenGet(blockSize);

  for (uint64_t i = 0; i < reqBlockLen; i++) {
    Counter *theMap = NULL;
    OrigMap::iterator origIter = origToAccessMap.find(orig);

    // Check to see if we've seen this originator before, and if so,
    // if this originator has seen this block.

    if (origIter != origToAccessMap.end()) {
      if (origIter->second->find(block) == origIter->second->end()) {
	// New access - update the number of originators accessing
	// this block.

	theMap = origToAccessMap[orig];
      }
    }
    else {
      origToAccessMap[orig] = new Counter;
      theMap = origToAccessMap[orig];
    }

    if (theMap != NULL) {
      (*theMap)[block] = 0;
      if (accessMap.find(block) == accessMap.end()) {
	accessMap[block] = 1;
      }
      else {
	accessMap[block]++;
      }
    }

    block.blockID++;
  }

  return (true);
}

void
StoreSharestats::statisticsReset()
{
  accessMap.clear();
  origToAccessMap.clear();
}

void
StoreSharestats::statisticsShow() const
{
  // Scan the counts of originators accessing each block, and return a
  // table of number of blocks against number of times that block was
  // accessed.

  int *blocksPerNumOrig = new int[(origToAccessMap.size() + 1)];
  for (int i = 1; i <= origToAccessMap.size(); i++) {
    blocksPerNumOrig[i] = 0;
  }

  for (Counter::const_iterator i = accessMap.begin();
       i != accessMap.end();
       i++) {
    blocksPerNumOrig[i->second]++;
  }

  for (int i = 1; i <= origToAccessMap.size(); i++) {
    printf("%d %d\n", i, blocksPerNumOrig[i]);
  }
}
