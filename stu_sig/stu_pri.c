/*
 *目标：打印未决信号
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/signal.h>

int main()
{
  //每个一秒钟读取一次
  while(1)
  {
    //获取未决信号集
    sigset_t pendset;
    sigpending(&pendset);
    for(int i = 1;i<=31;i++)
    {
      //对每个信号依次判断
      if(sigismember(&pendset,i))
      {
        printf("1");
      }
      else 
      {
        printf("0");
      }
    }
    printf("\n");
    sleep(1);
  }
  return 0;
}
