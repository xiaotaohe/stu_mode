/*
 *目标：本段代码(信号量)实现生产者消费者模型
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

//节点结构
typedef struct node
{
  int data;
  struct node* next;
}Node;
//永远指向链表的头节点
Node* head = NULL;

//创建生产者、消费者信号量
sem_t sem_produce;
sem_t sem_customer;
void* produce(void* arg)
{
  while(1)
  {
    Node* pnew = (Node*)malloc(sizeof(Node));
    //节点初始化
    pnew->data = rand()%1000;//0~999
    //判断是否有资源可供生产
    sem_wait(&sem_produce);//如果没有，则阻塞
    //指针域
    pnew->next = head;
    head = pnew;
    printf("========= produce:%lu,%d\n",pthread_self(),pnew->data);
    //通知阻塞的消费者线程，解除阻塞
    sem_post(&sem_customer);
    sleep(rand()%3);
  }
  return NULL;
}
void* customer(void* arg)
{
  while(1)
  {
    sem_wait(&sem_customer);//如果没有产品，阻塞；
    //链表不为空，删除一个节点
    Node* pdel = head;
    head = pdel->next;
    printf("========= customer:%lu,%d\n",pthread_self(),pdel->data);
    free(pdel);
    sem_post(&sem_produce);//通知生产者有，资源可供生产
  }
  return NULL;
}
int main(int ragc,char* argv[])
{
  pthread_t p1,p2;
  //信号量的初始化
  sem_init(&sem_produce,0,2);
  sem_init(&sem_customer,0,0);
  //1.创建生产者线程
  pthread_create(&p1,NULL,produce,NULL);
  //2.创建消费者线程
  pthread_create(&p2,NULL,customer,NULL);

  //阻塞回收子线程
  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
  //释放信号量
  sem_destroy(&sem_produce);
  sem_destroy(&sem_customer);
}

