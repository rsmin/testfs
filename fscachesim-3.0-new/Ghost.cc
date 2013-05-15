/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Ghost.cc,v 1.4 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <algorithm>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif /* HAVE_STDLIB_H */

#include "Block.hh"
#include "IORequest.hh"

#include "Ghost.hh"

using Block::block_t;

void
Ghost::blockPut(IORequest::IORequestOp_t inOp,
		Block::block_t inBlock)
{
  switch(inOp) {
  case IORequest::Demote:
    if (demote.isCached(inBlock)) {

      demote.blockGet(inBlock);
    }
    else if (demote.isFull()) {
      block_t ejectBlock;

      demote.blockGetAtHead(ejectBlock);
    }
    demote.blockPutAtTail(inBlock);
    break;
  case IORequest::Read:
    if (read.isCached(inBlock)) {

      read.blockGet(inBlock);
    }
    else if (read.isFull()) {
      block_t ejectBlock;

      read.blockGetAtHead(ejectBlock);
    }
    read.blockPutAtTail(inBlock);
    break;
  default:
    abort();
  }
}

void
Ghost::probUpdate(Block::block_t inBlock)
{
  if (demote.isCached(inBlock)) {
    // If all we cached was demotes, we would have won.

    demote.blockGet(inBlock);
    demote.blockPutAtTail(inBlock);
    demoteReadHits++;
  }
  if (read.isCached(inBlock)) {
    // If all we cached was reads, we would have won.

    read.blockGet(inBlock);
    read.blockPutAtTail(inBlock);
    readReadHits++;
  }

  // Update probabilities.

  demoteProb =
    (double)demoteReadHits / (demoteReadHits + readReadHits);
  readProb =
    (double)readReadHits / (demoteReadHits + readReadHits);
}

void
Ghost::statisticsReset()
{
  demoteReadHits = 1;
  readReadHits = 1;

  demoteProb = 0;
  readProb = 0;
}

void
Ghost::statisticsShow() const
{
  printf("{Ghost.%s\n", nameGet());
  printf("\t{readReadHits=%llu}\n", readReadHits);
  printf("\t{demoteReadHits=%llu}\n", demoteReadHits);
  printf("\t{normalizeFlag=%s}\n", (normalizeFlag ? "true" : "false"));
  printf("}\n");
}
