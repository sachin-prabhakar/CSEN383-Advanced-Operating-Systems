#pragma once
#include <queue>
#include <vector>
#include "Processes.h"
#include <iostream>
#include <cmath>
/*
Shortest Job First - Liam
*/

//Used for sorting in decending order
bool operator<(const Process& a, const Process& b){
    return a.expectedRunTime > b.expectedRunTime;
}

//Function to check to see if any processes have arrived, and if they have they are added to the ready queue
void checkForArrivingProcesses(std::queue<Process> &p, int &q, std::priority_queue<Process> &rq){
    while(!p.empty() && p.front().arrivalTime <= q && q < 99){
        Process temp = p.front();
        rq.push(temp);
        p.pop();
        std::cout<<"\e[1;32m"<<"Process "<<temp.id<<" added to ready queue"<<"\e[0m"<<std::endl;
    }
}

std::vector<Process> SJF(std::queue<Process> processes){
    int quanta = 0;
    std::priority_queue<Process> readyQueue;
	std::vector<Process> finishedJobs;
    Process running;

    while(true){
        //Update ready queue if needed
        checkForArrivingProcesses(processes, quanta, readyQueue);

        //Start of next quanta
        std::cout<<"\e[1;37m"<<"Time Slice: "<<quanta<<"\e[0m";

        //If there is a process ready to be run and time quanta hasn't passed 99 run a process
        if(!readyQueue.empty() && quanta <= 99){
            std::cout<<"\e[1;32m"<<"\tCPU Available"<<"\e[0m"<<std::endl;
            //Get process with shortest runtime
            running = readyQueue.top();

            //Remove that process from ready queue
            readyQueue.pop();

            //Save start time of process
            running.setstartTime(quanta);

            int finishtime = quanta + running.getexpectedRunTime();

            //Run the process until it finishes
            while(quanta < finishtime){
                std::cout<<"\e[1;37m"<<"Time Slice: "<<quanta<< "\e[1;33m"<<"\tCPU Running "<<"\e[1;34m"<<"Executing process: "<<running.id<<"\e[0m"<<std::endl;
                quanta = quanta + 1;  
                //Update ready queue if needed
                checkForArrivingProcesses(processes, quanta, readyQueue);
            }
          
            //Clean up running process
            running.setcompletionTime(quanta);
            finishedJobs.push_back(running);
            std::cout<<"\e[1;32m"<<"Process "<<running.id<<" finished executing "<<"\e[0m"<<std::endl;
        }else{
            //No processess to run and time quanta passed 99 so break.
            if(quanta > 99){
                std::cout<<"\e[1;37m"<<"\t Scheduling Finished"<<"\e[0m"<<std::endl;
                printResults(finishedJobs);
                break;
            }
            std::cout<<"\e[1;31m"<<"\tCPU Idle"<<"\e[0m"<<std::endl;
            quanta = quanta + 1;     
            }
    } //End while loop

    return finishedJobs;
}