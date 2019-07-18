/*************************************************************************
  > File Name: stu_pthread_cond.cpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Wed 17 Jul 2019 07:16:14 AM PDT
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<pthread.h>
#include<atomic>
#include<string>
using namespace std;

//标记每种原料在当前遍历原料总数的下标
int H_num = 0;
int O_num = 0;
atomic_bool H_flag(false);//拿到两个氢之后，去通知另一个线程去取氧
//标记已取出多少原料
atomic_int num(0);


//锁和条件变量
pthread_mutex_t mutex_lock;
pthread_cond_t H_ready;//氢条件变量
pthread_cond_t O_ready;//氧条件变量

string str = "";
void* product(void* arg)
{
  string str = *((string*)arg);
  while(1)
  {
    pthread_mutex_lock(&mutex_lock);
    while(H_flag == true)
      pthread_cond_wait(&H_ready,&mutex_lock);
    int second = 2;
    //取出两个氢
    while(second != 0 && H_num<str.size())
    {
      if(str[H_num++] == 'H')
      {
        cout<<"H";  
        num++;
        second--;
      }
    }
    //两个氢去完，释放锁
    H_flag = true;
    pthread_cond_signal(&O_ready);
    pthread_mutex_unlock(&mutex_lock);
  }
}

void* consumer(void* arg)
{
  string str = *((string*)arg);
  while(1)
  {
    pthread_mutex_lock(&mutex_lock);
    while(H_flag == false)
      pthread_cond_wait(&O_ready,&mutex_lock);
    //取出一个氧
    int second = 1;
    while(second != 0&&O_num<str.size())
    {
      if(str[O_num++] == 'O')
      {
        cout<<"O";
        fflush(stdout);
        num++;
        second--;
      }
    }
    //一个去完，释放锁
    H_flag = false;
    pthread_cond_signal(&H_ready);
    pthread_mutex_unlock(&mutex_lock);
  }
}

int main()
{
  string str;
  cout<<"请输入原料: "<<endl;
  cin>>str;
  //初始化
  pthread_cond_init(&H_ready,NULL);
  pthread_cond_init(&O_ready,NULL);
  pthread_mutex_init(&mutex_lock,NULL);
  //H(氢)作为生产者
  //O(氧)作为消费者
  pthread_t H_ret,O_ret;
  pthread_create(&H_ret,NULL,&product,&str);
  pthread_create(&O_ret,NULL,&consumer,&str);
  
  //线程分离
  pthread_join(H_ret,NULL);
  pthread_join(O_ret,NULL);
}
