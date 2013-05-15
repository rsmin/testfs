/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/IORequest.hh,v 1.10 2002/02/15 15:44:25 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/

#ifndef _IOREQUEST_HH_
#define _IOREQUEST_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>

/**
 * I/O request object abstraction.
 **/
class IORequest {
public:
  /**
   * Type of I/O operation.
   */
  enum IORequestOp_t {
    Demote, /**< Demote operation */
    Read, /**< Read operation */
    Write /**<Write Disk operation - xiaodong*/
  };

private:
  const char *orig;

  IORequestOp_t op;

  double timeIssued;

  uint64_t objID;
  uint64_t off;
  uint64_t len;

private:
  // Copy constructors - declared private and never defined

  IORequest& operator=(const IORequest&);

public:
  // Constructors and destructors

  /**
   * Create an I/O request.
   *
   * @param inOrig The name of the request originator.
   * @param inOp The operation type of the request.
   * @param inTimeIssued The time the request was issued.
   * @param inObjID The object ID of the request.
   * @param inOff The offset into the object of the request.
   * @param inOff The lenght of the request.
   */
  IORequest(const char *inOrig,
	    IORequestOp_t inOp,
	    double inTimeIssued,
	    uint64_t inObjID,
	    uint64_t inOff,
	    uint64_t inLen) :
    orig(inOrig),
    op(inOp),
    timeIssued(inTimeIssued),
    objID(inObjID),
    off(inOff),
    len(inLen) { ; };

  /**
   * Copy an I/O request.
   *
   * @param inIOReq The request to be copied.
   */
  IORequest(const IORequest& inIOReq) :
    orig(inIOReq.orig),
    op(inIOReq.op),
    timeIssued(inIOReq.timeIssued),
    objID(inIOReq.objID),
    off(inIOReq.off),
    len(inIOReq.len) { ; };

  /**
   * Destroy the request.
   */
  ~IORequest() { ; };

  // Accessors

  /**
   * Get the name of the originator.
   */
  const char *origGet() const {
    return (orig);
  };

  /**
   * Get the operation type of the request.
   */
  IORequestOp_t opGet() const {
    return (op);
  };

  /**
   * Get the issue time of the request.
   */
  double timeIssuedGet() const {
    return (timeIssued);
  };

  /**
   * Get the object ID that the request accesses.
   */
  uint64_t objIDGet() const {
    return (objID);
  };

  /**
   * Get the offset into of the object that the request accesses.
   */
  uint64_t offGet() const {
    return (off);
  };

  /**
   * Get the length of the request.
   */
  uint64_t lenGet() const {
    return (len);
  };

  /**
   * Get the offset into of the object that the request accesses, in blocks.
   *
   * @param inBlockSize The block size.
   */
  uint64_t blockOffGet(const uint64_t inBlockSize) const {
    return (off / inBlockSize);
  };

  /**
   * Get the length of the request, in block.
   *
   * @param inBlockSize The block size.
   */
  uint64_t blockLenGet(const uint64_t inBlockSize) const {
    //  The following expression is len + starting fill + ending fill.

    return ((len + (off % inBlockSize) +
	     ((len + off) % inBlockSize ?
	      inBlockSize - ((len + off) % inBlockSize) : 0)) / inBlockSize);
  };
};

#endif /* _IOREQUEST_HH_ */
