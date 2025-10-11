//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//Scheduling Algs
#include "FCFS.h"
#include "SJF.h"
#include "SRT.h"
#include "RR.h"
#include "HPF.h"

//Other Functionality
#include "Processes.h"

void testingQueue(int numProcesses){
    //Create a specific number of processes and get them in a queue

    //Optional parameter for createProcessQueue().  Remove if you want randomness.
    uint32_t seed = 444;
    std::queue<Process> processors = createProcessQueue(numProcesses, seed);

    //Print Size of Queue
    std::cout<<"Queue size is "<<processors.size()<<std::endl;

    //Testing the queue
    while(!processors.empty()){
        Process proc1 = processors.front();
        printf("------------------------------------\n");
        printf("Arrival time %d\n", proc1.arrivalTime);
        printf("Run time %d\n", proc1.expectedRunTime);
        printf("Priority %d\n", proc1.priority);
        printf("ID %c\n\n", proc1.id);
        processors.pop();
    }
}


int main() {

    //Number of processes to be created for the scheduling algorithms.  Increase this value if CPU is idle for longer than 2 quanta.
    int numProcesses = 15;

    /*
    Each algorithm, ran as a function, will be run 5 times.

    We need to implement calculating the averages across the 5 runs, not just individually.
    We also need to calculate throughput.
    */
    //for(int iteration = 0; iteration < 5; iteration++){

        //Generate random processes
        uint32_t seed = 444; 
        std::queue<Process> processors = createProcessQueue(numProcesses, seed);
        
        //FCFS(processors);
        //RR(processors, 2);  // Test Round Robin with time quantum of 2
    //}
    simulateScheduling(&SJF);

    return 0;
}