/*
 *本段代码目标：创建3个子进程，分别执行：ps 普通程序 段错误程序
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>

int main()
{
  pid_t pid = 0;
  int i = 0;
  //1.循环创建3个子进程
  for(i = 0;i<3;i++)
  {
    if((pid = fork()) == 0)
      break;
  }
  //思考用continue可以吗？
  //答案不可以，第一个子进程第一次会逃过，第二次不会跳过，此时便会出现创建多个进程。
  //2.判断每个进程是那个进程
  if(i == 0)
  {
    //执行ps程序
    execlp("ps","ps",NULL);
    perror("execlp ps");
    exit(-1);
  }
  if(i == 1)
  {
    //执行普通程序
    execlp("./temp/hello","hello",NULL);
    perror("execlp hello");
    exit(-1);
  }
  if(i == 2)
  {
    //执行段错误程序
    execlp("./temp/error","error",NULL);
    perror("execlp error");
    exit(-1);
  }
  if(i == 3)
  {
    int stute = 0;
    pid_t pid = 0;
    while((pid = wait(&stute))!=-1)
    {
      printf("this is process %d exit\n",pid);
      if(WIFEXITED(stute))
      {
        printf("retrue value %d\n",WEXITSTATUS(stute));
      }
      if(WIFSIGNALED(stute))
      {
        printf("died by signal %d\n",WTERMSIG(stute));
      }
    }
  }
  return 0;
}
