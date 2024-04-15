#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "tasks.h"
#include "scheduler.h"


// #define TEST_SUITE

int main()
{

#ifdef TEST_SUITE
    test_suite();
#endif

    // Initalize the tasks
    initScheduler();

    // Add tasks 
    // addTask(&task1, 4, 4098);
    // addTask(&task2, 3, 4098);

    set_pins();
    addTask(&switch_task, 4, 4098);
    addTask(&led_red_task, 2, 4098);
    addTask(&led_green_task, 1, 4098);
    addTask(&led_yellow_task, 3, 4098);

    // Start the scheduler
    runScheduler();

    // Should never reach here
    return 0;
}
