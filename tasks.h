#pragma once

#define SWITCH_PIN 31
#define RED_PIN 33
#define GREEN_PIN 35
#define SERVO_PIN 37

typedef enum STATE
{
    INIT,
    RUNNING,
    STOPPED
} STATE;


void task1();
void task2();

void set_pins();
void switch_task();
void led_red_task();
void led_green_task();
void servo_task();

