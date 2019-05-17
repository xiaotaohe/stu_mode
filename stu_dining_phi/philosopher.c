/*
 *目标：实现解决哲学家就餐问题
 *1.规定哲学家为 0 1 2 3 4
 *2.筷子为0 1 2 3 4
 *3.左边筷子为哲学家自身编号，右边筷子为哲学家自身编号加1
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<pthread.h>
#include<semaphore.h>

#define phil_num 5
//筷子
sem_t chopsticks[phil_num];
//锁
pthread_mutex_t  mutex;
//哲学家
pthread_t philosopher[phil_num];
int phil_id[5] = {0,1,2,3,4};


void* phil(void* arg)
{
  int i = *(int*)arg;
  int left = i;
  int right = (i+1)%phil_num;
  while(1)
  {
    //所有的筷子是临界资源
    printf("%d哲学家思考中.........\n",i);
    sleep(3);
    //饿了，开始抢筷子吃饭
    printf("%d哲学家饿了，开始抢筷子吃饭\n",i);
    pthread_mutex_lock(&mutex);
    sem_wait(&chopsticks[left]);
    printf("%d哲学家，获取左边的筷子\n",i);
    sem_wait(&chopsticks[right]);
    sleep(4);
    printf("%d哲学家获取一双筷子,开始吃饭\n",i);
    printf("%d哲学家吃饭中.......\n",i);
    sem_post(&chopsticks[left]);
    sem_post(&chopsticks[right]);
    printf("吃饭结束，并放下了所有筷子\n");
    pthread_mutex_unlock(&mutex); 
  }
}
int main()
{
  //1.初始化sem_t、mutex 
  for(int i = 0;i<5;i++)
  {
    sem_init(&chopsticks[i],0,1);
  }
  //2.初始化
  pthread_mutex_init(&mutex,NULL);
  //3.创建线程
  for(int i = 0;i<5;i++)
  {
    pthread_create(&philosopher[i],NULL,phil,(void*)&phil_id[i]);
  }
  for(int i = 0;i<5;i++)
  {
    pthread_join(philosopher[i],NULL);
  }
  for(int i = 0;i<5;i++)
    sem_destroy(&chopsticks[i]);
  pthread_cond_destroy(&mutex);
  return 0;
}
