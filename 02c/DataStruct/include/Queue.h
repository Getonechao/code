#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H

#define MAXQSIZE 20 //队列长度
#define MAXSSIZE 1024 //字符串大小
#define FUNCTION_FALSE 1
#define FUNCTION_TRUE 0


typedef struct QueueNode
{
    char string[MAXSSIZE]
}QueueNode;


/*
*   字符串数组队列
*/
typedef struct arrayQueue
{
    QueueNode *array;
    int front; //队头
    int rear;//队尾
    int length;//队列长度

}arrayQueue,*ArrayQueue;

/*****************************
@function:init_arrayQueue
@description: 初始化数组队列
@input: 
@outpt: 
@return: 成功：返回队列结构(堆内存)的指针
         失败：指向NULL的指针
@time: 2021-10-28 
@author: zhangchao 
******************************/
int init_arrayQueue(ArrayQueue Queue);

/*****************************
@function:getCount_arrayQueue
@description: 获取数组队列的排队个数
@input: 1：Queue init_arrayQueue()返回的指针
@outpt: 
@return: 队列长度count
******************************/
int getCount_arrayQueue(ArrayQueue Queue);

/*****************************
@function:push_arrayQueue
@description:任务入队列
@input: 1：Queue    init_arrayQueue()返回的指针
        2：string   要压入队列的字符串
@outpt: 
@return: 0 success 
         1 队伍已满
@time: 2021-10-28 
@author: zhangchao 
******************************/
int push_arrayQueue(ArrayQueue Queue,const QueueNode QueueNode);


/*****************************
@function:pop_arrayQueue
@description:任务出队列
@input: 1：Queue    init_arrayQueue()返回的指针
@outpt: 
@return: 出队列字符串的指针,如果队伍空了，返回NULL
@time: 2021-10-28 
@author: zhangchao 
******************************/
QueueNode pop_arrayQueue(ArrayQueue Queue);

/*****************************
@function:destroy_arrayQueue
@description:销毁队列
@input: 1：Queue    init_arrayQueue()返回的指针
@outpt: 
@return: 
@time: 2021-10-28 
@author: zhangchao 
******************************/
void destroy_arrayQueue(ArrayQueue Queue);

#endif