#include <iostream>
#include <queue>
#include <vector>
#include "FCFS.h"

int FCFS(std::queue<Process> processes) {
    int quanta = 0;
    std::vector<Process> finishedJobs;
    Process running;
    bool isRunning = false;
    int remainingTime = 0;

    std::cout << "\nFCFS Scheduling" << std::endl;

    while(true) {
        std::cout << "Time Slice: " << quanta << std::endl;
        
        // Get next process if none running and processes available
        if(!isRunning && !processes.empty() && processes.front().arrivalTime <= quanta) {
            running = processes.front();
            processes.pop();
            
            if(running.startTime < 0) {
                running.setstartTime(quanta);
                std::cout << "Process " << running.id << " started at time " << quanta << std::endl;
            }
            
            remainingTime = running.expectedRunTime;
            isRunning = true;
            std::cout << "Now running process: " << running.id << " (runtime: " << running.expectedRunTime << ")" << std::endl;
        }

        // Execute current process
        if(isRunning) {
            remainingTime--;
            std::cout << "Executing " << running.id << " (remaining: " << remainingTime << ")" << std::endl;
            
            // Check if done
            if(remainingTime <= 0) {
                running.setcompletionTime(quanta + 1);
                finishedJobs.push_back(running);
                std::cout << "Process " << running.id << " finished at time " << (quanta + 1) << std::endl;
                isRunning = false;
            }
        }
        else {
            std::cout << "CPU idle" << std::endl;
        }

        quanta++;
        
        // Stop if all done or past quantum 99
        if(quanta > 99 && processes.empty() && !isRunning) {
            std::cout << "Simulation ended at time " << quanta << std::endl;
            break;
        }
    }

    // Show results using shared utility
    completeJobs(finishedJobs);
    printResults(finishedJobs);
    return 1;
}
