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

    // if (argc > 4 || argc < 2) {
    //     std::cout<<"USAGE: ./main <algorithm> [numprocesses] [seed]"<<std::endl;
    //     return 1;
    // }
    //Number of processes to be created for the scheduling algorithms.  Increase this value if CPU is idle for longer than 2 quanta.
    //int numProcesses = argc == 3 ? std::stoi(argv[2]) : 15;
    //std::string alg = argv[1];

    //Generate random processes
    //uint32_t seed = argc==4 ? (std::stoi(argv[3])<0 ? UINT32_MAX : (uint32_t)std::stoi(argv[3])) : 444; 
    int numProcesses = 15;
    std::queue<Process> processors = createProcessQueue(numProcesses);
    
    // if (alg == "fcfs")      simulateScheduling(&FCFS,numProcesses);
    // else if (alg == "rr")   simulateScheduling(&RR,numProcesses);
    // else if (alg == "sjf")  simulateScheduling(&SJF,numProcesses);
    // else if (alg == "srt")  simulateScheduling(&SRT,numProcesses);
    // else if (alg == "hpfp") simulateHPF(&HPF_preemptive, true, numProcesses);
    // else if (alg == "hpfn") simulateHPF(&HPF_nonpreemptive, true, numProcesses);
    // else {
    //     std::cout<<"please try again; unreadable alg"<<std::endl;
    //     return 1;
    // }

    simulateScheduling(&FCFS,numProcesses);
    simulateScheduling(&RR,numProcesses);
    simulateScheduling(&SJF,numProcesses);
    simulateScheduling(&SRT,numProcesses);
    simulateHPF(&HPF_preemptive, true, numProcesses);
    simulateHPF(&HPF_nonpreemptive, true, numProcesses);

    return 0;
}
