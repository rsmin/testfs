/* 
 * asc2bin: converts ascii trace to binary
 * author: Anurag Acharya, acha@cs.umd.edu 
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include "iotrace.h"

FILE *asciiFile;
FILE *binaryFile;
char line[1024];

int processHeader()
{
  traceHeader_t header;
  int i;
  int len;

  /* parse the header */

  fscanf(asciiFile,"%d",&(header.numOfProcesses));
  assert(header.numOfProcesses > 0);

  fscanf(asciiFile,"%d",&(header.numOfFiles));
  assert(header.numOfFiles > 0);

  fscanf(asciiFile,"%d\n",&(header.numOfRecords));
  assert(header.numOfRecords > 0);

  len = sizeof(string_t)*header.numOfFiles;
  header.fileNames = (string_t *) malloc(len);
  assert(header.fileNames);

  for (i = 0; i < header.numOfFiles; i++)
  {
    int linelen;

    fscanf(asciiFile,"%d %s\n", &linelen, line);
    linelen++;
    assert(linelen > 1);

    /* round up linelen to the nearest  alignment */
    linelen = ((linelen + (SALIGN - 1))/SALIGN) * SALIGN;
    
    header.fileNames[i].length = linelen;
    header.fileNames[i].name   = (char *) malloc(linelen);
    assert(header.fileNames[i].name);
    memcpy(header.fileNames[i].name,line,linelen);
    len += linelen;
  }

  header.offsetToTraceRecords = len;

  /* dump the header */
  fwrite(&header,sizeof(header),1,binaryFile);
  fwrite(header.fileNames,sizeof(string_t),header.numOfFiles,binaryFile);

  for (i = 0; i < header.numOfFiles; i++)
  {
    int linelen = header.fileNames[i].length;

    fwrite(header.fileNames[i].name,sizeof(char),linelen,binaryFile);
    free(header.fileNames[i].name);
    header.fileNames[i].name = NULL;
  }

  free(header.fileNames);
  header.fileNames = NULL;

  return header.numOfRecords;
}


int processRecords(int numRecords)
{
  int i;
  int ret;
  int recsWritten;
  char inrec[1024];
  tracerec_t record;

  recsWritten = 0;
  while (fgets(inrec, 1024, asciiFile))
    {
      record.op = atoi(strtok(inrec, " "));

      record.numRecords = atoi(strtok(NULL, " "));
      if (record.op == LISTIO_HEADER)
	{assert(record.numRecords >= 1);}
      else
	{assert(record.numRecords == 1);}
      
      record.pid        = atoi(strtok(NULL, " "));
      assert(record.pid >= 0);

      record.fileId     = atoi(strtok(NULL, " "));
      assert(record.fileId >= 0);
      
      record.wallClock  = atof(strtok(NULL, " "));
      assert(record.wallClock >= 0.0L);

      record.processClock = atof(strtok(NULL, " "));      
      assert(record.processClock >= 0.0L);

      record.offset = atol(strtok(NULL, " "));
      assert(record.offset >= 0L);

      record.length = atol(strtok(NULL, " \n"));
      assert(record.length >= 0);

      /* everything is kosher, dump the record to file */
      fwrite(&record,sizeof(record),1,binaryFile);
      recsWritten++;
    }

  return recsWritten;
}

void main(int argc, char *argv[])
{
  int numrecs, outrecs;

  if (argc < 2) 
  {
    fprintf(stderr,"Usage: %s ascii-file binary-file\n",argv[0]);
    exit(1);
  }

  asciiFile = fopen(argv[1],"r");
  if (asciiFile == NULL)
  {
    fprintf(stderr,"Unable to open input file '%s' for reading\n", argv[1]);
    exit(1);
  }

  binaryFile = fopen(argv[2],"w");
  if (binaryFile == NULL)
  {
    fprintf(stderr,"Unable to open output file '%s' for writing\n", argv[2]);
    exit(1);
  }

  numrecs = processHeader();
  outrecs = processRecords(numrecs);

  fclose(asciiFile);
  fclose(binaryFile);

  /* patch numRecs field if necessary (used by httpd) - Mustafa */ 
  if (numrecs != outrecs)
    {
      int fd = open(argv[2], O_RDWR);
      lseek(fd, 2*sizeof(int), SEEK_SET);
      write(fd, &outrecs, sizeof(int));
      close(fd);
    }
}



