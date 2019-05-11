/*
 *1.目标：实现两个线程对同一个数的加操作
 *2.不加锁有怎样的效果？
 *3.实现线程同步
 */
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>
#define MAX 10000

pthread_mutex_t mutex;
int number;
void* funcA(void* arg)
{
  for(int i = 0;i<MAX;i++)
  {
    pthread_mutex_lock(&mutex);
    int cur = number;
    cur++;
    number = cur;
    pthread_mutex_unlock(&mutex);
    printf("线程A,id = %lu,cur = %d\n",pthread_self(),number);
   usleep(20);
  }
  return NULL;
}
void* funcB(void* arg)
{
  for(int i = 0;i<MAX;i++)
  {
    pthread_mutex_lock(&mutex);
    int cur = number;
    cur++;
    number = cur;
    pthread_mutex_unlock(&mutex);
    printf("线程B,id = %lu,cur = %d\n",pthread_self(),number);
  usleep(20);
  }
  return NULL;
}
int main()
{
  pthread_mutex_init(&mutex,NULL);
  //创建线程
  pthread_t p1,p2;
  pthread_create(&p1,NULL,funcA,NULL);
  pthread_create(&p2,NULL,funcB,NULL);
  //线程等待
  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
  pthread_mutex_destroy(&mutex);
  return 0;
}

