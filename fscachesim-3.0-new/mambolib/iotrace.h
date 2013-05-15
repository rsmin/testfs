/* 
 * iotrace.h: structures used for the in-core and binary versions. 
 * author: Anurag Acharya, acha@cs.umd.edu
 *
 */

# if !defined(_IOTRACE_H_)
#define _IOTRACE_H_

typedef struct STRING
{
   int length;
   char *name;
} string_t;

typedef struct TRACEHEADER
{
  int numOfProcesses;
  int numOfFiles;
  int numOfRecords;
  int offsetToTraceRecords;
  string_t *fileNames;
} traceHeader_t;

/* enums don't survive  dumps*/
#define OPEN 0
#define CLOSE 1
#define READ 2
#define WRITE 3
#define SEEK 4
#define LISTIO_HEADER 5

typedef struct TRACEREC
{
  int          op;
  int          numRecords;
  int          pid;
  int          fileId;
  double       wallClock;
  double       processClock;
  long         offset;
  long         length;
} tracerec_t;

/* alignment for char strings */
#define SALIGN 4

#endif /* _IOTRACE_H_ */
