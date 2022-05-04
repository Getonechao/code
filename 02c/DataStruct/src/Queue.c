#include<stdlib.h>
#include"Queue.h"
#include"string.h"
#include"stdio.h"


/*****************************
@function:init_arrayQueue
@description: 初始化数组队列
@input: 
        Queue:  ArrayQueue类型的数组
@outpt: 
@return: 0 success 1 fail
******************************/
int init_arrayQueue(ArrayQueue Queue)//段错误，指针也是copy进来的
{
    Queue->array = (ArrayQueue)malloc(MAXQSIZE * sizeof(QueueNode));
    if (!Queue->array)
    {
        printf("初始化队列失败");
        Queue->array = NULL;
        return FUNCTION_FALSE;
    }
    else
    {
        memset(Queue->array, 0, MAXQSIZE * sizeof(QueueNode));
        Queue->front = 0;
        Queue->rear = 0;
        Queue->length = 0;
        return FUNCTION_TRUE;
    }
}


int getCount_arrayQueue(ArrayQueue Queue)
{
    int length = (*Queue).length;
    return length;
}

int push_arrayQueue(ArrayQueue Queue,const QueueNode QueueNode)
{
    if((*Queue).length==MAXQSIZE)
    {
        printf("队伍已满\n");
        return 1;
    }
    else
    {
    memcpy(&(*Queue).array[(*Queue).rear], &QueueNode,sizeof(QueueNode));
    (*Queue).rear = ((*Queue).rear+1) % MAXQSIZE;
    (*Queue).length++;
    return 0;
    }
}

QueueNode pop_arrayQueue(ArrayQueue Queue)
{
    if((*Queue).length==0)
    {
        printf("队伍已空\n");
        
    }
    else
    {
    (*Queue).front = ((*Queue).front + 1) % MAXQSIZE;
    (*Queue).length--;
    return (*Queue).array[((*Queue).front - 1+MAXQSIZE) % MAXQSIZE];
    }
}

void destroy_arrayQueue(ArrayQueue Queue)
{
    free(Queue);//释放后，请将指针置空
}