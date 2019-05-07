#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<errno.h>

int main()
{
  //1.打开文件
  int fd = open("txt",O_RDWR);
  int len = lseek(fd,0,SEEK_END);

  //2.创建内存映射区
  void *ptr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(ptr == MAP_FAILED)
  {
    perror("mmap error");
    exit(1);
  }
  printf("%s",(char*)ptr);

  //3.释放内存映射区
  munmap(ptr,len);
  close(fd);
  return 0;
}
