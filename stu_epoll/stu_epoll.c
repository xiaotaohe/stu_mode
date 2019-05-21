#include<stdio.h>
#include<stdlib.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<sys/epoll.h>

/*
 *目标：简单实用epoll
 */

int main()
{
  int epfd = epoll_create(20);
  struct epoll_event event;
  event.data.fd = 0;
  event.events = EPOLLIN;
  epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
  struct epoll_event all[20];
  while(1)
  {
    int ret = epoll_wait(epfd,all,sizeof(all)/sizeof(all[0]),-1);
    for(int i = 0;i<ret;i++)
    {
      char buf[1024];
      memset(buf,0,sizeof(buf));
      int fd = all[i].data.fd;
      read(fd,buf,1024);
      printf("%s",buf);
    }
  }
}
 
