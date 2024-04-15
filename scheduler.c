#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <time.h>
#include <setjmp.h>

#include "scheduler.h"

// Global vars
task task_list[MAX_TASKS];
ucontext_t scheduler_context;
char stack[16000];
ucontext_t* current_context;
queue p_four;
queue p_three;
queue p_two;
queue p_one;


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
            p_four[i] = NULL;
            p_three[i] = NULL;
            p_two[i] = NULL;
            p_one[i] = NULL;
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
            // Make the context
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

            // Add the context to the correct queue
            switch(priority)
            {
                case 4:
                {
                    addToQueue(&(task_list[i]), &p_four);
                    break;
                }
                case 3:
                {
                    addToQueue(&(task_list[i]), &p_three);
                    break;
                }
                case 2:
                {
                    addToQueue(&(task_list[i]), &p_two);
                    break;
                }
                case 1:
                {
                    addToQueue(&(task_list[i]), &p_one);
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
    }
}

void addToQueue(task* t, queue* q)
{
    for(int i = 0; i < MAX_TASKS; i++)
    {
        if((*q)[i] == NULL)
        {
            (*q)[i] = t;
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

    // Make the scheduler
    make_scheduler();

    // Create isr and link to alarm
    signal(SIGALRM, isr);

    while(1)
    {
        // Run level four tasks 
        int i = 0; 
        while(p_four[i] != NULL)
        {
            alarm(4);
            current_context = &(p_four[i]->context);
            printf("\n\n\n\n%d\n", (p_four[i]->taskPriority));
            if(swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            i++;
        }

        // Run level three tasks 
        i = 0; 
        while(p_three[i] != NULL)
        {
            alarm(3);
            current_context = &(p_three[i]->context);
            if(swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            i++;
        }

        // Run level two tasks 
        i = 0; 
        while(p_two[i] != NULL)
        {
            alarm(2);
            current_context = &(p_two[i]->context);
            if(swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            i++;
        }

        // Run level one tasks 
        i = 0; 
        while(p_one[i] != NULL)
        {
            alarm(1);
            current_context = &(p_one[i]->context);
            if(swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            i++;
        }
    }
}
