
#ifndef _LOCALBLOCK_HH_
#define _LOCALBLOCK_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */

   


namespace LocalBlock {

   typedef struct {

    uint64_t objID;

    uint64_t blockID;

   } block_data_t;   

  typedef struct {
    block_data_t data; 
    uint64_t refNum;
    uint64_t refTime;
  } local_block_t;

  class LessThan {
  public:

    bool operator()(const block_data_t &block1, const block_data_t &block2) const {
      return (block1.objID < block2.objID ||
	      (block1.objID == block2.objID &&
	       block1.blockID < block2.blockID));
    }
  };

};

#endif /* _LOCALBLOCK_HH_ */
