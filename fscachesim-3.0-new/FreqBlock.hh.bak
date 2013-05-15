/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Block.hh,v 1.5 2002/02/15 15:44:24 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _FREQBLOCK_HH_
#define _FREQBLOCK_HH_

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
namespace FreqBlock {

  typedef struct {

    uint64_t freq;
    uint64_t dist;

    uint64_t demotetime;

  } history_t;

  typedef struct {
    /**
     * The object ID.
     */
    uint64_t objID;
    /**
     * The block offset into the object.
     */
    uint64_t blockID;

    uint64_t freq;
    uint64_t dist;
    uint64_t time;
    uint64_t lastchecktime;

  } freq_block_t;

  typedef struct {

    uint64_t freq;
    uint64_t dist;

    uint64_t time;
    uint64_t checktime;

  } freq_time_t;

  typedef struct {

    uint64_t checktime;
    uint64_t time;

  } check_time_t;

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
    bool operator()(const freq_time_t &block1, const freq_time_t &block2) const {
/*      return (block1.freq < block2.freq ||
               (block1.freq == block2.freq &&
	       block1.dist > block2.dist)||
                (block1.freq == block2.freq && 
                block1.dist == block2.dist &&
	        block1.time < block2.time));*/

      return (block1.freq < block2.freq ||
               (block1.freq == block2.freq &&
	       block1.checktime < block2.checktime)||
                (block1.freq == block2.freq && 
                block1.checktime == block2.checktime &&
	        block1.time < block2.time));

    }

    bool operator()(const check_time_t &block1, const check_time_t &block2) const {

      return (block1.checktime < block2.checktime ||
               (block1.checktime == block2.checktime &&
	       block1.time < block2.time));

    }
  };


};

#endif /* _FREQBLOCK_HH_ */
