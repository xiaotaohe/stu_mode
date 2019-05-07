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
  //int fd = open("txt",O_RDWR);
  //int fd = open("txt",O_RDONLY);保持fd权限与mmap权限一致
  //int len = lseek(fd,0,SEEK_END);

  //2.创建匿名内存映射区
  void *ptr = mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
  //雅正越界操作
  //void *ptr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,4096);
  //验证创建mmap完成后，关闭fd有没有影响
  //close(fd);
  if(ptr == MAP_FAILED)
  {
    perror("mmap error");
    exit(1);
  }
  strcpy(ptr,"hello mmap");
  printf("%s",(char*)ptr);
  //ptr++;对ptr++是错误操作
  printf("%s",ptr);
  //3.释放内存映射区
  int ret = munmap(ptr,4096);
  if(ret<0)
  {
    perror("munmap error!");
    exit(1);
  }
  //close(fd);
  return 0;
}
