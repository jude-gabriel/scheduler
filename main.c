#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "tasks.h"
#include "scheduler.h"


int main()
{

#ifdef TEST_SUITE
    test_suite();
#endif

    // Initalize the tasks
    initScheduler();

#ifdef WIN_TESTS
    // Add tasks 
    addTask(&task1, 4, 4098);
    addTask(&task2, 3, 4098);
    addTask(&task3, 1, 4098);
    addTask(&task4, 4, 4098);
    addTask(&task5, 1, 4098);

#endif

#ifdef RPI_TESTS
    set_pins();
    addTask(&switch_task, 4, 4098);
    addTask(&led_red_task, 2, 4098);
    addTask(&led_green_task, 1, 4098);
    addTask(&led_yellow_task, 3, 4098);
#endif

    // Start the scheduler
    runScheduler();

    // Should never reach here
    return 0;
}
