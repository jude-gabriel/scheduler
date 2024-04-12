#include <stdio.h>
#include <stdlib.h>

#include "tasks.h"
#include "scheduler.h"


int main()
{

    // Initalize the tasks
    initScheduler();

    // Add tasks 
    addTask(&task1, 4, 4098);
    addTask(&task2, 3, 4098);

    // Start the scheduler
    runScheduler();

    // Should never reach here
    return 0;
}
