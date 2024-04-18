#include <stdio.h>
#include <stdlib.h>
#include "tasks.h"
#include "scheduler.h"

#ifdef RPI_TESTS
#include <wiringPi.h> 
#endif

STATE sys_state = INIT;



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
    while(1)
    {
        printf("Task 2: %d\n", i);
      
        i++;
        if(i > 100000){
           set_delete();
        }
    }
}

void task3()
{
    int i = 0;
    
    while(1)
    {
        printf("Task 3: %d\n", i);
        if(i % 10000 == 0)
            setIncreaseP();
        i++;
    }
    return;
}

void task4()
{
    int i = 0;    
    while(1)
    {
        printf("Task 4: %d\n", i);
        if(i < 1)
            task_yield();
        i++;
    }
    return;
}

void task5()
{
    int i = 0;    
    while(1)
    {
        printf("Task 5: %d\n", i);
        i++;
    }
    return;
}



void set_pins()
{
#ifdef RPI_TESTS
    wiringPiSetup();
    pinMode(SWITCH_PIN, INPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
#endif
}

void switch_task()
{
#ifdef RPI_TESTS
    while(1)
    {
        if(digitalRead(SWITCH_PIN) == HIGH)
        {
            sys_state = RUNNING;
            printf("RUNNING\n");
        }
        else
        {
            sys_state = STOPPED;
            printf("STOPPED\n");
        }
    }
#endif
}

void led_red_task()
{
#ifdef RPI_TESTS
    while(1)
    {
        switch(sys_state)
        {
            case INIT:
            {
                digitalWrite(RED_PIN, HIGH);
                break;
            }
            case RUNNING:
            {
                digitalWrite(RED_PIN, HIGH);
                printf("writing high\n");
                break;
            }
            case STOPPED:
            {
                digitalWrite(RED_PIN, LOW);
                break;
            }
            default:
            {
                digitalWrite(RED_PIN, LOW);
                break;
            }
        }
    }
#endif
}

void led_green_task()
{
#ifdef RPI_TESTS
    while(1)
    {
        switch(sys_state)
        {
            case INIT:
            {
                digitalWrite(GREEN_PIN, HIGH);
                break;
            }
            case RUNNING:
            {
                digitalWrite(GREEN_PIN, LOW);
                break;
            }
            case STOPPED:
            {
                digitalWrite(GREEN_PIN, HIGH);
                break;
            }
            default:
            {
                digitalWrite(RED_PIN, LOW);
                break;
            }
        }
    }
#endif
}

void led_yellow_task()
{
#ifdef RPI_TESTS
    while(1)
    {
        switch(sys_state)
        {
            case INIT:
            {
                digitalWrite(YELLOW_PIN, LOW);
                break;
            }
            case RUNNING:
            {
                digitalWrite(YELLOW_PIN, HIGH);
                delay(500);
                break;
            }
            case STOPPED:
            {
                digitalWrite(YELLOW_PIN, LOW);
                break;
            }
            default:
            {
                digitalWrite(YELLOW_PIN, LOW);
                break;
            }
        }
    }
#endif
}
