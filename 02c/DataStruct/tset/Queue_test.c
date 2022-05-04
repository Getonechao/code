#include<stdio.h>
#include"Queue.h"


int main()
{
    ArrayQueue Q=init_arrayQueue();
    printf("number:%d\n", getCount_arrayQueue(Q));
    push_arrayQueue(Q,"hello0");
    push_arrayQueue(Q,"hello1");
    push_arrayQueue(Q,"hello2");
    printf("%s\n", pop_arrayQueue(Q));
    printf("%s\n", pop_arrayQueue(Q));
    printf("%s\n", pop_arrayQueue(Q));
    while (1)
    {
        
    }
    
}