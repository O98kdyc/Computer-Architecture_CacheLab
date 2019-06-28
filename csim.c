#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <strings.h>

typedef struct {
    int empty;
    int valid;
    long long visit_time;
    unsigned long long tag;
} Block;

typedef struct {
    int BlockNum;
    Block *block_list;
} Set;

typedef struct{
	Set *set_list;
	int hits;
	int misses;
	int evictions;
} Cache;

//Calculate Block
int CalBlock(int blocksize, int addr)
{
    return addr>>blocksize;
}

//Calculate Set
int CalSet(int setsize, int blocknum)
{
    int tmp=blocknum>>setsize;
    tmp=tmp<<setsize;
    return(blocknum-tmp);
}

int GetTag(int addr, int setsize, int blocksize)
{
    return (unsigned int)addr>>(setsize+blocksize);
}

void init(int setsize, int lineperset, int blocksize,Cache *Cache)
{
    Cache->hits=0;
    Cache->misses=0;
    Cache->evictions=0;
    Cache->set_list = (Set*) malloc (sizeof(Set) * pow(2,setsize)); //pow(2,setsize)=nunber of sets
    for(int i=0;i<pow(2,setsize);i++)
    {
      Cache->set_list[i].BlockNum=0;
      Cache->set_list[i].block_list=(Block* ) malloc (sizeof(Block) * lineperset);
      for(int j=0;j<lineperset;j++)
      {
        Cache->set_list[i].block_list[j].empty=1;
        Cache->set_list[i].block_list[j].valid=0;
      }
    }
}

void sim(int setsize, int lineperset, int blocksize, int addr, Cache *Cache, int print)
{
    int blocknum=CalBlock(blocksize,addr);
    int setnum=CalSet(setsize,blocknum);
    int Tag=GetTag(addr,setsize,blocksize);
    int isFind=0,tmp_pos=0;

    //All visit_time++ in the current set and Find
    for(int i=0;i<lineperset;i++)
    {
        //visit_time++
        if(Cache->set_list[setnum].block_list[i].valid) Cache->set_list[setnum].block_list[i].visit_time++;
        //Find
        if(Cache->set_list[setnum].block_list[i].tag==Tag&&Cache->set_list[setnum].block_list[i].valid)
        {
          isFind=1;
          Cache->set_list[setnum].block_list[i].visit_time=1;
        }
        //find empty
        if(Cache->set_list[setnum].block_list[i].empty)
        {
          tmp_pos=i;
        }
    }

    if(isFind)//founded
    {
        Cache->hits++;
        if(print) printf("hit ");
        return;
    }

    //not founded
    Cache->misses++;
    if(print) printf("miss ");
    //exisit empty
    if(Cache->set_list[setnum].BlockNum<lineperset)
    {
       Cache->set_list[setnum].BlockNum++;
       //Find an empty Block and fill it
       Cache->set_list[setnum].block_list[tmp_pos].valid=1;
       Cache->set_list[setnum].block_list[tmp_pos].empty=0;
       Cache->set_list[setnum].block_list[tmp_pos].tag=Tag;
       Cache->set_list[setnum].block_list[tmp_pos].visit_time=1;
    }
    //no empty
    else
    {
       int max_time=Cache->set_list[setnum].block_list[0].visit_time;
       int pos=0;
      //Evict one
       for(int i=0;i<lineperset;i++)
       {
          if(max_time<Cache->set_list[setnum].block_list[i].visit_time)
          {
            max_time=Cache->set_list[setnum].block_list[i].visit_time;
            pos=i;
          }
       }
       //replace
       Cache->evictions++;
       if(print) printf("eviction ");
       Cache->set_list[setnum].block_list[pos].valid=1;
       Cache->set_list[setnum].block_list[pos].empty=0;
       Cache->set_list[setnum].block_list[pos].tag=Tag;
       Cache->set_list[setnum].block_list[pos].visit_time=1;
    }
}

void printsummary(int x, int y, int z)
{
   printf("hits:%d  ",x);
   printf("misses:%d  ",y);
   printf("evictions:%d\n",z);
}

int main(int argc,char **argv)
{
    FILE *trace_file;
    char *trace_name;
    Cache Cache;
    int verbose_flag = 0;
    char cmd_char;
    int setsize,lineperset,blocksize;
    cmd_char = getopt(argc,argv, "s:E:b:t:v") ;
    while(cmd_char!=-1)
   {
switch(cmd_char)
{
case 's':
setsize = atol(optarg) ;
break;

case 'E':
lineperset = atol(optarg) ;
break;

case 'b':
blocksize = atol(optarg) ;
break;

case 't':
trace_name = optarg;
break;

case 'v':
verbose_flag = 1; 
break;

default:
printf("wrong command!\n");
exit(1);
}
   cmd_char = getopt(argc,argv, "s:E:b:t:vh") ;
   }
    init(setsize,lineperset,blocksize,&Cache);
    trace_file  = fopen(trace_name, "r");
    char trace_char;
    long long addr;
    int size=0;
    if(trace_file!=NULL)
{
int readnumber=fscanf(trace_file," %c %llx,%d",&trace_char,&addr,&size);
while(readnumber==3)
{
switch(trace_char)
 {
case 'I':
if(verbose_flag) printf(" %c %llx,%d\n",trace_char,addr,size);
break;
case 'M':
if(verbose_flag) printf("%c %llx,%d\t",trace_char,addr,size);
for(int i=0;i<2;i++)
 sim(setsize,lineperset,blocksize,addr,&Cache,verbose_flag);
if(verbose_flag) printf("\n");
break;
case 'S':case'L':
if(verbose_flag) printf("%c %llx,%d\t",trace_char,addr,size);
 sim(setsize,lineperset,blocksize,addr,&Cache,verbose_flag);
if(verbose_flag) printf("\n");
 }
readnumber=fscanf(trace_file," %c %llx,%d",&trace_char,&addr,&size);
}
}
else
{
printf("error reading trace file!\n");
exit(0);
}
    printSummary(Cache.hits,Cache.misses,Cache.evictions);
    return 0;
}
