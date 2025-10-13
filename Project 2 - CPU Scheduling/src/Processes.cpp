#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>
#include "Processes.h"

//Function to create a new process and initialize with random values
Process createProcess(std::mt19937& gen){
    //int from 0 to 99 inclusive
    std::uniform_int_distribution<int> arrival(0, 99);

    //int from 1 to 10 inclusive
    std::uniform_int_distribution<int> runtime(1, 10);
    
    //int from 1 to 4 inclusive
    std::uniform_int_distribution<int> prio(1, 4);
    
    //Generate Numbers
    int arrivalTime = arrival(gen);
    int runTime = runtime(gen);
    int priority = prio(gen);

    return Process(arrivalTime,runTime,priority);
}

//Logic for sorting processes.  If true, proc1 goes before proc2
bool arrivaltimeSort(const Process& proc1, const Process& proc2){
    return proc1.arrivalTime < proc2.arrivalTime;
}

//Function to return a queue of processes sorted by arrival time
std::queue<Process> createProcessQueue(int numProcesses, uint32_t seed){

    //Input checking
    if(numProcesses <= 0 || numProcesses > 26){
        std::cerr << "\033[31m" <<"ERROR: INVALID NUMBER OF PROCESSES.  MUST ENTER A POSITIVE NUMBER AND LESS THAN 27." << "\033[0m" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Create a Vector which can be sorted with std::sort
    std::vector<Process> procs;

    //Logic to assign alphabetical id's to each process
    int iterator = 0;

    //Logic to either select random or set seed.
    uint32_t generator_seed;
    if(seed == UINT_MAX){
        //Random number generator to produce a seed for the number generator below
        std::random_device rd;
        generator_seed = rd();
    }else{
        generator_seed = seed;
    }

    //Mersenne Twister pseudo-random generator of 32-bit numbers
    std::mt19937 gen(generator_seed);

    for(char id = 'A'; id <= 'Z'; id++){

        //Create process with random values
        Process proc = createProcess(gen);
        proc.id = id;
        std::cout<<"new process "<<id<<std::endl;
        std::cout<<"\tarrival, run: "<<proc.arrivalTime<<", "<<proc.expectedRunTime<<std::endl;

        procs.push_back(proc);

        iterator++;

        if(iterator == numProcesses){
            break;
        }     
    }

    //Sort the Vector in-place by arrival time.
    std::sort(procs.begin(), procs.end(), &arrivaltimeSort);

    //Create output queue
    std::queue<Process> procsout;
    
    //Copy the Vector into the queue
    for(int i = 0; i < numProcesses; i++){
        procsout.push(procs[i]);
    }

    return procsout;
}

void Process::printProcessData(){
    std::cout << "\n====== Process Data ======" << std::endl;
    std::cout <<"Arrival Time:"<<std::setw(13)<<getarrivalTime()<< std::endl <<
                "--------------------------"<<std::endl<<
                "Completion Time:"<<std::setw(10)<<getcompletionTime()<< std::endl <<
                "--------------------------"<<std::endl<<
                "Runtime:"<<std::setw(18)<<getexpectedRunTime()<< std::endl <<
                "--------------------------"<<std::endl<<
                "Priority:"<<std::setw(17)<<getpriority()<< std::endl <<
                "--------------------------"<<std::endl<<
                "id:"<<std::setw(23)<<getid()<< std::endl <<
                "--------------------------"<<std::endl<<
                "Start Time:"<<std::setw(15)<<getstartTime()<< std::endl <<
                "--------------------------"<<std::endl<<
                "TAT:"<<std::setw(22)<<getturnaroundTime()<< std::endl <<
                "--------------------------"<<std::endl<<
                "Response Time:"<<std::setw(12)<<getresponseTime()<< std::endl <<
                "--------------------------"<<std::endl<<
                "Wait Time:"<<std::setw(16)<<getwaitTime()<< std::endl<<
                "--------------------------"<<std::endl;
}

//Function to print final results of the processes
void printResults(std::vector<Process> finishedJobs){
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Process\tArrival\tRuntime\tPriority\tStart\tFinish\tTurnaround\tResponse\tWait" << std::endl;
    std::cout << "-------\t-------\t-------\t--------\t-----\t------\t----------\t--------\t----" << std::endl;
    
    int totalTurnaround = 0, totalResponse = 0, totalWait = 0;
    for(const Process& proc : finishedJobs){
        int turnaround = proc.completionTime - proc.arrivalTime;
        int response = proc.startTime - proc.arrivalTime;
        int wait = turnaround - (proc.completionTime - proc.startTime);
        
        totalTurnaround += turnaround;
        totalResponse += response;
        totalWait += wait;
        
        printf("%c\t%.1i\t%.1i\t%.1i\t\t%.1i\t%.1i\t%.1i\t\t%.1i\t\t%.1i\n", 
               proc.id, proc.arrivalTime, proc.completionTime - proc.startTime, proc.priority, 
               proc.startTime, proc.completionTime, turnaround, response, wait);
    }
    
    if(!finishedJobs.empty()){
        std::cout << "\nAverages:" << std::endl;
        std::cout << "Turnaround Time: " << totalTurnaround / static_cast<double>(finishedJobs.size()) << std::endl;
        std::cout << "Response Time: " << totalResponse / static_cast<double>(finishedJobs.size()) << std::endl;
        std::cout << "Wait Time: " << totalWait / static_cast<double>(finishedJobs.size()) << std::endl;
        std::cout << "Throughput: " << finishedJobs.size() / static_cast<double>(finishedJobs.back().completionTime) << std::endl;
    }
}
void simulateScheduling(std::vector<Process> (*fun)(std::queue<Process>),int procs){

    std::vector<Process> completedJobs;
    std::vector<Process> tempJobs;
    int totalRunTime = 0;
    int totalProcsCompleted = 0;

    for(int i=0; i<5; i++){
        std::queue<Process> processors = createProcessQueue(procs);

        tempJobs = fun(processors);

        totalRunTime = totalRunTime + tempJobs.back().completionTime;
        totalProcsCompleted = tempJobs.size();

        completedJobs.insert(completedJobs.end(),tempJobs.begin(),tempJobs.end());
    }

    std::cout << "\n=== Results ===" << std::endl;
   
    int totalTurnaround = 0, totalResponse = 0, totalWait = 0;
    for(const Process& proc : completedJobs){
        int turnaround = proc.completionTime - proc.arrivalTime;
        int response = proc.startTime - proc.arrivalTime;
        int wait = turnaround - (proc.completionTime - proc.startTime);
        
        totalTurnaround += turnaround;
        totalResponse += response;
        totalWait += wait;   
    }
    
    if(!completedJobs.empty()){
        std::cout << "\nAverages:" << std::endl;
        std::cout << "Turnaround Time: " << totalTurnaround / static_cast<double>(completedJobs.size()) << std::endl;
        std::cout << "Response Time: " << totalResponse / static_cast<double>(completedJobs.size()) << std::endl;
        std::cout << "Wait Time: " << totalWait / static_cast<double>(completedJobs.size()) << std::endl;
        std::cout << "Throughput: " << totalProcsCompleted / static_cast<double>(totalRunTime) << std::endl;
    }
}