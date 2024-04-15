#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h> 
#include "tasks.h"

STATE sys_state = IDLE;

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
    pinMode(SERVO_PIN, OUTPUT);
}

void switch_task()
{
    while(1)
    {
        if(digitalRead(SWITCH_PIN) == HIGH)
        {
            sys_state = RUNNING;
        }
        else
        {
            sys_state = RUNNING;
        }
    }
}

void led_red_task()
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

void led_green_task()
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

void servo_task()
{
    switch(sys_state)
    {
        case INIT:
        {
            digitalWrite(SERVO_PIN, LOW);
            break;
        }
        case RUNNING:
        {
            digitalWrite(SERVO_PIN, HIGH);
            break;
        }
        case STOPPED:
        {
            digitalWrite(SERVO_PIN, LOW);
            break;
        }
        default:
        {
            digitalWrite(SERVO_PIN, LOW);
            break;
        }
    }
}