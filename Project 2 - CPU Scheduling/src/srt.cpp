// ethan shenassa
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "SRT.h"

void printq(std::vector<Process> q) {
    if (q.empty()) return ;
    std::cout<<"PROCESSES: ";
    for (auto &it : q) {
        std::cout << it.id << " ";
    }
    std::cout<<std::endl;
}

void printarrivals(std::vector<Process> q) {
    while (!q.empty()) {
        std::cout<<q.front().id<<":"<<q.front().arrivalTime<<":"<<q.front().expectedRunTime<<", ";
        q.erase(q.begin());
    }
    std::cout<<std::endl;
}
void printarrivals(std::queue<Process> q) {
    while (!q.empty()) {
        std::cout<<q.front().id<<":"<<q.front().arrivalTime<<":"<<q.front().expectedRunTime<<", ";
        q.pop();
    }
    std::cout<<std::endl;
}

int SRT(std::queue<Process> &processes) {
	int quanta = 0;
	std::vector<Process> ready; // ready queue
	std::vector<Process> finished;
	while(!processes.empty() || !ready.empty()) {
		if (quanta > 400) {
            std::cout<<"Something's wrong"<<std::endl;
            return 1;
        }

        std::cout<<"quantum="<<quanta<<std::endl;
        std::cout<<"\tprocesses: ";
        printarrivals(processes);
        std::cout<<"\tready: ";
        printarrivals(ready);
        std::cout<<"\tfinished: ";
        printarrivals(finished);

        // each new quantum, move any new jobs that arrived into the ready queue
        while(!processes.empty() && processes.front().arrivalTime <= quanta) {
            ready.push_back(processes.front());
            processes.pop();
            // more efficient would be to just insert in sorted position
        }
        // sort the arrived jobs by process runtime (descending)
        // problem: if a new job arrives with the same remaining time as the current job, it might preempt the already running one
        if (!ready.empty()) {
            std::cout<<quanta<<std::endl;
            std::sort(ready.begin(), ready.end(), &remainingTimeSort);
            std::cout<<quanta<<std::endl;
            ready.front().setstartTime(quanta);
            ready.front().expectedRunTime--;
            std::cout<<"quanta"<<std::endl;
            for (auto &it : ready) {	// jobs that are ready but not running are waiting
                it.waitTime++;
            }
            std::cout<<quanta<<std::endl;
            ready.front().waitTime--;	// okay i know this is silly—sue me

            if (ready.front().expectedRunTime <= 0) {
                finished.push_back(ready.front());
                ready.erase(ready.begin());
                finished.back().completionTime = quanta;
            }
        }
		quanta++;
    }
    std::cout<<"HI"<<std::endl;
    printResults(finished);
    return 0;
}

// Logic for sorting processes.  If true, proc1 goes after proc2	*double check this
// sorted in descending order to make popping easier
bool remainingTimeSort(const Process& job1, const Process& job2){
    return job1.expectedRunTime < job2.expectedRunTime;
}