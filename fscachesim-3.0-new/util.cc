
#include <util.hh>



char mod[STATNUM+1][20] = {"Read","DemoteAny","DemoteWithout","DemoteAny2Without"};

char dis_char[MAXDIS][10] = {"0","1","16", "32","64","128","256","512","1K", \
                                 "2K","4K","8K","16K","32K", \
                               "64K","128K","256K","512K","1M","2M","4M","8M","16M","32M", \
                               "64M","128M","256M","512M","1G",   \
                               "2G","4G" ,"8G"};
char access2[MAXACC][10] = {"0","1","2","4", "8","16","32","64","128","256","512","1K", \
                                 "2K","4K","8K","16K","32K"};



uint64_t log2(uint64_t distance)
{
 if (distance > 0 )
 {
    uint64_t d = 0;
    while (1)
    {
      d++;
      distance = distance>>1;
      if (distance == 0)
      {
        distance = d;
        break;
      }
    }
    distance = distance - 1;
 }
 return distance;
}
