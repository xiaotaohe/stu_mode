/*************************************************************************
    > File Name: ABC.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Sat 31 Aug 2019 10:37:02 PM PDT
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h>
using namespace std;

int num = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void* run(void* arg)
{
  long pthread_id = (long)arg;
  for(int i = 0;i<10;i++)
  {
    pthread_mutex_lock(&mutex);
    while(pthread_id != num)
      pthread_cond_wait(&cond,&mutex);
    printf("%c",num+'A');
    fflush(stdout);
    num = (num+1)%3;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
  }
  return NULL;
}

int main()
{
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond,NULL);
  pthread_t ret[3];
  for(int i = 0;i<3;i++)
    pthread_create(&ret[i],NULL,run,(void*)i);
  for(int i = 0;i<3;i++)
    pthread_join(ret[i],NULL);
	return 0;
}
