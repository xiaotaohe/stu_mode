#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

/*
 * 测试一秒钟笔记本可以数多少数
 */

int main()
{
  alarm(1);
  int i = 0;
  while(1)
  {
    i++;
    printf("%d\n",i);
  }
}
