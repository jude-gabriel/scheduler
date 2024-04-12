#include <stdio.h>
#include <stdlib.h>

#include "tasks.h"

void task1()
{
    int i = 0;
    while(1)
    {
        printf("Task 1: %d\n", i);
        i++;
    }
}

void task2()
{
    int i = 0;
    // while(1)
    // {
    //     printf("Task 2: %d\n", i);
    //     i++;
    // }
    // int i;
    for(i = 0; i < 10000; i++)
    {
        printf("Task 2: %d\n", i);
    }
    return;
}