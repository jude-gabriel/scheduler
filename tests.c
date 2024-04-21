#include "tests.h"
#include "tasks.h"
#include "scheduler.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <time.h>

int task_test_global = 0;
int isr_hit = 0;
ucontext_t test_context;

void test_suite()
{
    // Test that adding tasks works correctly
    add_tasks();

    // Test that context switching works correctly
    context_switch();

    // Test that the isr is called correctly
    test_isr();

    //Test Deleting a task
    test_delete();

    // Test increasing priority
    test_priority_increase();

    printf("Test suite done\n");
}

void add_tasks()
{
    initScheduler();
    addTask(&task_1, 1, 4098);
    addTask(&task_2, 2, 4098);
    addTask(&task_3, 3, 4098);
    addTask(&task_4, 4, 4098);

    if (task_list[0].taskPriority != 1)
    {
        printf("Add Task fails\n");
        return;
    }
    else if (task_list[1].taskPriority != 2)
    {
        printf("Add Task fails\n");
        return;
    }
    else if (task_list[2].taskPriority != 3)
    {
        printf("Add Task fails\n");
        return;
    }
    else if (task_list[3].taskPriority != 4)
    {
        printf("Add Task fails\n");
        return;
    }
    else if (p_one[0]->taskFunc != task_1)
    {
        printf("Add to priority queue fails\n");
    }
    else if (p_two[0]->taskFunc != task_2)
    {
        printf("Add to priority queue fails\n");
    }
    else if (p_three[0]->taskFunc != task_3)
    {
        printf("Add to priority queue fails\n");
    }
    else if (p_four[0]->taskFunc != task_4)
    {
        printf("Add to priority queue fails\n");
    }
    else
    {
        printf("Add Task passes\n");
        printf("Add to priority queue passes\n");
    }
}

void context_switch()
{
    task_test_global = 0;
    make_test_suite_context();

    current_context = &(p_one[0]->context);
    swapcontext(&test_context, current_context);
    if (task_test_global != 1)
    {
        printf("Context switch failure\n");
    }

    current_context = &(p_two[0]->context);
    swapcontext(&test_context, current_context);
    if (task_test_global != 2)
    {
        printf("Context switch failure\n");
    }

    current_context = &(p_three[0]->context);
    swapcontext(&test_context, current_context);
    if (task_test_global != 3)
    {
        printf("Context switch failure\n");
    }

    current_context = &(p_four[0]->context);
    swapcontext(&test_context, current_context);
    if (task_test_global != 4)
    {
        printf("Context switch failure\n");
    }

    printf("Context switch passes\n");
}

void make_test_suite_context()
{
    getcontext(&scheduler_context);
    scheduler_context.uc_stack.ss_sp = stack;
    scheduler_context.uc_stack.ss_size = sizeof(stack);
    makecontext(&scheduler_context, test_suite, 0);
}

void test_priority_increase()
{
    int setP = 0;
    int increasedP = 0;
    
    addTask(&task_7, 3, 4098);
    signal(SIGALRM, isr);
    int i = 0;
    while (p_three[i] != NULL)
    {
        alarm(3);
        current_context = &(p_three[i]->context);
        if (swapcontext(&scheduler_context, current_context) == -1)
        {
            perror("Swap context");
        }
        if (increase)
        {
            setP = 1;
            int id = p_three[i]->taskid;
            increasePriority(p_three[i]);
            int j =0;
            while(p_four[j] != NULL){
                if(p_four[j]->taskid == id && p_four[j]->taskPriority == 4){
                    increasedP = 1;
                }
                j++;
            }
            i--;
        }
        i++;
    }
    if(setP){
        printf("Success setting priority\n");
    }
    else{
             printf("Failed setting priority\n");
    }
    if(increasedP){
         printf("Success increasing priority\n");
    }
    else{
        printf("Failed increasing priority\n");
    }
}

void test_task_yield()
{
    task_test_global = 0;
    make_test_suite_context();
    task_5();
}

void test_delete()
{
    int setDS = 0;
    int deleteTL = 0;
    int shifted = 0;
    int numInQ = 0;
    addTask(&task_6, 4, 4098);
    signal(SIGALRM, isr);
    int i = 0;
    while (p_four[i] != NULL)
    {
        numInQ++;
        i++;
    }
    i = 0;
    while (p_four[i] != NULL)
    {
        alarm(3);
        current_context = &(p_four[i]->context);
        if (swapcontext(&scheduler_context, current_context) == -1)
        {
            perror("Swap context");
        }
        if (delete)
        {
            setDS = 1;
            deleteFromList(*p_four[i]);
            if (task_list[p_four[i]->taskid].taskPriority == 0)
            {
                deleteTL = 1;
            }
            shift_task(&p_four, i);
            int j = 0;
            while (p_four[j] != NULL)
            {
                numInQ--;
                j++;
            }
            if (numInQ > 0)
            {
                shifted = 1;
            }
            i--;
            delete = 0;
        }
        i++;
    }
    if (setDS)
    {
        printf("Success setting delete\n");
    }
    else
    {
        printf("Failed set delete\n");
    }
    if (shifted)
    {
        printf("Success removing from queue\n");
    }
    else
    {
        printf("Failed removing from queue\n");
    }
    if (deleteTL)
    {
        printf("Success deleting from task list\n");
    }
    else
    {
        printf("Failed deleting from task list\n");
    }
}

void test_isr()
{
    signal(SIGALRM, isr_test_func);
    alarm(2);
    while (!isr_hit);
    printf("ISR callback passes\n");
}

void isr_test_func()
{
    isr_hit = 1;
}

void task_1()
{
    printf("Task 1 Running\n");
    task_test_global = 1;
    swapcontext(current_context, &test_context);
}

void task_2()
{
    printf("Task 2 Running\n");
    task_test_global = 2;
    swapcontext(current_context, &test_context);
}

void task_3()
{
    printf("Task 3 Running\n");
    task_test_global = 3;
    swapcontext(current_context, &test_context);
}

void task_4()
{
    printf("Task 4 Running\n");
    task_test_global = 4;
    swapcontext(current_context, &test_context);
}

void task_5()
{
    printf("Task 5 Running\n");
    task_yield();
    printf("Yield Fail\n");
    return (void)-1;
}

void task_6()
{
    printf("Task 6 Running\n");
    set_delete();
}

void task_7()
{
    printf("Task 7 Running\n");
    setIncreaseP();
}