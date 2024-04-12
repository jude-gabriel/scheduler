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

typedef task* queue[MAX_TASKS];

void initScheduler();
void make_scheduler();
void addTask(void* func, int priority, int stack_size);
void addToQueue(task* t, queue* q);

void runScheduler();
void isr();