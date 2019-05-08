/*
 *用子进程杀死父进程
 */
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>

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
    printf("father process %d\n",getpid());
    while(1)
      ;
  }
  else if(pid == 0)
  {
    sleep(2);
    printf("kill father %d\n",getppid());
    kill(getppid(),SIGKILL);
  }
  return 0;
}

