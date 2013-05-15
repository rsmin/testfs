/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/StoreCache.cc,v 1.4 2002/02/18 00:23:45 tmwong Exp $
  Author:       T.M. Wong <tmwong+@cs.cmu.edu>
*/
 
#include <list>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */
#include <stdio.h>
#include <stdlib.h>

#include "Block.hh"
#include "Char.hh"
#include "IORequest.hh"
#include "Store.hh"

#include "StoreCache.hh"
#include "util.hh"

using Block::block_t;

  extern char mod[STATNUM+1][20];

  extern char dis_char[MAXDIS][10];
  extern char access2[MAXACC][10];

bool
StoreCache::IORequestDown(const IORequest& inIOReq)
{
  block_t block = {inIOReq.objIDGet(), inIOReq.blockOffGet(blockSize)};
  list<IORequest> newIOReqs;

  // Log incoming request if desired.

  if (logRequestFlag) {
    printf("%lf %llu %llu %llu\n",
	   inIOReq.timeIssuedGet(),
	   inIOReq.objIDGet(),
	   inIOReq.offGet(),
	   inIOReq.lenGet());
  }

  // See if each block is cached; if not, make space for it and send an I/O
  // request to the lower-level storage device.

  if ((nextStore)&&(size == 0))
  {
       nextStore->IORequestDown(inIOReq);
  }
  else
  {
    for (uint64_t i = 0; i < inIOReq.blockLenGet(blockSize); i++) {
      if ((!nextStore)&&(mode == Analyze))
      {
            UpdateRefDistance(inIOReq, block);            
            if (i==0)   
                  UpdateReqLens(inIOReq.blockLenGet(blockSize));
      }
      else
	    BlockCache(inIOReq, block, newIOReqs);
      block.blockID++;
    }
  }
  // Send I/O requests to the next storage device.

/*  if (nextStore) {
    for (list<IORequest>::iterator i = newIOReqs.begin();
	 i != newIOReqs.end();
	 i++) {

      nextStore->IORequestDown(*i);
    }
  }*/

  return (true);
}

void StoreCache::UpdateReqLens(uint64_t lens)
{
     if (lens > 0)
       lens = log2(lens) + 1;
    
     if (lens > (MAXACC - 1))
        lens = MAXACC - 1;
     reqlens[lens]++;
}


void StoreCache::UpdateDemote(const IORequest& inIOReq, const Block::block_t& inBlock)
{

  LocalBlock::local_block_t b;
 
  b.data.objID = inBlock.objID;
  b.data.blockID = inBlock.blockID;
  int j;

  if (demotetime == 0)
      demotetime = time;
  else
      demotetime++;

      bool upCached = isUpCached(inBlock);
      
      if (upCached)
      {
         j = 1;
      }
      else
      {
         j = 2;
      }

     
      if (j == 2)
       ds.blockDemote(inBlock, demotetime);
}

void StoreCache::UpdateRead(const IORequest& inIOReq, const Block::block_t& inBlock)
{

  LocalBlock::local_block_t b;

  b.data.objID = inBlock.objID;
  b.data.blockID = inBlock.blockID;
  int j;

 time ++;

 uint64_t *worktime = &time;


  worktime = &time ;
  j = 0;

  if (request[j]->isCached(b))
  {
    request[j]->blockGet(b);

    uint64_t distance = *worktime - b.refTime;

    if (distance > 0)
    {
         distance =  distance/16;
         if (distance > 0)
             distance = log2(distance) + 2;
          else 
             distance = 1;
    }

    if (distance > (MAXDIS - 1))
        distance = MAXDIS - 1;
    req_distance[j][distance]++;


      b.refNum++;
      b.refTime = *worktime;
      request[j]->blockPutAtTail (b);

      LocalBlock::local_block_t c = b;
   
      if (req_dis_block[j][distance]->isCached(c))
      {
        req_dis_block[j][distance]->blockGet(c);
        c.refNum++;
      }
      else
        c.refNum = 1;
      req_dis_block[j][distance]->blockPutAtTail (c);

    req_total[j][1]++;
  }
  else
  {
      b.refNum = 1;
      b.refTime = *worktime;
      request[j]->blockPutAtTail (b);
  } 

       ds.blockRead(inBlock, demotetime);
}


