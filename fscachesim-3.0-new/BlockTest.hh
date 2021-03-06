/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Block.hh,v 1.5 2002/02/15 15:44:24 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _BLOCKTEST_HH_
#define _BLOCKTEST_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */


namespace BlockTest {


   typedef struct {

    uint64_t objID;

    uint64_t blockID;
 
 

       
  } block_data_t;   

  
   typedef struct {

    block_data_t data;

    uint64_t regTime;

       
  } block_t;   

  class LessThan {
  public:

    bool operator()(const block_data_t &block1, const block_data_t &block2) const {
      return (block1.objID < block2.objID ||
	      (block1.objID == block2.objID &&
	       block1.blockID < block2.blockID));
    }
  };



};

#endif /* _BLOCKTEST_HH_ */
