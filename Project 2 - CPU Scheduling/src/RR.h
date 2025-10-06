#pragma once
/*
Round Robin

Sachin

*/

#include <queue>
#include <vector>
#include "Processes.h"
#include <iostream>

int RR(std::queue<Process> processes, int timeQuantum = 2){
    int quanta = 0;
    std::queue<Process> readyQueue;
    std::vector<Process> finishedJobs;
    Process running;
    bool isRunning = false;
    int remainingTime = 0;

    std::cout << "\nRound Robin Scheduling (Time Quantum: " << timeQuantum << ") " << std::endl;

    while(true){
        std::cout << "Time Slice: " << quanta << std::endl;
        
        // Add new processes to ready queue
        while(!processes.empty() && processes.front().arrivalTime <= quanta && quanta < 100){
            std::cout << "Adding process " << processes.front().id << " to ready queue at time " << quanta << std::endl;
            readyQueue.push(processes.front());
            processes.pop();
        }

        // Get next process if none running
        if(!isRunning && !readyQueue.empty()){
            running = readyQueue.front();
            readyQueue.pop();
            
            if(running.startTime < 0){
                running.setstartTime(quanta);
                std::cout << "Process " << running.id << " started at time " << quanta << std::endl;
            }
            
            remainingTime = timeQuantum;
            isRunning = true;
            std::cout << "Now running process: " << running.id << " (remaining time: " << running.expectedRunTime << ")" << std::endl;
        }

        // Execute current process
        if(isRunning){
            running.expectedRunTime -= 1;
            remainingTime--;
            
            std::cout << "Executing " << running.id << " (remaining: " << running.expectedRunTime << ")" << std::endl;
            
            // Check if done or time up
            if(running.expectedRunTime <= 0){
                running.setcompletionTime(quanta + 1);
                finishedJobs.push_back(running);
                std::cout << "Process " << running.id << " finished at time " << (quanta + 1) << std::endl;
                isRunning = false;
            }
            else if(remainingTime <= 0){
                std::cout << "Time quantum expired for " << running.id << ", adding back to ready queue" << std::endl;
                readyQueue.push(running);
                isRunning = false;
            }
        }
        else{
            std::cout << "CPU idle" << std::endl;
        }

        quanta++;
        
        // Stop if all done
        if(quanta >= 100 && readyQueue.empty() && !isRunning){
            std::cout << "Simulation ended at time " << quanta << std::endl;
            break;
        }
    }

    // Show results using shared utility
    printResults(finishedJobs);

    return 1;
}
