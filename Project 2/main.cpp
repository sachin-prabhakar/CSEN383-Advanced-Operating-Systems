#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "FCFS.h"
#include "SJF.h"
#include "SRT.h"
#include "RR.h"
#include "HPF.h"
#include "Processes.h"


int main() {

    //Create a specific number of processes and get them in a queue
    int numProcesses = 5;  
    std::queue<Process> processors = createProcessQueue(numProcesses);

    //Print Size of Queue
    std::cout<<"Queue size is "<<processors.size()<<std::endl;

    //Testing the queue
    while(!processors.empty()){
        Process proc1 = processors.front();
        printf("------------------------------------\n");
        printf("Arrival time %f\n", proc1.arrivalTime);
        printf("Run time %f\n", proc1.expectedRunTime);
        printf("Priority %d\n", proc1.priority);
        printf("ID %c\n\n", proc1.id);
        processors.pop();
    }

    //loop for running each algorithm 5 times
    // for(int iteration = 0; iteration < 5; iteration++){

    //     int numProcesses = 5;  
    //     std::queue<Process> processors = createProcessQueue(numProcesses);
     
    //     //loop to simulate 100 time slices
    //     for(int timeSlice = 0; timeSlice < 100; timeSlice++){

    //     }

    // }
    return 0;
}

