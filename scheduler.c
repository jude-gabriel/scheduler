#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <setjmp.h>
#include <string.h>

#include "scheduler.h"

// Global vars
task task_list[MAX_TASKS];
ucontext_t scheduler_context;
int delete = 0;
int increase = 0;
char stack[16000];
ucontext_t *current_context;
queue p_four;
queue p_three;
queue p_two;
queue p_one;

void initScheduler()
{
    // Init the tasks in the scheduler
    static bool didInit = false;
    if (!didInit)
    {
        for (int i = 0; i < MAX_TASKS; i++)
        {
            task_list[i].taskFunc = NULL;
            task_list[i].taskPriority = 0;
            task_list[i].taskid = i;
            p_four[i] = NULL;
            p_three[i] = NULL;
            p_two[i] = NULL;
            p_one[i] = NULL;
        }
        didInit = true;
    }
}

void addTask(void *func, int priority, int stackSize)
{
    // Check if task args are valid
    if (func == NULL)
    {
        printf("Invalid task function. Skipping.\n");
        return;
    }
    if (priority < 1 || priority > 4)
    {
        printf("Invalid task priority. Skipping.\n");
    }

    // Find room for the task
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (task_list[i].taskFunc == NULL && task_list[i].taskPriority == 0)
        {
            // Make the context
            task_list[i].taskFunc = func;
            task_list[i].taskPriority = priority;
            if (getcontext(&task_list[i].context) == -1)
            {
                perror("getcontext");
            }
            task_list[i].context.uc_stack.ss_sp = task_list[i].stack;
            task_list[i].context.uc_stack.ss_size = sizeof(task_list[i].stack);
            task_list[i].context.uc_link = &scheduler_context;
            makecontext(&(task_list[i].context), task_list[i].taskFunc, 0);

            // Add the context to the correct queue
            switch (priority)
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

void addToQueue(task *t, queue *q)
{
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if ((*q)[i] == NULL)
        {
            (*q)[i] = t;
            break;
        }
    }
}

void isr()
{
    // Timer expired - Swap the context back to the scheduler
    if (swapcontext(current_context, &scheduler_context) == -1)
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

/*
sets the delete value to one to delete the task on the next iteration
*/
void set_delete()
{
    alarm(0);
    delete = 1;
    task_yield();
}

/*
@params - the queue you want to shift, and the value to start the shift
@return - none
@description - Removes the task at the from value and shifts the rests of the tasks one to the left
*/
void shift_task(queue *q, int from)
{
    while ((*q)[from] != NULL)
    {
        if (from + 1 < MAX_TASKS)
        {
            (*q)[from] = (*q)[from + 1];
        }
        from++;
    }
}

/*
@params - none
@return - none
@description - allows a task to immediately yield itself
*/
void task_yield()
{
    // remove isr alarm
    alarm(0);
    // swap context back to the scheduler
    if (swapcontext(current_context, &scheduler_context) == -1)
    {
        perror("swapcontext");
    }
}

/*
@params - task you want to increase priority of
@return none
@description - moves a task up one priority level by using add to queue
*/
void increasePriority(task *t)
{
    if(t == NULL){
        return;
    }
    // determine what queue the task is in and change its priority to be accurate
    switch (t->taskPriority)
    {
    case 4:
    {

        printf("No higher priority\n");
        break;
    }
    case 3:
    {
        addToQueue(t, &p_four);
        t->taskPriority = 4;
        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (p_three[i]->taskid == t->taskid)
            {
                shift_task(&p_three, i);
                break;
            }
        }
        break;
    }
    case 2:
    {
        addToQueue(t, &p_three);
        t->taskPriority = 3;
        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (p_two[i]->taskid == t->taskid)
            {
                shift_task(&p_two, i);
                break;
            }
        }
        break;
    }
    case 1:
    {
        addToQueue(t, &p_two);
        t->taskPriority = 2;
        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (p_one[i]->taskid == t->taskid)
            {
                shift_task(&p_one, i);
                break;
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

// sets increase to 1
void setIncreaseP()
{
    increase = 1;
}

/*
@params - the task you want to remove from the task list
@return - nune
@description - removes the associated task from the task list and adds it to the emptyTaskList array
*/
void deleteFromList(task t)
{
    int deleteX = -1;

    // locate the task
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (t.taskid == task_list[i].taskid)
        {

            deleteX = i;
            break;
        }
    }
    if (deleteX < 0)
    {
        return;
    }
    // Clear out the task from the list
    task_list[deleteX].taskFunc = NULL;
    task_list[deleteX].taskid = task_list[deleteX + 1].taskid;
    task_list[deleteX].taskPriority = 0;
    printf("%d %d\n", deleteX, task_list[deleteX].taskPriority);
    task_list[deleteX].context.uc_stack.ss_sp = NULL;
    task_list[deleteX].context.uc_stack.ss_size = 0;
    task_list[deleteX].context.uc_link = NULL;
}

void runScheduler()
{

    // Make the scheduler
    make_scheduler();

    // Create isr and link to alarm
    signal(SIGALRM, isr);

    while (1)
    {
        // Run level four tasks
        int i = 0;
        while (p_four[i] != NULL)
        {
            alarm(4);
            current_context = &(p_four[i]->context);
            if (swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            if (delete)
            {
                deleteFromList(*p_four[i]);
                shift_task(&p_four, i);
                i--;
                delete = 0;
            }
            if (increase)
            {
                increase = 0;
            }
            i++;
        }

        // Run level three tasks
        i = 0;
        while (p_three[i] != NULL)
        {
            alarm(3);
            current_context = &(p_three[i]->context);
            if (swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            if (delete && !increase)
            {
                deleteFromList(*p_three[i]);
                shift_task(&p_three, i);
                i--;
                delete = 0;
            }
            if (increase)
            {
                increasePriority(p_three[i]);
                i--;
                increase = 0;
            }
            i++;
        }
        // Run level two tasks
        i = 0;
        while (p_two[i] != NULL)
        {
            alarm(2);
            current_context = &(p_two[i]->context);
            if (swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            if (delete && !increase)
            {
                deleteFromList(*p_two[i]);
                shift_task(&p_two, i);

                i--;

                delete = 0;
            }
            if (increase)
            {
                increasePriority(p_two[i]);
                i--;
                increase = 0;
            }
            i++;
        }

        // Run level one tasks
        i = 0;
        while (p_one[i] != NULL)
        {
            alarm(1);
            current_context = &(p_one[i]->context);
            if (swapcontext(&scheduler_context, current_context) == -1)
            {
                perror("Swap context");
            }
            if (delete && !increase)
            {
                deleteFromList(*p_one[i]);
                shift_task(&p_one, i);
                i--;

                delete = 0;
            }
            if (increase)
            {
                increasePriority(p_one[i]);
                i--;
                increase = 0;
            }
            i++;
        }
    }
}
