/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/Char.hh,v 1.4 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _CHAR_H_
#define _CHAR_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <map>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <string.h>

/**
 * Character string utility functions and containers
 */
namespace Char {
  using std::map;

  /**
   * Lexographic sorting function class for strings
   */
  class LessThan {
  public:
    /**
     * Lexographic sorting operator.
     */
    bool operator()(const char *str1, const char *str2) const {
      return (strcmp(str1, str2) < 0);
    };
  };

  /**
   * Per-string counter
   */
  typedef map<const char *, uint64_t, LessThan> Counter;
};

#endif /* _CHAR_H_ */
