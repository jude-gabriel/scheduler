#pragma once

#define SWITCH_PIN 22
#define RED_PIN 23
#define GREEN_PIN 24
#define YELLOW_PIN 25

//#define RPI_TESTS
#define WIN_TESTS

typedef enum STATE
{
    INIT,
    RUNNING,
    STOPPED
} STATE;


void task1();
void task2();
void task3();
void task4();
void task5();

void set_pins();
void switch_task();
void led_red_task();
void led_green_task();
void led_yellow_task();

