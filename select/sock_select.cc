#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/select.h>
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
  printf("%d\n",listen_sock);
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
  //这里才是select的重点部分
  //4.多路复用部分
  fd_set read_set;
  fd_set temp_set;//因为select每次都会修改read_set,通过temp_set备份
  int max = 0;
  int a_size = 0;
  //添加listen套接字到read_set中
  FD_ZERO(&read_set);
  FD_ZERO(&temp_set);
  FD_SET(listen_sock,&read_set);
  max = listen_sock;
  a_size++;
  temp_set = read_set;
  //文件描述符表
  int a[1024] = {-1};
  a[0] = listen_sock;
  //5.循环处理业务
  while(1)
  {
    read_set = temp_set;
    sockaddr_in peer_addr;
    FD_SET(listen_sock,&read_set);
    struct timeval t;
    t.tv_sec = 2;
    int ret = select(max+1,&read_set,NULL,NULL,&t);
    if(ret<0)
    {
      printf("select\n");
    }
    if(ret == 0)
    {
      printf("time out!\n");
    }
    if(FD_ISSET(listen_sock,&read_set))
    {
      int connect_fd = accept(listen_sock,(struct sockaddr*)&peer_addr,&len);
      if(connect_fd<0)
      {
        printf("accept!\n");
        return 5;
      }
      //判断fd是否加入到文件描述符表
      a[a_size++] = connect_fd; 
      //将connect_fd加入到temp_set中
      FD_SET(connect_fd,&temp_set);
      //判断是否需要更新max
      if(connect_fd>max)
        max = connect_fd;
    }
    //处理connect_fd
    for(int i = 0;i<a_size;i++)
    {
      if(FD_ISSET(a[i],&read_set))
      {
        char buf[1024] = {0};
        int rz = read(a[i],buf,1023);
        if(rz<0)
        {
          printf("read!\n");
          continue;
        }
        if(rz == 0)
        {
          printf("peer close!!\n");
          close(a[i]);
        }
        printf("peer say:%s\n",buf);
        write(a[i],buf,strlen(buf));
      }
    }
  }
  return 0;
}