void StoreCache::UpdateRefDistance(const IORequest& inIOReq, const Block::block_t& inBlock)
{

  if (inIOReq.opGet() == IORequest::Demote)
      UpdateDemote (inIOReq, inBlock);
  else 
      UpdateRead (inIOReq, inBlock);

}

void StoreCache::UpdateTotalRef()
{
  uint64_t ref;
  int j = 0;

    UpdateRef (request[j],&(req_access[j]));
    AccumulateRef ( &req_access[j], &req_total_access[j]);
    CalcPerc( &req_total_access[j],&access_perc[j]);

    for (int k = 0; k < MAXDIS; k++)
    {
         UpdateRef(req_dis_block[j][k], &req_dis_access[j][k], true);
         AccumulateRef ( &req_dis_access[j][k], &req_dis_total_access[j][k]);
    }

  CalcLensPerc();

  ds.Calc();
}

void StoreCache::CalcLensPerc()
{
 uint64_t total = 0;

 for (int k=0; k < MAXACC; k++)
 {
    if (reqlens[k])
     total += reqlens[k];
 }

 for (int k=0; k < MAXACC; k++)
 {
   if (reqlens[k])
        reqlens_perc[k] = (double)(reqlens[k])/(double)total;
 }
}

void StoreCache::UpdateRef(BlockBuffer *history, uint64_t (*req_access)[MAXACC][2], bool cali)
{
  LocalBlock::local_block_t b; 

  uint64_t ref;

  while (!history->isEmpty())
  {
     history->blockGetAtHead(b);
     ref = b.refNum;
     if (cali)
         ref++;

     if (ref > 0)
       ref = log2(ref) + 1;
    
     if (ref > (MAXACC - 1))
        ref = MAXACC - 1;
     (*req_access)[ref][0]++;
     (*req_access)[ref][1] = (*req_access)[ref][1] + b.refNum;
   }
}

void StoreCache::AccumulateRef(uint64_t (*req_access)[MAXACC][2], 
            uint64_t (*req_accu_access)[MAXACC][2])
{
         for (int p = MAXACC - 1; p >= 0; p--)
         {
            (*req_accu_access)[p][0] += (*req_access)[p][0];
            (*req_accu_access)[p][1] += (*req_access)[p][1];
            if (p < MAXACC - 1)
            {
               (*req_accu_access)[p][0] += (*req_accu_access)[p+1][0];
               (*req_accu_access)[p][1] += (*req_accu_access)[p+1][1];
            }
         }
}

void StoreCache::CalcPerc(uint64_t (*req_total_access)[MAXACC][2], 
            double (*access_perc)[MAXACC][2])
{
 for (int k=0; k < MAXACC; k++)
 {
   if ((*req_total_access)[0][0])
        (*access_perc)[k][0] = (double)(*req_total_access)[k][0]/  \
           (double)(*req_total_access)[0][0];
   if ((*req_total_access)[0][1])
        (*access_perc)[k][1] = (double)(*req_total_access)[k][1]/     \
          (double)(*req_total_access)[0][1];  
 }
}


void
StoreCache::statisticsReset()
{
  demoteHitsPerOrig.clear();
  demoteMissesPerOrig.clear();

  readHitsPerOrig.clear();
  readMissesPerOrig.clear();

  coopHitsPerOrig.clear();

  Store::statisticsReset();
}

void
StoreCache::statisticsShow() const
{
   if (mode != Analyze)
      simulationShow();
   else
     if (!(nextStore))
        analyzeShow();
}

