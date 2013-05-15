/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Block.hh,v 1.5 2002/02/15 15:44:24 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _MEMORYREGION_HH_
#define _MEMORYREGION_HH_

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
namespace MemoryRegion {
  using std::map;


  typedef struct {
    uint64_t objID;
    uint64_t blockID;
    uint64_t len;
    double H;
  } memory_region_t;

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
    bool operator()(const memory_region_t &mr1, const memory_region_t &mr2) const {
      return (mr1.objID < mr2.objID ||
	      (mr1.objID == mr2.objID &&
	       mr1.blockID < mr2.blockID)||
	      (mr1.objID == mr2.objID &&
	       mr1.blockID == mr2.blockID &&
               mr1.len < mr2.len)||
	      (mr1.objID == mr2.objID &&
	       mr1.blockID == mr2.blockID &&
               mr1.len == mr2.len&&
               mr1.H < mr2.H));
    }
  };

  /**
   * Per-block counter.
   */
  typedef map<memory_region_t, uint64_t, LessThan> Counter;
};

#endif /* _MEMORYREGION_HH_ */
