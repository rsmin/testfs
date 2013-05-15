/* 
 * bin2asc: converts binary trace to ascii
 * author: Anurag Acharya, acha@cs.umd.edu 
 *
 */

#include <stdio.h>
#include <assert.h>
#include "iotrace.h"

FILE *asciiFile;
FILE *binaryFile;
char line[1024];

typedef struct OpNameMap
{
  int op;
  char *name;
} opNameMap_t;

opNameMap_t nameMapTable[] = 
{
  {READ, "read"},
  {WRITE, "write"},
  {OPEN, "open"},
  {CLOSE, "close"},
  {SEEK, "seek"},
  {LISTIO_HEADER, "listio_header"},
  {READ, "READ"},
  {WRITE, "WRITE"},
  {OPEN, "OPEN"},
  {CLOSE, "CLOSE"},
  {SEEK, "SEEK"},
  {LISTIO_HEADER, "LISTIO_HEADER"},
};

int countMapTable = 10;

char *mapOpId(int op)
{
  int i;

  for (i = 0; i < countMapTable; i++)
    if (nameMapTable[i].op == op)
     return nameMapTable[i].name;

  fprintf(stderr,"Unable to find operation '%d'. Aborting.\n",op);
  exit(1);
}

int processHeader()
{
  traceHeader_t header;
  int i;
  int len;

  fread(&header,sizeof(header),1,binaryFile);
  
  assert(header.numOfProcesses > 0);
  assert(header.numOfFiles > 0);
  assert(header.numOfRecords > 0);
  len = sizeof(string_t)*header.numOfFiles;
  header.fileNames = (string_t *) malloc(len);
  assert(header.fileNames);

  fread(header.fileNames,sizeof(string_t),header.numOfFiles,binaryFile);

  fprintf(asciiFile,"%d %d %d\n",header.numOfProcesses,header.numOfFiles,
	                         header.numOfRecords);

  for (i = 0; i < header.numOfFiles; i++)
  {
    int linelen = header.fileNames[i].length;
    fread(line,sizeof(char),linelen,binaryFile);

    fprintf(asciiFile,"%s\n",line);
  }

  free(header.fileNames);
  header.fileNames = NULL;

  return header.numOfRecords;
}


void processRecords(int numRecords)
{
  int i;
  tracerec_t record;

  for (i = 0; i < numRecords; i++)
  {
    fread(&record,sizeof(record),1,binaryFile);
    fprintf(asciiFile,"%s ", mapOpId(record.op));
    fprintf(asciiFile,"%d %d %d ",record.numRecords,record.pid,record.fileId);
    fprintf(asciiFile,"%lf %lf ",record.wallClock,record.processClock);
    fprintf(asciiFile,"%ld %ld\n",record.offset,record.length);
  }
}

void main(int argc, char *argv[])
{
  int numrecs;

  if (argc < 2) 
  {
    fprintf(stderr,"Usage: %s binary-file ascii-file\n");
    exit(1);
  }

  binaryFile = fopen(argv[1],"r");
  if (binaryFile == NULL)
  {
    fprintf(stderr,"Unable to open input file '%s' for reading\n", argv[1]);
    exit(1);
  }

  asciiFile = fopen(argv[2],"w");
  if (asciiFile == NULL)
  {
    fprintf(stderr,"Unable to open output file '%s' for writing\n", argv[2]);
    exit(1);
  }

  numrecs = processHeader();
  processRecords(numrecs);

  fclose(asciiFile);
  fclose(binaryFile);
}
