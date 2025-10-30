#include "jobs.h"

Job::Job(uint32_t seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> sizeDist(0, 3);
    std::uniform_int_distribution<int> serviceDist(1, 5);
    std::uniform_int_distribution<int> arrivalDist(0, 599);
    int sizes[] = {5, 11, 17, 31};

    id = ++numProcs;
    procSize = sizes[sizeDist(gen)];
    arrivalTime = arrivalDist(gen);
    serviceTime = serviceDist(gen);
    remainingTime = -1;  
}

JobNode* createJob(uint32_t seed){
    JobNode* newJob = new JobNode;
    newJob->job = Job::Job(seed);
    newJob->next = nullptr;

    return newJob;
}


void JobList::addJob_Start(JobNode* newJob){
    newJob->next = head;
    head = newJob;
}

void JobList::addJob_End(JobNode* newJob){
    if(head == nullptr){
        head = newJob;
        return;
    }

    JobNode* curr = head;
    while(curr != nullptr){
        curr = curr->next;
    }

    curr->next = newJob;
}

void JobList::addJob_Sorted(JobNode* newJob){
    if(head == nullptr || newJob->job.arrivalTime < head->job.arrivalTime){
        newJob->next = head;
        head = newJob;
        return;
    }

    JobNode* curr = head;
    while(curr != nullptr && curr->next->job.arrivalTime < newJob->job.arrivalTime){
        curr = curr->next;
    }
    newJob->next = curr->next;
    curr->next = newJob;
}

JobNode* generateJobs(uint32_t seed, int numJobs) {

    std::forward_list<Job> jobList;

    for (int i = 0; i < numJobs; i++) {
        //jobList.push_back(Job(seed));;
    }

    jobList.sort();

    return; //jobList;
}

//Vector Sorting Function
bool arrivaltimeSort(const Job& job1, const Job& job2){
    return job1.arrivalTime < job2.arrivalTime;
}

std::ostream& operator<<(std::ostream& os, const Job& job) {
    os<<"\nPROCESS "<<job.id<<"\n\tARRIVAL\t="<<job.arrivalTime<<"\n\tSIZE=\t"<<job.procSize;
    os<<"\n\tDUR=\t"<<job.serviceTime<<"\n\tREM=\t"<<job.remainingTime<<std::endl;
    return os;
}
