/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Ghost.hh,v 1.2 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _GHOST_HH_
#define _GHOST_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "Block.hh"
#include "Cache.hh"
#include "IORequest.hh"
#include "Statistics.hh"

/**
 * Ghost cache for tracking read and demoted blocks [Wong2002].
 *
 * Ghost caches are used by adaptive caches to evaluate the effectiveness
 * of caching only read or demoted blocks. A ghost cache is maintained for
 * each type of block the actual cache holds: blocks read from disk, and
 * blocks demoted by higher-level storage devices. Each ghost is the same
 * size as the actual cache.
 *
 * When a read request is received by the actual cache from a higher-level
 * storage device, the {read, demote} ghost cache is checked to see if the
 * request would have hit in an actual array that only cached {read,
 * demoted} blocks. The hit counts are incremented accordingly. The hit
 * counts are later used in a "probability function" (this might be a
 * misnomer) to determine where in the real cache to insert read or demoted
 * blocks.
 */
class Ghost : public Statistics {
private:
  Cache demote;
  uint64_t demoteReadHits;

  Cache read;
  uint64_t readReadHits;

  double demoteProb;
  double readProb;

  bool normalizeFlag;

private:
  // Copy constructors - declared private and never defined

  Ghost(const Ghost&);
  Ghost& operator=(const Ghost&);

public:
  /**
   * Create a ghost cache.
   */
  Ghost(const char *inName,
	uint64_t inCacheSize,
	bool inNormalizeFlag) :
    Statistics(inName),
    demote(inCacheSize),
    demoteReadHits(1),
    read(inCacheSize),
    readReadHits(1),
    demoteProb(0),
    readProb(0),
    normalizeFlag(inNormalizeFlag) { ; };

  /**
   * Put a {read, demoted} block in the {read, demote} ghost.
   *
   * @param inOp The inOp I/O operation type.
   * @param inBlock The block to put.
   */
  void blockPut(IORequest::IORequestOp_t inOp,
		Block::block_t inBlock);
  /**
   * Update the hit counts and "probabilities" in the ghosts. When a
   * read-requested block is "received" in the cache, the hit counts for
   * the {read, demote} ghost is incremented if the requested block would
   * have been cached in a purely {read, demote} cache. Then, the read and
   * demote "probabilities" are updated.
   *
   * @warning This function should only be called by read-requested blocks.
   */
  void probUpdate(Block::block_t inBlock);

  /**
   * Return the {read, demote} "probability".
   *
   * @param inOp The "probability" to return.
   */
  double probGet(IORequest::IORequestOp_t inOp);

  void statisticsReset();

  void statisticsShow() const;
};

inline double
Ghost::probGet(IORequest::IORequestOp_t inOp)
{
  double retval;

  if (normalizeFlag) {
    retval =
      (inOp == IORequest::Demote ? demoteProb : readProb) /
      max(demoteProb, readProb);
  }
  else {
    retval = (inOp == IORequest::Demote ? demoteProb : readProb);
  }    
  return (retval);
}


#endif /* _GHOST_HH_ */
