//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

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


int main(int argc, char *argv[]) {

    if (argc > 3 || argc < 2) {
        std::cout<<"USAGE: ./main <algorithm> [numprocesses]"<<std::endl;
        return 1;
    }
    //Number of processes to be created for the scheduling algorithms.  Increase this value if CPU is idle for longer than 2 quanta.
    int numProcesses = argc == 3 ? std::stoi(argv[2]) : 15;
    std::string alg = argv[1];

    std::cout<<"alg = "<<alg<<"\nnumprocs = "<<numProcesses<<std::endl;

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
        // SJF(processors);
        SRT(processors);

        if (alg == "fcfs")      FCFS(processors);
        else if (alg == "rr")   RR(processors, 2);
        else if (alg == "sjf")  SJF(processors);
        else if (alg == "srt")  SRT(processors);
        else if (alg == "hpfp")  HPF_preemptive(processors);

        else {
            std::cout<<"please try again; unreadable alg"<<std::endl;
            return 1;
        }


    //}
    
    // simulateScheduling(&SJF);

    return 0;
}
