#include "FastQ.h"
#include <list>
#include <thread>
#include <istream>
#include <fstream>
#include <time.h>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>



void readFread(const char* fileName)
{
  std::string line;
  std::ifstream myfile (fileName);
  if (myfile.is_open())
  {
    size_t count = 0;
    while ( getline (myfile,line) )
    {
      //   if (count % 100000 == 0) { std::cout << count << '\n'; }
      ++count;
    }
    myfile.close();
    //code goes here
  }
}

void readMemMap(const char* fileName, bool first_half)
{
  unsigned char* read;
  unsigned int length = 6553;
  int fd = -1;
  char* result;
  struct stat buf;
  std::cout << fileName << std::endl;

  fd = open(fileName, O_RDONLY);
  if (fd < 0)
  {
    std::cout << "error while opening file" << std::endl;
    return;;
  }

  if (fstat(fd, &buf) < 0)
  {
    std::cout << "Error while determining file length" << std::endl;
    return;
  }

  size_t len = (size_t)buf.st_size;
  size_t len_half = len / 2;
  size_t limit = (first_half) ? len_half : len;

  result = (char*)mmap(NULL, len*sizeof(int), PROT_READ,MAP_FILE|MAP_PRIVATE,fd,0);
  size_t count = (first_half) ? 0 : len_half;
  size_t lineCount = 0;
  size_t alleleCount = 0;
  std::string test = "AATCAAAAATAA";
  const char* test_ptr = test.c_str();
  size_t test_size = test.size();
  std::list<unsigned long> allele_points;
  while (count < limit)
  {
    if (std::memcmp(test_ptr, &result[count], test_size) == 0)
    {
      allele_points.push_back((unsigned long)&result[count]);
      ++alleleCount;
      count += test_size;
      continue;
    }
    else
    {
      ++count;
    }
    if (std::memcmp("\n", &result[count], 1) == 0) { ++lineCount; }
    //    if (count % 1000000 == 0) { std::cout << "-- " << count << std::endl; }
  }

  std::cout << "There are " << lineCount << " lines in this file" << std::endl;
  std::cout << "There are " << alleleCount << " alleles in this file" << std::endl;
  printf("The whole file has been read.\n");
}

int main()
{
  char fileName[] = "/Users/dillonl/Projects/afilereader/data/SRR062640_1.filt.fastq";
  clock_t t1,t2;
  t1=clock();

  std::thread thread1(readMemMap, fileName, true);
  std::thread thread2(readMemMap, fileName, false);

  thread1.join();
  thread2.join();

  //readMemMap(fileName);
  //readFread(fileName);

  t2=clock();
  float diff ((float)t2-(float)t1);
  float seconds = diff / CLOCKS_PER_SEC;
  std::cout << "EOF: " << seconds << std::endl;

  return 0;
}
