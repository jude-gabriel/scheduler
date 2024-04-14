#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "tasks.h"
#include "scheduler.h"


#define TEST_SUITE

int main()
{

#ifdef TEST_SUITE
    test_suite();
#elif

    // Initalize the tasks
    initScheduler();

    // Add tasks 
    addTask(&task1, 4, 4098);
    addTask(&task2, 3, 4098);

    // Start the scheduler
    runScheduler();
#endif
    // Should never reach here
    return 0;
}
