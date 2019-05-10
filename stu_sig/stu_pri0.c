/*
 * 目标：信号阻塞
 *手动屏蔽信号
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
  //1.自定义信号集合
  sigset_t myset;
  //2.清空集合
  sigemptyset(&myset);
  //3.添加要屏蔽的信号
  sigaddset(&myset,SIGINT);//ctrl+c
  sigaddset(&myset,SIGQUIT);//ctrl+反斜杠
  sigaddset(&myset,SIGKILL);

  //4.自定义信号集数据设置给内核阻塞信号集
  sigprocmask(SIG_BLOCK,&myset,NULL);
  while(1)
  {
    sigset_t pendset;
    //获取内核pending集合
    sigpending(&pendset);
    sleep(1);
    for(int i = 1;i<=31;i++)
    {
      if(sigismember(&pendset,i))
        printf("1");
      else 
        printf("0");

    }   
    printf("\n");
  }
  return 0;
}
