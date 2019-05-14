/*
 *目标：本段代码（通过条件变量+互斥锁）实现生产者消费者模型
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>

//节点结构
typedef struct node
{
  int data;
  struct node* next;
}Node;
//永远指向链表的头节点
Node* head = NULL;
//线程同步：互斥锁
pthread_mutex_t mutex;
//线程阻塞：条件变量
pthread_cond_t cond;
void* produce(void* arg)
{
  while(1)
  {
    Node* pnew = (Node*)malloc(sizeof(Node));
    //节点初始化
    pnew->data = rand()%1000;//0~999
    //使用互斥锁对共享内存加锁
    pthread_mutex_lock(&mutex);
    //指针域
    pnew->next = head;
    head = pnew;
    printf("========= produce:%lu,%d\n",pthread_self(),pnew->data);
    pthread_mutex_unlock(&mutex);
    //通知阻塞的消费者线程，解除阻塞
    pthread_cond_signal(&cond);
    sleep(rand()%3);
  }
  return NULL;
}
void* customer(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    //判断链表是否为空
    if(head == NULL)
    {
      //线程阻塞
      //该函数会对互斥锁解锁
      pthread_cond_wait(&cond,&mutex);
      //当被唤醒时，解除阻塞，会对互斥锁加锁
    }
    //链表不为空，删除一个节点
    Node* pdel = head;
    head = pdel->next;
    printf("========= customer:%lu,%d\n",pthread_self(),pdel->data);
    free(pdel);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
int main(int ragc,char* argv[])
{
  pthread_t p1,p2;
  //互斥锁，条件变量初始化
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond,NULL);
  //1.创建生产者线程
  pthread_create(&p1,NULL,produce,NULL);
  //2.创建消费者线程
  pthread_create(&p2,NULL,customer,NULL);

  //阻塞回收子线程
  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
  //释放互斥锁，条件变量
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}

