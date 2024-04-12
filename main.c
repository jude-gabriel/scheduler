#include <stdio.h>
#include <stdlib.h>

#include "tasks.h"
#include "scheduler.h"


int main()
{
    // Add tasks 
    addTask(&task1, 1, 4098);
    addTask(&task2, 1, 4098);

    // Start the scheduler
    runScheduler();

    // Should never reach here
    return 0;
}
