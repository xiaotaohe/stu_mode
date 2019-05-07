#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<errno.h>
#include<wait.h>

int main()
{
  //1.打开文件
  int fd = open("txt",O_RDWR);
  ftruncate(fd,4096);
  //int fd = open("txt",O_RDONLY);保持fd权限与mmap权限一致
  int len = lseek(fd,0,SEEK_END);

  //2.创建内存映射区
  void *ptr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  //雅正越界操作
  //void *ptr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,4096);
  if(ptr == MAP_FAILED)
  {
    perror("mmap error");
    exit(1);
  }
  while(1)
  {
    sleep(1);
    printf("%s\n",(char*)ptr+1024);
  }
  int ret = munmap(ptr,len);
  if(ret<0)
  {
    perror("munmap error!");
    exit(1);
  }
  close(fd);
  return 0;
}
