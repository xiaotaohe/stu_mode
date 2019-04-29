#include<stdio.h>
#include<unistd.h>

/*
 *本段目标：1.代码实现循环创建多个进程
 *2.并判断是那个子进程
 */
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
    printf("I am first pid = %d fork!\n",getpid());
  if(i == 1)
    printf("I am second pid = %d fork!\n",getpid());
  if(i == 2)
    printf("I am three pid = %d fork!\n",getpid());
  if(i == 3)
    printf("I am father pid = %d fork!\n",getpid());
  return 0;
}
