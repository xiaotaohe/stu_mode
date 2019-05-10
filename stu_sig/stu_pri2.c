/*
 *目标：学习使用sigaction
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/signal.h>

void myfunc(int nb)
{
  printf("捕捉信号:%d\n",nb);
  sleep(3);
  printf("wake up");
}
int main()
{
  struct sigaction act;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  act.sa_handler = myfunc;
  //当执行handler时临时屏蔽SIGQUIT,直到handler执行完毕才处理该信号
  sigaddset(&act.sa_mask,SIGQUIT);//ctrl+反斜杠
  //捕捉ctrl+c信号
  sigaction(SIGINT,&act,NULL);
  while(1)
    ;
  return 0;
}
