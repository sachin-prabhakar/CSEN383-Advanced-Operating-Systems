#pragma once
#include <queue>
#include <vector>
#include "Processes.h"
#include <iostream>
#include <cmath>
/*
Shortest Job First

Liam

*/

bool operator<(const Process& a, const Process& b){
    return a.arrivalTime < b.arrivalTime;
}

int SJF(std::queue<Process> processes){
    int quanta = 0;
    //int numprocs = processes.size();
    std::priority_queue<Process> readyQueue;
	std::vector<Process> finishedJobs;
    Process running;



    //while(!processes.empty() && finishedJobs.size() != numprocs){
    while(true){

        std::cout<<"Time Slice: "<<quanta<<std::endl;
        
        while(!processes.empty() && processes.front().arrivalTime < quanta && quanta < 99){
            std::cout<<"Adding a process to the ready queue at time "<<quanta<<std::endl;
            readyQueue.push(processes.front());
            processes.pop();

        }

        if(!readyQueue.empty()){
            running = readyQueue.top();
            running.setstartTime(quanta);

            std::cout<<"Now running process: "<<running.id<<" at time "<<quanta<<std::endl;
            std::cout<<"Expected finish time is "<<quanta + running.getexpectedRunTime()<<std::endl;
            float finishtime = quanta + running.getexpectedRunTime();
            while(quanta < finishtime){
                quanta = quanta + 1;
            }

            readyQueue.pop();
            running.setcompletionTime(quanta);
            finishedJobs.push_back(running);
            std::cout<<"Process: "<<running.id<<" finished at time "<<quanta<<", runtime was "<<quanta-running.getstartTime()<<std::endl;
            
            
        }else{
            if(quanta > 99 ){
                break;
            }

            quanta = quanta + 1;
        }
        
    }
    return 1;
}