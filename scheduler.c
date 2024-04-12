#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <time.h>
#include <setjmp.h>

#include "scheduler.h"

// Global vars
ucontext_t scheduler_context;
char stack[16000];
ucontext_t* current_context;


void initScheduler()
{
    // Init the tasks in the scheduler
    static bool didInit = false;
    if(!didInit)
    {
        for(int i = 0; i < MAX_TASKS; i++)
        {
            task_list[i].taskFunc = NULL;
            task_list[i].taskPriority = 0;
        }
        didInit = true;
    }
}

void addTask(void* func, int priority, int stackSize)
{
    // Check if task args are valid
    if(func == NULL)
    {
        printf("Invalid task function. Skipping.\n");
        return;
    }
    if(priority < 1 || priority > 4)
    {
        printf("Invalid task priority. Skipping.\n");
    }

    // Find room for the task
    for(int i = 0; i < MAX_TASKS; i++)
    {
        if(task_list[i].taskFunc == NULL && task_list[i].taskPriority == 0)
        {
            task_list[i].taskFunc = func;
            task_list[i].taskPriority = priority;
            if(getcontext(&task_list[i].context) == -1)
            {
                perror("getcontext");
            }
            task_list[i].context.uc_stack.ss_sp = task_list[i].stack;
            task_list[i].context.uc_stack.ss_size = sizeof(task_list[i].stack);
            task_list[i].context.uc_link = &scheduler_context;
            makecontext(&(task_list[i].context), task_list[i].taskFunc, 0);
            break;
        }
    }
}

void isr()
{
    printf("callback");
    // Timer expired - Swap the context back to the scheduler
    if(swapcontext(current_context, &scheduler_context) == -1)
    {
        perror("swapcontext");
    }
}

void make_scheduler()
{
    // Create a context for the scheduler
    getcontext(&scheduler_context);
    scheduler_context.uc_stack.ss_sp = stack;
    scheduler_context.uc_stack.ss_size = sizeof(stack);
    makecontext(&scheduler_context, runScheduler, 0);
}

void runScheduler()
{
    // Initalize the tasks
    initScheduler();

    // Make the scheduler
    make_scheduler();

    // Create isr and link to alarm
    signal(SIGALRM, isr);

    while(1)
    {
        for(int i = 0; i < 2; i++)
        {
            // Kick the alarm
            alarm(1); 

            printf("hit\n");

            // Set the current context to the current task
            current_context = &(task_list[i].context);

            printf("hit2\n");

            // TODO: Call this from the schedulig algorithm - Swap the context
            // This needs to be put in a wrapper funciton so we can see if the function has finished or not
            // Create a third context 
            if(&(task_list[i].context) == NULL)
            {
                perror("context");
            }

            if(swapcontext(&scheduler_context, &(task_list[i].context)) == -1)
            {
                perror("Swap context");
            }

            printf("hit3\n");
            
            alarm(0);

            // TODO: Replace this with the scheduling algorithm
            for(int j = 0; j < 1000; j++)
            {
                printf("HERE: %d\n", j);
            }
            printf("resetting\n");
        }
    }
}
