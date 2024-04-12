#pragma once

#include <ucontext.h>


#define MAX_TASKS 32
#define MAX_PRIORITY 4
#define MIN_PRIORITY 1


typedef struct task
{
    void (*taskFunc)(void);
    int taskPriority;
    char stack[16000];
    ucontext_t context;
} task;


task task_list[MAX_TASKS];


void make_scheduler();
void addTask(void* func, int priority, int stack_size);
void runScheduler();
void isr();