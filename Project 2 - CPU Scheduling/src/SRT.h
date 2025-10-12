#pragma once
/*
Shortest Remaining Time
Ethan
*/

#include <queue>
#include "Processes.h"


// struct Job {
// 	float arrivalTime;
// 	float completionTime;
// 	float expectedRunTime;
// 	float initialExpectedRunTime;	// this is just to confirm wait time
//     int priority;
//     char id;

// 	float startTime;
// 	float turnaroundTime;
// 	float responseTime;
// 	float waitTime;

//     // constructor for when job enters ready queue
//     Job(float arrivalTime, int priority, char id, float expectedRunTime, float startTime = -1);
//     Job(Process proc);
    
//     // sets the start time of process (happens once)
// 	float setStartTime(float time) ;
//     // {   return this.startTime = (this.startTime < 0 ? time : this.startTime);   }

//     //for when job actually finishes executing, calculate turnaround time, etc.
//     void finish(float time);
// };

// Logic for sorting processes.  If true, proc1 goes after proc2	*double check this
// sorted in descending order to make popping easier
bool remainingTimeSort(const Process& job1, const Process& job2);

int SRT(std::queue<Process> &processes);
