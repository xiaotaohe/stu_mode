/*
 *目标：使用信号捕捉，捕捉ctrl+c
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/signal.h>

void handler(int nb)
{
  printf("捕捉到ctrl+c信号:%d\n",nb);
}
int main()
{
  //捕捉ctrl+c
  //1.注册捕捉函数
  signal(SIGINT,handler); 
  while(1)
  {
    sleep(1);
    printf("hello!\n");
    //signal(SIGINT,&handler); 
  }
  return 0;
}
