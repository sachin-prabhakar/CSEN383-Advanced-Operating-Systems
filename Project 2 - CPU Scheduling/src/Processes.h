#pragma once
#include <random>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

//Data structure for each process
struct Process {

    int arrivalTime = -1;       //Value obtained at Process creation
	int completionTime = -1;    //Value obtained at Process completion
	int expectedRunTime = -1;   //Value obtained at Process creation
    int priority = -1;          //Value obtained at Process creation
    char id = '\0';             //Value obtained at Process creation

	int startTime = -1;         //Value obtained when Process starts
	int turnaroundTime = -1;    //Value obtained at Process completion
	int responseTime = -1;      //Value obtained at Process completion
	int waitTime = -1;          //Value obtained at Process completion

    //Default constructor
    Process() = default;

    //Overloaded constructor for ProcessCreation function
    Process(int at, int rt, int p) : arrivalTime(at), expectedRunTime(rt), priority(p){}

    //Getters for every value
    int getarrivalTime(){return arrivalTime;}
    int getcompletionTime(){return completionTime;}
    int getexpectedRunTime(){return expectedRunTime;}
    int getpriority(){return priority;}
    char getid(){return id;}
    int getstartTime(){return startTime;}
    int getturnaroundTime(){return turnaroundTime;}
    int getresponseTime(){return responseTime;}
    int getwaitTime(){return waitTime;}

    //Setters for values that the user can modify
    void setcompletionTime(int x){completionTime = x;}
    void setstartTime(int x){if(startTime < 0){startTime = x;}else{exit(1);};}
    void setturnaroundTime(int x){turnaroundTime = x;}
    void setresponseTime(int x){responseTime = x;}
    void setwaitTime(int x){waitTime = x;}
  
    //Function to print all data relevant to a process
    void printProcessData();
};

//Function to create a new process and initialize with random values
Process createProcess(std::mt19937& gen1);

//Logic for sorting processes.  If true, proc1 goes before proc2
bool arrivaltimeSort(const Process& proc1, const Process& proc2);

//Function to return a queue of processes sorted by arrival time
std::queue<Process> createProcessQueue(int numProcesses, uint32_t seed = UINT_MAX);

//Function to print final results of the processes
void printResults(std::vector<Process> finishedJobs);

//Function to run process N times and calculate averages
void simulateScheduling(std::vector<Process> (*fun)(std::queue<Process>), int procs = 15);