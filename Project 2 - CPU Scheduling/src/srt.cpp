// ethan shenassa
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "SRT.h"

void printq(std::vector<Process> q) {
    for (auto &it : q) {
        std::cout << it.id << " ";
    }
}

int SRT(std::queue<Process> &processes) {
	int quanta = 0;
	std::vector<Process> ready; // ready queue
	std::vector<Process> finished;
	while(!processes.empty() || !ready.empty()) {
		// each new quantum, move any new jobs that arrived into the ready queue
		if (!processes.empty() && processes.front().arrivalTime <= quanta) {
			while(processes.front().arrivalTime <= quanta) {
				ready.push_back(processes.front());
				processes.pop();
			}
			// sort the arrived jobs by process runtime (descending)
			// problem: if a new job arrives with the same remaining time as the current job, it might preempt the already running one
			
            std::sort(ready.begin(), ready.end(), &remainingTimeSort);
		}
        printq(ready);
        printq(finished);

		ready.back().setstartTime(quanta);
		ready.back().expectedRunTime--;
		for (auto &it : ready) {	// jobs that are ready but not running are waiting
			it.waitTime++;
		}
		ready.back().waitTime--;	// okay i know this is silly—sue me

		if (ready.back().expectedRunTime <= 0) {
			finished.push_back(ready.back());
			ready.pop_back();
			std::cout << "finished job " << finished.back().id << " at time slice " << quanta << std::endl;
			// now arrivalTime shows Turnaround Time for process
			finished.back().completionTime = quanta;
		}
		quanta++;
	}

    return 0;
}

// Logic for sorting processes.  If true, proc1 goes after proc2	*double check this
// sorted in descending order to make popping easier
bool remainingTimeSort(const Process& job1, const Process& job2){
    return job1.expectedRunTime > job2.expectedRunTime;
}