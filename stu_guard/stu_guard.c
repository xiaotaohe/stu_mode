/*
 * 目标:创建守护进程,每隔3秒将时间写入磁盘
 */
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/time.h>
#include<time.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>
#include<string.h>
#include<ctime>
#include<signal.h>

void do_work(int nb)
{
      int fd = open("/root/Class_L/stu_mode/stu_guard/time_temp.txt",O_CREAT|O_RDWR|O_APPEND,0664);
      time_t tm;
      time(&tm);
      char* s = ctime(&tm);
      write(fd,s,strlen(s));
			sleep(3);
}
int main()
{
    /* 屏蔽一些有关控制终端操作的信号 
     * 防止在守护进程没有正常运转起来时，因控制终端受到干扰退出或挂起 
     * */  
    signal(SIGINT,  SIG_IGN);// 终端中断  
    signal(SIGHUP,  SIG_IGN);// 连接挂断  
    signal(SIGQUIT, SIG_IGN);// 终端退出  
    signal(SIGPIPE, SIG_IGN);// 向无读进程的管道写数据  
    signal(SIGTTOU, SIG_IGN);// 后台程序尝试写操作  
    signal(SIGTTIN, SIG_IGN);// 后台程序尝试读操作  
    signal(SIGTERM, SIG_IGN);// 终止 
  //1.重置当前工作目录
  chdir("/");
  //2.重置文件掩码
  umask(0);
  //3.创建子进程，父进程退出
  pid_t pid = fork();
  if(pid<0)
  {
    printf("fork error!\n");
    exit(-1);
  }
  if(pid > 0)
    exit(1);
  else if(pid == 0)
  {
    //4.子进程开启会话
    setsid();
		//5.关闭文件描述符（打开的文件描述符，以免浪费资源）
		for(int i = 0;i<3;i++)
		{
			close(i);
		}
		while(1)
		{
      //获取时间
      //方法1：
      /*
			time_t time;
			struct tm *tminfo = localtime(&time);
			write(fd,asctime(tminfo),strlen(asctime(tminfo)));
      */
      //方法2：
      /*
      time_t tm;
      time(&tm);
      char* s = ctime(&tm);
      write(fd,s,strlen(s));
			sleep(3);
      */
      //方法3：使用定时器
      //1.信号捕捉，捕捉定时器
      struct sigaction act;
      act.sa_flags = 0;
      act.sa_handler = do_work;
      sigemptyset(&act.sa_mask);
      sigaction(SIGALRM,&act,NULL);
      //设置定时器
      //1.设置第一次触发的时间
      struct itimerval val;
      val.it_value.tv_sec = 1;
      val.it_value.tv_usec = 0;
      //2.设置循环时间
      val.it_interval.tv_sec = 2;
      val.it_interval.tv_usec = 0;
      setitimer(ITIMER_REAL,&val,NULL);
      while(1)
        ;
		}
  }
	return 0;
}

