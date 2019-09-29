#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "admin.h"

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        kernel->currentThread->Yield();
    }
}

void
ThreadTest()
{
    printf("i am here 1\n");
   admin TrainSimulator;
   TrainSimulator.startSimulate();
}
