#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h> 
#include "tasks.h"

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
    }
    return;
}

void set_pins()
{
    wiringPiSetup();
    pinMode(SWITCH_PIN, INPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
}

void switch_task()
{
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
}

void led_red_task()
{
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
    
}

void led_green_task()
{
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
}

void led_yellow_task()
{
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
}
