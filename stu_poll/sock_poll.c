#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/poll.h>
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
  //4.poll的核心代码
  struct pollfd all[1024];
  //将listen_sock加入到数组中
  all[0].fd = listen_sock;
  all[0].events = POLLIN;
  for(int i = 1;i<1024;i++)
  {
    all[i].fd = -1;
  }
  int index = 0;
  while(1)
  {
    int ret = poll(all,index+1,-1);
    if(ret < 0)
    {
      perror("poll error");
      exit(1);
    }
    if(all[0].revents & POLLIN)
    {
      struct sockaddr_in client_addr;
      socklen_t len = sizeof(client_addr);
      int client_sock = accept(listen_sock,(struct sockaddr*)&client_addr,&len);
      //将client_sock加入到数组中
      for(int i = 1;i<1024;i++)
      {
        if(all[i].fd == -1)
        {
          all[i].fd = client_sock;
          all[i].events = POLLIN;
          //可能会更新index
          if(i>index)
            index = i;
          break;
        }
      }
    }
    //开始对client进行服务,遍历
    for(int i = 1;i<index+1;i++)
    {
      char buf[1024];
      int cfd = all[i].fd;
      if(cfd == -1)
        continue;
      if(all[i].revents & POLLIN)//开始服务
      {
        int sz = read(cfd,buf,1023);
        if(sz < 0)
        {
          perror("recv error\n");
          exit(1);
        }
        else if(sz == 0)
        {
          printf("client quit!\n");
          close(cfd);
          all[i].fd = -1;
        }
        else if(sz>0) 
        {
          printf("client say: %s\n",buf);
          write(cfd,buf,strlen(buf));
        }
      }
    }
  }
  return 0;
}
