/* 
 * iotracelib.h: interface for lib routines for accessing the trace
 * author: Anurag Acharya, acha@cs.umd.edu 
 *
 */

#if !defined(_IOTRACELIB_H_)
#define _IOTRACELIB_H_

#include "iotrace.h"

extern int openTrace(char *tracefile);
extern void closeTrace(int traceDescr);
extern int  getNextRecord(int traceDescr,tracerec_t *record);
extern int  getFileIdFromName(int traceDescr,char *fileName);
extern char * getNameFromFileId(int traceDescr,int fileId);
extern int  getNumProcesses(int td);
extern int  getNumFiles(int td);
extern int  getNumRecords(int td);

#define MAX_OPEN_TRACES 512

#endif /* _IOTRACELIB_H_ */
