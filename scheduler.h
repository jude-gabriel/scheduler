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
    int taskid;
    ucontext_t context;
} task;

typedef task* queue[MAX_TASKS];

// Global vars
extern task task_list[MAX_TASKS];
extern ucontext_t scheduler_context;
extern char stack[16000];
extern int delete;
extern int increase;
extern ucontext_t* current_context;
extern queue p_four;
extern queue p_three;
extern queue p_two;
extern queue p_one;

void initScheduler();
void make_scheduler();
void addTask(void* func, int priority, int stack_size);
void addToQueue(task* t, queue* q);
void set_delete();
void shift_task(queue *q, int from);
void task_yield();
void deleteFromList(task t);
void increasePriority(task *t);
void setIncreaseP();
void runScheduler();
void isr();