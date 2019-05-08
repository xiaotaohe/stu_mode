/*
 *1.子进程自杀
 *2.父进程判断子进程是被那个信号干掉的
 */
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
#include<wait.h>

int main()
{
  pid_t pid = fork();
  if(pid<0)
  {
    perror("fork error");
    exit(1);
  }
  if(pid>0)
  {
    int s;
    printf("father process %d\n",getpid());
    printf("child died pid = %d\n",pid);
    wait(NULL);
    if(WIFSIGNALED(s))
    {
      printf("died by signal %d\n",WTERMSIG(s));
    }
  }
  else if(pid == 0)
  {
    //自己给自己发信号
    raise(SIGINT);
  }
  return 0;
}

