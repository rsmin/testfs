/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Block.hh,v 1.5 2002/02/15 15:44:24 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _BLOCK_HH_
#define _BLOCK_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

/**
 * block_t disk block type abstraction and utility functions.
 */
namespace Block {
  using std::map;

  /**
   * block_t models disks as a collection of objects, where each object
   * contains blocks.
   */
  typedef struct {
    /**
     * The object ID.
     */
    uint64_t objID;
    /**
     * The block offset into the object.
     */
    uint64_t blockID;

  } block_t;

  /**
   * "Less than" ordering function class for disk blocks
   */
  class LessThan {
  public:
    /**
     * "Less than" operator. Lower object IDs are less than higher IDs. If
     * two blocks have equal object IDs, one one with a lower block ID is
     * less than the one with a higher ID.
     */
    bool operator()(const block_t &block1, const block_t &block2) const {
      return (block1.objID < block2.objID ||
	      (block1.objID == block2.objID &&
	       block1.blockID < block2.blockID));
    }
  };

  /**
   * Per-block counter.
   */
  typedef map<block_t, uint64_t, LessThan> Counter;
};

#endif /* _BLOCK_HH_ */
