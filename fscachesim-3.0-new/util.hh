#ifndef _UTIL_HH_
#define _UTIL_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

#define NDEBUG

#include <assert.h>
#include <list>
#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif 

uint64_t log2(uint64_t distance);

#endif /* _UTIL_HH_ */
