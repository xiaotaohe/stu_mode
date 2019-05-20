/*
 * 目标：实现最简单的线程池，理清线程池思路
 */
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<assert.h>
#include<pthread.h>

/*任务队列维护的结构体*/
typedef struct worker
{
  //成员：线程的入口函数(回调函数);回调函数的参数
  void* (*process)(void*arg);
  void *arg;
  struct worker *next;
}Thread_worker;

/*线程池结构*/
/*成员：1.操作任务队列时，对任务队列需要进行保护(锁)
 * 2.线程有任务是工作，没有任务时需要阻塞，线程阻塞(条件变量)
 * 3.任务队列
 * 4.当前任务个数
 * 5.线程池的开启与关闭，需要控制shutdown
 * 6.所线程的标识符，需要保存
 * 7.允许创建线程的最大值(在这例子线程数是固定的)
 */
typedef struct pool
{
  pthread_mutex_t queue_lock;/*保护任务队列资源的锁*/
  pthread_cond_t queue_ready;/*线程阻塞的条件变量*/

  Thread_worker *queue_head;/*任务队列*/
  int cur_queue_size;/*当前队列中的任务个数*/ 

  bool shutdown;/*线程池的销毁开关*/

  pthread_t *threadid;/*线程池中线程的线程id*/
  int max_thread_num;/*线程池中最大线程数*/
}Thread_pool;

bool pool_create(int max_thread_num);
bool pool_add_worker(void* (*process)(void* arg),void *arg);
void  pool_destroy();
void* thread_routine(void* arg);

static Thread_pool *pool = NULL;/*线程池*/

bool pool_create(int max_thread_num)
{
  pool = (Thread_pool*)malloc(sizeof(Thread_pool));
  if(pool == NULL)
    return false;
  pthread_mutex_init(&pool->queue_lock,NULL);
  pthread_cond_init(&pool->queue_ready,NULL);

  pool->queue_head = NULL;//刚开始任务队列为空
  pool->max_thread_num = max_thread_num;//初始化最大线程数
  pool->cur_queue_size = 0;//开始时任务数为0

  pool->shutdown = false;//线程池销毁开关为关闭

  //线程创建
  pool->threadid = (pthread_t*)malloc(sizeof(pthread_t)*max_thread_num);
  int i = 0;
  for(i = 0;i<max_thread_num;i++)
    pthread_create(&(pool->threadid[i]),NULL,thread_routine,NULL);
  return true;
}

/*向任务队列中添加任务*/

bool pool_add_worker(void*(*process)(void* arg),void* arg)
{
  //开辟任务的空间
  Thread_worker *new_worker = (Thread_worker*)malloc(sizeof(Thread_worker));
  if(new_worker == NULL)
    return false;
  new_worker->process = process;
  new_worker->arg = arg;
  new_worker->next = NULL;

  //向任务队列中添加任务
  pthread_mutex_lock(&pool->queue_lock);
  Thread_worker* queue = pool->queue_head;
  if(queue == NULL)
    pool->queue_head = new_worker;
  else 
  {
    while(queue->next != NULL)
      queue = queue->next;
    queue->next = new_worker;
  }
  assert(pool->queue_head != NULL);

  pool->cur_queue_size++;
  pthread_mutex_unlock(&pool->queue_lock);

  /*任务队列中有了任务通知阻塞的线程*/
  pthread_cond_signal(&(pool->queue_ready));
  return true;
}

void pool_destroy()
{
  //防止销毁两次
  if(pool->shutdown)
    return;
  pool->shutdown = true;

  /*唤醒所有线程，销毁所有线程*/
  pthread_cond_broadcast(&(pool->queue_ready));
  /*阻塞等待线程退出，回收资源*/
  int i = 0;
  for(;i<pool->max_thread_num;i++)
    pthread_join(pool->threadid[i],NULL);
  free(pool->threadid);

  /*销毁等待队列*/
  Thread_worker *del_worker = NULL;
  while(pool->queue_head != NULL)
  {
    del_worker = pool->queue_head;
    pool->queue_head = pool->queue_head->next;
    free(del_worker);
  }

  /*销毁条件变量和互斥量*/
  pthread_cond_destroy(&pool->queue_ready);
  pthread_mutex_destroy(&pool->queue_lock);

  /*销毁线程池*/
  free(pool);
  pool = NULL;
}

/*线程池中所有函数的统一入口*/
void* thread_routine(void* arg)
{
  printf("start thread 0x%x\n",pthread_self());
  while(1)
  {
    pthread_mutex_lock(&(pool->queue_lock));
    while(pool->cur_queue_size == 0 && !pool->shutdown)
    {
      printf("thread 0x%x is waiting\n",pthread_self());
      pthread_cond_wait(&(pool->queue_ready),&(pool->queue_lock));
    }

    /*当线程池要销毁时*/
    if(pool->shutdown)
    {
      pthread_mutex_unlock(&(pool->queue_lock));
      printf("thread 0x%x will exit\n",pthread_self());
      pthread_exit(NULL);
    }
    /*此时线程从任务队列中获取任务，开始工作*/
    printf("thread 0x%x starting work\n",pthread_self());

    assert(pool->cur_queue_size != 0);
    assert(pool->queue_head != NULL);

    /*从任务队列中获取任务*/
    pool->cur_queue_size--;
    Thread_worker *worker = pool->queue_head;
    pool->queue_head = worker->next;
    pthread_mutex_unlock(&(pool->queue_lock));

    /*调用线程的回调函数*/
    worker->process(worker->arg);
    /*从任务队列中获取的任务最终都任务完成都要释放*/ 
    free(worker);
    worker = NULL;
  }
  /*程序一般是不会运行到这个位置的*/
  pthread_exit(NULL);
}

//测试
void* my_process(void *arg)
{
  printf("threadid is 0x%x,working on task %d\n",pthread_self(),*(int*)arg);
  sleep(2);
  return NULL;
}

int main()
{
  //线程池中最大创建线程数为3
  int max_thread_num = 3;
  pool_create(max_thread_num);
  //连续向线程池中投入10个任务
  int *worker_arg = (int*)malloc(sizeof(int)*10);
  int i = 0;
  for(;i<10;i++)
  {
    worker_arg[i] = i;
    pool_add_worker(&my_process,&worker_arg[i]);
  }
  //等待所有任务完成
  sleep(6);
  //销毁线程池
  pool_destroy();

  free(worker_arg);
  return 0;
}
