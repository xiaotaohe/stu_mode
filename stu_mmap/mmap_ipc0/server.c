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
  //验证创建mmap完成后，关闭fd有没有影响
  //close(fd);
  //子进程
  //p += strlen(p);
  //printf("++后的结果：%s",p);
  //3.释放内存映射区
  int flag = 0;
  while(1)
  {
    char* p = (char*) ptr;
    p += 1024;
    if(flag == 0)
    {
      strcpy(p,"hello friends!\n");
      flag = 1;
    }
    sleep(4);
  }
  int ret = munmap(ptr,len);
  if(ret<0)
  {
    perror("munmap error!");
    exit(1);
  }
  //close(fd);
  return 0;
}
