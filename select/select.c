#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
int main()
{
  int ret=0;
  char buff[1024];
  fd_set read_fds;
  FD_ZERO(&read_fds);
  struct timeval val;
  val.tv_sec=2;
  fd_set temp_fds;
  temp_fds = read_fds;
  //val.tv_usec=0;
  while(1)
  {
   //memset(buff,'\0',sizeof(buff));
  // fgets(buff,1023,stdin);
   //FD_ZERO(&read_fds);
   // FD_SET(STDIN_FILENO,&read_fds);
   val.tv_sec=2;
  read_fds = temp_fds;
   ret=select(STDIN_FILENO+1,&read_fds,NULL,NULL,&val);
   //printf("select\n");
   //printf("%d\n",ret);
   if(ret<0)
   {
     printf("错误\n");
  //   break;
   }
  else if(ret == 0)
   {
     printf("tiem out\n");
   }
   
   //read(0,buff,1024);
   if(FD_ISSET(STDIN_FILENO,&read_fds))
   {
     printf("%s\n",buff);
   }
 }
  return 0;
}
