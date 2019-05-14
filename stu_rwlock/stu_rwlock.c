#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>


//创建读写锁
pthread_rwlock_t lock;

//共享资源
int number = 0;
void* write_func(void* arg)
{
  //循环写
  while(1)
  {
    //加写锁
    pthread_rwlock_wrlock(&lock);
    number++;
    printf("write:%lu,%d\n",pthread_self(),number);
    pthread_rwlock_unlock(&lock);
    usleep(500);
  }
  return NULL;
}
void* read_func(void* arg)
{
  while(1)
  {
    pthread_rwlock_rdlock(&lock);
    printf("read:%lu,%d\n",pthread_self(),number);
    pthread_rwlock_unlock(&lock);
    usleep(500);
  }
  return NULL;
}
int main()
{
  pthread_t p[8];
  //初始化读写锁
  pthread_rwlock_init(&lock,NULL);
  //1.创建3个写线程
  for(int i = 0;i<3;i++)
    pthread_create(&p[i],NULL,write_func,NULL);
  //2.创建5个读线程
  for(int i = 3;i<8;i++)
    pthread_create(&p[i],NULL,read_func,NULL);
  //3.阻塞回收子线程的pcb
  for(int i = 0;i<8;i++)
    pthread_join(p[i],NULL);
  //释放读写锁资源
  pthread_rwlock_destroy(&lock);
  return 0;
}
