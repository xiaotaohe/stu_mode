#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>

#include<stdlib.h>
#include<string.h>

int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./server [ip] [port]\n");
    return 1;
  }
  //1.创建socket
  int listen_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(listen_sock<0)
  {
    printf("socket\n");
    return 2;
  }
  //2.绑定
  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_addr.s_addr = inet_addr(argv[1]);
  listen_addr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(listen_addr);
  if((bind(listen_sock,(const struct sockaddr*)&listen_addr,len))<0)
  {
    perror("bind!\n");
    return 3;
  }

  //3.改套接字状态为监听状态
  if(listen(listen_sock,5)<0)
  {
    printf("listen!\n");
    return 4;
  }
  //这里才是epoll的重点部分
  //4.多路复用部分
  //创建epoll句柄
  int epfd = epoll_create(200);
  //添加listen套接字到以epfd为根节点的红黑树中
  struct epoll_event ev;
  ev.data.fd = listen_sock;
  ev.events = EPOLLIN;
  //将监听套接字加入到红黑树中
  int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);
  if(ret < 0)
  {
    printf("listen_sock epoll_ctl error!\n");
    return -1;
  }
  //5.循环处理业务
  struct epoll_event all[200];
  while(1)
  {
    ret = epoll_wait(epfd,all,sizeof(all)/sizeof(all[0]),-1);
    //当作到这里，epoll返回触发事件的所有文件描述符，接下来遍历处理每个描述符
    for(int i = 0;i<ret;i++)
    {
      //此时的触发事件的文件描述符分为两种：
      //1.listen_sock 
      //2.client_sock 
      if(all[i].data.fd == listen_sock)
      {
        static struct sockaddr_in client_addr;
        static socklen_t len = sizeof(client_addr); 
        printf("get a new link!\n");
        int client_sock = accept(listen_sock,(struct sockaddr*)&client_addr,&len);
      //将获取的client连接的问价描述符加入到红黑树中
        ev.data.fd=client_sock;
        ev.events = EPOLLIN;
        epoll_ctl(epfd,EPOLL_CTL_ADD,client_sock,&ev);
      }
      //处理客户端文件描述符
      else
      {
        char buf[1024];
        memset(buf,0,1024);
        int fd = all[i].data.fd;
        int sz = read(fd,buf,1024);
        if(sz == 0)
        {
          close(fd);
          epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
          printf("client quit!\n");
          continue;
        }
        else if(sz == -1)
        {
          printf("read error!\n");
          continue;
        }
        else 
        {
          printf("client say:%s\n",buf);
          write(fd,"hello client!",strlen("hello client!"));
        }
      }
    }
  }
}
