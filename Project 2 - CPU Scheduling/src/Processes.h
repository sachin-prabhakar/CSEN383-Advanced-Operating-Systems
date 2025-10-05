#pragma once
#include <random>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

//Data structure for each process
struct Process {

    float arrivalTime;
	float completionTime = -1;
	float expectedRunTime;
	//float initialExpectedRunTime;	// this is just to confirm wait time
    int priority;
    char id;

	float startTime = -1;
	float turnaroundTime = -1;
	float responseTime = -1;
	float waitTime = 0;

    float getarrivalTime(){return arrivalTime;}
    float getcompletionTime(){return completionTime;}
    float getexpectedRunTime(){return expectedRunTime;}
    //float getinitialExpectedRunTime(){return initialExpectedRunTime;}
    int getpriority(){return priority;}
    char getid(){return id;}
    float getstartTime(){return startTime;}
    float getturnaroundTime(){return turnaroundTime;}
    float getresponseTime(){return responseTime;}
    float getwaitTime(){return waitTime;}


    void setcompletionTime(float x){completionTime = x;}
    //void setinitialExpectedRunTime(float x){initialExpectedRunTime = x;}
    void setstartTime(float x){if(startTime < 0){startTime = x;}else{exit(1);};}
    void setturnaroundTime(float x){turnaroundTime = x;}
    void setresponseTime(float x){responseTime = x;}
    void setwaitTime(float x){waitTime = x;}
  
    //for when job actually finishes executing, calculate turnaround time, etc.
    void finish(float time);
};

Process createProcess(std::mt19937& gen);
bool arrivaltimeSort(const Process& proc1, const Process& proc2);
std::queue<Process> createProcessQueue(int numProcesses, uint32_t seed = UINT_MAX);

/*

WARNING: Depreciated

std::vector<Process> create_Processlist(int numProcesses){

    std::vector<Process> procs;
    int iterator = 0;

    for(char id = 'A'; id < 'Z'; id++){
        Process proc = createProcess();
        proc.id = id;

        procs.push_back(proc);

        iterator++;

        if(iterator == numProcesses){
            break;
        }     
    }

    std::sort(procs.begin(), procs.end(), &arrivaltimeSort);

    return procs;
}

std::queue<Process> create_ProcessQueue(int numProcesses){

    std::queue<Process> procs;
    int iterator = 0;

    for(char id = 'A'; id < 'Z'; id++){
        Process proc = createProcess();
        proc.id = id;

        procs.push(proc);

        iterator++;

        if(iterator == numProcesses){
            break;
        }     
    }

    return procs;
}
*/

//#endif