void
StoreCache::simulationShow() const
{
  printf("\t{readHitsPerOrig ");
  for (Char::Counter::const_iterator i = readHitsPerOrig.begin();
       i != readHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");
  printf("\t{readMissesPerOrig ");
  for (Char::Counter::const_iterator i = readMissesPerOrig.begin();
       i != readMissesPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{read {hits=%llu} {misses=%llu} }\n", readHits, readMisses);

  printf("\t{demoteHitsPerOrig ");
  for (Char::Counter::const_iterator i = demoteHitsPerOrig.begin();
       i != demoteHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");
  printf("\t{demoteMissesPerOrig ");
  for (Char::Counter::const_iterator i = demoteMissesPerOrig.begin();
       i != demoteMissesPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{demote {hits=%llu} {misses=%llu} }\n", demoteHits, demoteMisses);

  printf("\t{coopHitsPerOrigPerOrig ");
  for (Char::Counter::const_iterator i = coopHitsPerOrig.begin();
       i != coopHitsPerOrig.end();
       i++) {
    printf("{%s=%llu} ", i->first, i->second);
  }
  printf("}\n");

  printf("\t{cooperative {hits=%llu} }\n", cooperativeHits);


  printf("}\n");
}



void
StoreCache::analyzeShow() const
{
  printf("\n");
  distanceShow();
  freqShow();
  freqSpecShow();
  demotefreqShow();
  reqlensShow();
}

void
StoreCache::reqlensShow() const
{
  int j = 0;

    
       printf("\tReqlens:\n");

       printf("\tLens\tNumber of Req\tPerc\n");
 
       for (int i = 0; i < MAXACC; i++)
          if (reqlens[i] > 0)
          {
             printf("\t%s\t%llu\t%lf\n",
              access2[i],reqlens[i],reqlens_perc[i]); 
          }
       printf("\n"); 

}

void
StoreCache::distanceShow() const
{
  int j = 0;  

     if ((req_total[j][1] + req_total[j][0])<= 0)
         return;
 
     printf("\t%s: \n", mod[j]);
     if (j != 0)
     {
        printf("\tAccesses\tHits\n");
        printf("\t%llu\t%llu\n",req_total[j][0],req_total[j][1]);
     }
     printf("\tDistance\tNumber of Accesses\n");     
     for (int i = 0; i < MAXDIS; i++)
     {
        if (req_distance[j][i] > 0)
           printf("\t%s\t%llu\n",dis_char[i],req_distance[j][i]);
     }
     printf("\n");

}


void
StoreCache::freqShow() const
{

  int j = 0;

       if (!((req_total_access[j][0][0] > 0)||(req_total_access[j][0][1] > 0)))
          return;
     
       printf("\t%s:\n", mod[j]);

       printf("\tFrequency\tNumber of Blocks\tNumber of Accesses\tTotal Number of Blocks\tTotal Number of Accesses\tPercentage of Blocks\tPercentage of Accesses\n");
 
       for (int i = 0; i < MAXACC; i++)
          if ((req_access[j][i][0] > 0)||(req_access[j][i][1] > 0))
          {
             printf("\t%s\t%llu\t%llu\t%llu\t%llu\t%lf\t%lf\n",
              access2[i],req_access[j][i][0],  req_access[j][i][1], 
              req_total_access[j][i][0], req_total_access[j][i][1],
              access_perc[j][i][0],access_perc[j][i][1]); 
          }
       printf("\n");

}

void
StoreCache::freqSpecShow() const
{
  int j = 0;

    if (!((req_total_access[j][0][0] > 0)||(req_total_access[j][0][1] > 0)))
        return;
    printf("\t%s: \n", mod[j]);
    printf("\tFrequency\tDistance\tNumber of Accesses\tTotal Number of Accesses\n");
    for (int k = 0; k < MAXACC; k++)
      for (int i = 0; i < MAXDIS; i++)
      {
         if (req_dis_access[j][i][k][1] > 0)
            printf("\t%s\t%s\t%llu\t%llu\n",access2[k],dis_char[i],   \
               req_dis_access[j][i][k][1], req_dis_total_access[j][i][k][1]);
      }
     printf("\n"); 
     printf("\n");

}
void
StoreCache::demotefreqShow() const
{
      if (ds.blockNum <= 0)
          return ;


     // printf total demote distance

     printf("\t Total Demote Distance\n");

     printf("\tDistance\tNumber of Accesses\n");     
     for (int i = 0; i < MAXDIS; i++)
     {
        if (ds.demote_distance[i] > 0)
           printf("\t%s\t%llu\n",dis_char[i], ds.demote_distance[i]);
     }
     printf("\n"); 

     // printf access specific demote distance

     printf("\t Access Specific Demote Distance\n"); 

       printf("\tblock number\tdemote number\thit number\tmiss number\thit perc\n");
       printf("\t%llu\t%llu\t%llu\t%llu\t%lf\n\n", 
         ds.blockNum, ds.demoteNum, ds.hitNum, ds.missNum, ds.hitPerc);

       printf("\tFrequency\t# of Blocks\t# of ALL Hit Blocks\t# of One Miss Blocks\t# of Demotes\t# of Hits\t# of Misses\tPerc of local Hits\tPerc of Blocks\tPerc of Demotes\tPerc of Hits\tPerc of Misses\tD-R Distance Mean\tD-R Variance\tR-D Mean\tR-D Variance\n");
 
       for (int i = 0; i < MAXACC; i++)
          if (ds.freq_stat[i][0] > 0)
          {
            printf("\t%s\t%llu\t%llu\t%llu\t%llu\t%llu\t%llu\t%lf\t%lf\t%lf\t%lf\t%lf\t%s\t%s\t%s\t%s\n",
               access2[i], ds.freq_stat[i][0], ds.freq_stat[i][1], ds.freq_stat[i][2],
               ds.freq_stat[i][3], ds.freq_stat[i][4], ds.freq_stat[i][5], ds.freq_perc[i][4],
               ds.freq_perc[i][0], ds.freq_perc[i][1], ds.freq_perc[i][2], ds.freq_perc[i][3],
               dis_char[ds.freq_dist_mv[i][0]], dis_char[ds.freq_dist_mv[i][1]],
               dis_char[ds.freq_dist_mv[i][2]], dis_char[ds.freq_dist_mv[i][3]]);
          }
       printf("\n");

       printf("\tFreq\tDistance\tD-R\tR-D\n");
       for (int i = 0; i < MAXACC; i++)
       {
          if (ds.freq_stat[i][0] > 0)
          {
            for (int j = 0; j < MAXDIS; j++)
              if ((ds.freq_dist_spec[i][j][0] > 0)||(ds.freq_dist_spec[i][j][1] > 0))
               printf("\t%s\t%s\t%llu\t%llu\n",
                 access2[i], dis_char[j], ds.freq_dist_spec[i][j][0],ds.freq_dist_spec[i][j][1]);
          }
       }
       printf("\n");
//       crdemotedistShow();
}

void
StoreCache::crdemotedistShow() const
{
       printf("\tPrev Distance\tDistance\tNumber\n");

         for (int j = 0; j < MAXDIS; j++)
         {
           for (int k = 0; k < MAXDIS; k++)
           { 
              if (ds.d_r_dist[j][k][0] > 0)
                printf("\t%s\t%s\t%llu\n",
                  dis_char[j], dis_char[k],ds.d_r_dist[j][k][0]);
           }
         }

       printf("\n\n\tPrev R-D Distance\tDistance\tNumber\n");

         for (int j = 0; j < MAXDIS; j++)
         {
           for (int k = 0; k < MAXDIS; k++)
           { 
              if (ds.d_r_dist[j][k][1] > 0)
                printf("\t%s\t%s\t%llu\n",
                  dis_char[j], dis_char[k],ds.d_r_dist[j][k][1]);
           }
         }

}


void StoreCache::beforeShow()
{
   if (!(nextStore)&&(mode == Analyze))
      UpdateTotalRef();
}

void StoreCache::init()
{

    for (int j=0; j < STATNUM+1; j++)
    {
       for (int i=0; i < MAXDIS; i++)
          req_distance[j][i] = 0;
       req_total[j][0] = 0;
       req_total[j][1] = 0;
    }

    for (int j=0; j < STATNUM; j++)
    {
       for (int i=0; i < MAXDIS; i++)
       {
         req_dis_block[j][i] = new BlockBuffer(1000000000);
         for (int k=0; k < MAXACC; k++)
         {
            req_dis_access[j][i][k][0] = 0;
            req_dis_access[j][i][k][1] = 0;
            req_dis_total_access[j][i][k][0] = 0;
            req_dis_total_access[j][i][k][1] = 0;
         }
       }
       request[j] = new BlockBuffer(1000000000);
    }   

    for (int j=0; j < STATNUM; j++)
      for (int i=0; i < MAXACC; i++)
         for (int k=0; k < 2; k++)
         {
           req_access[j][i][k] = 0;
           req_total_access[j][i][k] = 0;
           access_perc[j][i][k] = 0.0;
         }

   for (int i=0; i < MAXACC; i++)
   {
         reqlens[i]=0; 
         reqlens_perc[i]=0;
   }
}

StoreCache::~StoreCache() 
{
    for (int j=0; j < STATNUM; j++)
    {
       for (int i=0; i < MAXDIS; i++)
       {
          if (req_dis_block[j][i])
              delete req_dis_block[j][i];
       }
       if (request[j])
          delete req_dis_block[j];
    }          

}

