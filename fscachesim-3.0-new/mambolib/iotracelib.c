/* 
 * iotracelib.c: implementation of lib routines for accessing the trace
 * author: Anurag Acharya, acha@cs.umd.edu 
 *
 */

#include <stdio.h>
#include <assert.h>
#include "iotracelib.h"

extern void *malloc(int size);

#define RECBUFSIZE 512

typedef struct TRACEFILE
{
  char          *name;
  FILE          *fp;
  traceHeader_t  header;
  tracerec_t    *recbuffer;
  int            numrecs; 
  int            recpt; 
} tracefile_t;

static tracefile_t headers[MAX_OPEN_TRACES];
static int initialized = 0;
static char line[1024];

int openTrace(char *tracefile)
{
  int i,j;
  int len;

  /* check if initialization has been done -- if not, init the space */
  if (!initialized)
  {
    for (i = 0; i < MAX_OPEN_TRACES; i++)
      headers[i].name = NULL;
    initialized = 1;
  }

  /* find a free record -- if none, print error message and return */
  for (i = 0; i < MAX_OPEN_TRACES; i++)
    if (headers[i].name == NULL) 
      break;

  if (i == MAX_OPEN_TRACES) 
  {
    fprintf(stderr,"Too many open traces. Currently, %d are open.\n",i);
    return -1;
  }
  
  headers[i].name = (char *) malloc(strlen(tracefile)+1);
  assert(headers[i].name);
  memcpy(headers[i].name,tracefile,strlen(tracefile)+1);

  headers[i].fp  = fopen(tracefile,"r");
  assert(headers[i].fp);

  fread(&(headers[i].header),sizeof(traceHeader_t),1,headers[i].fp);
  assert(headers[i].header.numOfProcesses > 0);
  assert(headers[i].header.numOfFiles > 0);
  assert(headers[i].header.numOfRecords > 0);
  
  len = sizeof(string_t)*headers[i].header.numOfFiles;
  headers[i].header.fileNames = (string_t *) malloc(len);
  assert(headers[i].header.fileNames);

  fread(headers[i].header.fileNames,sizeof(string_t),
        headers[i].header.numOfFiles,headers[i].fp);

  for (j = 0; j < headers[i].header.numOfFiles; j++)
  {
    int linelen = headers[i].header.fileNames[j].length;

    fread(line,sizeof(char),linelen,headers[i].fp);
    
    headers[i].header.fileNames[j].name = (char *) malloc(linelen);
    assert(headers[i].header.fileNames[j].name);
    memcpy(headers[i].header.fileNames[j].name,line,linelen);
  }

  headers[i].numrecs = 0;
  headers[i].recpt = 0;
  headers[i].recbuffer = (tracerec_t *) malloc(sizeof(tracerec_t)*RECBUFSIZE);
  assert(headers[i].recbuffer);

  return i;
}

void closeTrace(int td)
{
  int i;
  assert(td >= 0 && td < MAX_OPEN_TRACES);

  if (headers[td].name == NULL)
    return;

  fclose(headers[td].fp);

  for (i = 0; i < headers[td].header.numOfFiles; i++)
    free(headers[td].header.fileNames[i].name);

  free(headers[td].header.fileNames);
  headers[td].header.fileNames = NULL;

  free(headers[td].recbuffer);
  headers[td].recbuffer = NULL;
}

int getNextRecord(int td,tracerec_t *record)
{
  assert(td >= 0 && td < MAX_OPEN_TRACES);
  assert(record);
  assert(headers[td].name);

  if (headers[td].recpt >= headers[td].numrecs)
  {
    int ret;
    /* refill buffer */
    ret = fread(headers[td].recbuffer,sizeof(tracerec_t),RECBUFSIZE,
		headers[td].fp);
    if (ret <= 0)
      return 0;

    headers[td].numrecs = ret;
    headers[td].recpt   = 0;
  }
  
  memcpy(record,headers[td].recbuffer+headers[td].recpt,sizeof(tracerec_t));
  headers[td].recpt++;

  return 1; /* success */
}

int  getFileIdFromName(int td,char *fileName)
{
  int i;
  assert(td >= 0 && td < MAX_OPEN_TRACES);
  assert(headers[td].name);

  for (i = 0; i < headers[td].header.numOfFiles; i++)
    if (strcmp(headers[td].header.fileNames[i].name,fileName) == 0)
      return i;

  assert(0);
}

char *getNameFromFileId(int td,int fileId)
{
  int i;
  assert(td >= 0 && td < MAX_OPEN_TRACES);
  assert(headers[td].name);
  assert(fileId >= 0 && fileId < headers[td].header.numOfFiles);

  return headers[td].header.fileNames[fileId].name;
}

int  getNumProcesses(int td)
{
  assert(td >= 0 && td < MAX_OPEN_TRACES);
  assert(headers[td].name);
  
  return headers[td].header.numOfProcesses;
}


int  getNumFiles(int td)
{
  assert(td >= 0 && td < MAX_OPEN_TRACES);
  assert(headers[td].name);
  
  return headers[td].header.numOfFiles;
}

int  getNumRecords(int td)
{
  assert(td >= 0 && td < MAX_OPEN_TRACES);
  assert(headers[td].name);
  
  return headers[td].header.numOfRecords;
}

void main()
{
  int td;
  tracerec_t record;

  td = openTrace("test.bin");
  printf("fiel id = %d\n", getFileIdFromName(td,"acha"));
  printf("fiel id = %d\n", getFileIdFromName(td,"acharya"));
  printf("file name = %s\n", getNameFromFileId(td,0));
  printf("file name = %s\n", getNameFromFileId(td,1));
  printf("Numprocs = %d\n",getNumProcesses(td));
  printf("NumFiles = %d\n",getNumFiles(td));
  printf("Numrcs = %d\n", getNumRecords(td));
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  getNextRecord(td,&record);
  closeTrace(td);
}
