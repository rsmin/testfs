/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCache2Q.cc,v 1.5 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#include <list>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "StoreCache2Q.hh"

using Block::block_t;

/**
 * This cache maintains one ejection queue. The head of the queue is the
 * eject-me-next block. Hence, for LRU and MRU we add blocks at the tail
 * and head respectively.
 */



void
StoreCache2Q::BlockCache(const IORequest& inIOReq,
			     const block_t& inBlock,
			     list<IORequest>& outIOReqs)
{
   const char* reqOrig = inIOReq.origGet();

   if (inIOReq.opGet() == IORequest::Read)
   {
 
       if (TQ.blockGet(inBlock))
       {
	      readHitsPerOrig[reqOrig]++;
	      readHits++; 
       }
       else
       {
	      readMissesPerOrig[reqOrig]++;
	      readMisses++; 
       }
    }
}



void
StoreCache2Q::statisticsShow() const
{
  printf("{StoreCache2Q.%s\n", nameGet());

  if (mode != Analyze)
  {
       printf("\t{size {total=%llu} }\n", TQ.sizeGet() * blockSizeGet());

  }
  StoreCache::statisticsShow();
  if (mode != Analyze)
  {
    printf("}\n");
  }
}
 

