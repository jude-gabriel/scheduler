#include "tests.h"
#include "tasks.h"
#include "scheduler.h"
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

    printf("Test suite done\n");
}

void add_tasks()
{
    initScheduler();
    addTask(&task_1, 1, 4098);
    addTask(&task_2, 2, 4098);
    addTask(&task_3, 3, 4098);
    addTask(&task_4, 4, 4098);

    if(task_list[0].taskPriority != 1)
    {
        printf("Add Task fails\n");
        return;
    }
    else if(task_list[1].taskPriority != 2)
    {
        printf("Add Task fails\n");
        return;
    }
    else if(task_list[2].taskPriority != 3)
    {
        printf("Add Task fails\n");
        return;
    }
    else if(task_list[3].taskPriority != 4)
    {
        printf("Add Task fails\n");
        return;
    }
    else if(p_one[0]->taskFunc != task_1)
    {
        printf("Add to priority queue fails\n");
    }
    else if(p_two[0]->taskFunc != task_2)
    {
        printf("Add to priority queue fails\n");
    }
    else if(p_three[0]->taskFunc != task_3)
    {
        printf("Add to priority queue fails\n");
    }
    else if(p_four[0]->taskFunc != task_4)
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
    if(task_test_global != 1)
    {
        printf("Context switch failure\n");
    }

    current_context = &(p_two[0]->context);
    swapcontext(&test_context, current_context);
    if(task_test_global != 2)
    {
        printf("Context switch failure\n");
    }

    current_context = &(p_three[0]->context);
    swapcontext(&test_context, current_context);
    if(task_test_global != 3)
    {
        printf("Context switch failure\n");
    }

    current_context = &(p_four[0]->context);
    swapcontext(&test_context, current_context);
    if(task_test_global != 4)
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

void test_task_deletion(){

}

void test_task_yield(){
    task_test_global = 0;
    make_test_suite_context();
    task_5();

}

void test_isr()
{
    signal(SIGALRM, isr_test_func);
    alarm(2);
    while(!isr_hit);
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

void task_5(){
    printf("Task 5 Running\n");
    task_yield();
    printf("Yield Fail\n");
    return (void) -1;
}