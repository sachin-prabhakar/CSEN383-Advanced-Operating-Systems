#pragma once
#include <random>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

//Data structure for each process
struct Process {

    float arrivalTime;
	float completionTime;
	float expectedRunTime;
	float initialExpectedRunTime;	// this is just to confirm wait time
    int priority;
    char id;

	float startTime;
	float turnaroundTime;
	float responseTime;
	float waitTime;
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