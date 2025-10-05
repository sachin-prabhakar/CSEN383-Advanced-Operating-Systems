// ethan shenassa
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "SRT.h"

// constructor for when job enters ready queue
Job::Job(float arrivalTime, int priority, char id, float expectedRunTime, float startTime) {
	this->arrivalTime = arrivalTime;
	this->completionTime = -1;
	this->expectedRunTime = expectedRunTime;
	this->initialExpectedRunTime = expectedRunTime;
	this->priority = priority;
	this->id = id;

	this->startTime = startTime;
	this->turnaroundTime = -1;
	this->responseTime = -1;
	this->waitTime = 0;
}

Job::Job(Process proc) {
	this->arrivalTime = proc.arrivalTime;
	this->completionTime = -1;
	this->expectedRunTime = proc.expectedRunTime;
	this->initialExpectedRunTime = proc.expectedRunTime;
	this->priority = proc.priority;
	this->id = proc.id;

	this->startTime = -1;
	this->turnaroundTime = -1;
	this->responseTime = -1;
	this->waitTime = 0;
	
}

// sets the start time of process (happens once)
float Job::setStartTime(float time) {
	if (this->startTime < 0) {
		this->startTime = time;
		return time;
	}
	else {
		return -1;
	}
}


void Job::finish(float time) {
	if (this->expectedRunTime > 0)	std::cout<<"something's fishy"<<std::endl;
	this->completionTime = time;
	this->turnaroundTime = this->completionTime-this->arrivalTime;
	this->responseTime = this->startTime-this->arrivalTime;
}


int SRT(std::queue<Process> &processes) {
	int numproc = processes.size();
	int quanta = 0;
	std::vector<Job> arrivedJobs;	// ready queue
	std::vector<Job> completedJobs;

	while(!processes.empty() || !arrivedJobs.empty()) {
		// each new quantum, move any new jobs that arrived into the ready queue
		if (processes.front().arrivalTime <= quanta) {
			while(processes.front().arrivalTime <= quanta) {
				arrivedJobs.push_back(Job(processes.front()));
				processes.pop();
			}
			// sort the arrived jobs by process runtime (descending)
			// problem: if a new job arrives with the same remaining time as the current job, it might preempt the already running one
			std::sort(arrivedJobs.begin(), arrivedJobs.end(), &remainingTimeSort);
		}

		arrivedJobs.back().setStartTime(quanta);
		arrivedJobs.back().expectedRunTime--;
		for (auto &it : arrivedJobs) {	// jobs that are ready but not running are waiting
			it.waitTime++;
		}
		arrivedJobs.back().waitTime--;	// okay i know this is silly—sue me

		if (arrivedJobs.back().expectedRunTime <= 0) {
			completedJobs.push_back(arrivedJobs.back());
			arrivedJobs.pop_back();
			std::cout << "finished job " << completedJobs.back().id << " at time slice " << quanta << std::endl;
			// now arrivalTime shows Turnaround Time for process
			completedJobs.back().completionTime = quanta;
		}
		quanta++;
	}

}

// Logic for sorting processes.  If true, proc1 goes after proc2	*double check this
// sorted in descending order to make popping easier
bool remainingTimeSort(const Job& job1, const Job& job2){
    return job1.expectedRunTime > job2.expectedRunTime;
}