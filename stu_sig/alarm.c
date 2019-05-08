#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/time.h>

int main(int argc,char* argv[])
{
  //设置定时期
  struct itimerval new_val;
  //第一次触发的时间,这两个值会相加
  new_val.it_value.tv_sec = 2;
  new_val.it_value.tv_usec = 0;
  //设置周期性定时
  new_val.it_interval.tv_sec = 1;
  new_val.it_interval.tv_usec = 0;
  //设置定时法则
  //倒计时两秒
  setitimer(ITIMER_REAL,&new_val,NULL);
  while(1)
  {
    printf("hello setitimer\n");
    sleep(1);
  }
  return 0;
}
