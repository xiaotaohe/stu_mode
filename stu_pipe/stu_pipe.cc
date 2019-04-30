/*
 *本段代码的目标是：通过pipe实现管道的功能 例如：ps -aux | grep "bash"命令
 */
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<errno.h>
#include<stdlib.h>

int main()
{
  //1.创建管道
  int fd[2];
  if(pipe(fd)<0)
  {
    perror("pipe perror!\n");
    exit(-1);
  }
  //2.创建父子进程
  pid_t pid;
  if((pid = fork())<0)
  {
    perror("fork error!\n");
    exit(-1);
  }
  //父进程写ps -aux的结果进入管道
  if(pid > 0)
  {
    //关闭读端
    close(fd[0]);
    dup2(fd[1],STDOUT_FILENO);
    execlp("ps","ps","aux",NULL);
    perror("execlp error\n");
    exit(-1);
  }
  //子进程 grep "bash"在父进程写入管道的数据中搜索 "bash"
  if(pid == 0)
  {
    //关闭写端
    close(fd[1]);
    dup2(fd[0],STDIN_FILENO);
    execlp("grep","grep","bash",NULL);
    perror("execlp error\n");
    exit(-1);
  }
  close(fd[0]);
  close(fd[1]);
  return 0;
}

