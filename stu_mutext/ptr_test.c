#include<stdio.h>
#include<malloc.h>
int main()
{
  int arr[] = {1,2,3,4,5};
  char* str[] = {"hello","world"};
  int* p = arr;
  //对数组区地址为二级指针
  int(*p1)[5] = &arr;
  printf("%d\n",(*p1)[1]);
  //指针数组类似于二级指针
  char** p2 = str;
  printf("%s\n",p2[0]);
  //对指针数组取地址为三级
  char(**p3)[6] = &str;
  printf("%s\n",*(p3[1]));
}
