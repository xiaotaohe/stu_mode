/*
 *通过将print封装称为静态库、动态库
 */
#include<stdio.h>
void print(char* str)
{
  printf("%s\n",str);
}

