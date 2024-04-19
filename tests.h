# pragma once

//#define TEST_SUITE

// Test Suite functions
void test_suite();
void add_tasks();
void context_switch();
void test_isr();
void test_scheduler();
void test_task_yield();
void test_delete();
void test_priority_increase();


// ISR function
void isr_test_func();

// Task functions
void task_1();
void task_2();
void task_3();
void task_4();
void task_5();
void task_6();
void task_7();

// Helper functions
void make_test_suite_context();
