#include "jobs.h"

int Job::numProcs = 0;  // initialize numProcs

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
    newJob->job = Job(seed);
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

JobList* generateJobs(uint32_t seed, int numJobs) {

    JobList* tempList = new JobList;
    JobNode* tempNode = new JobNode;

    for (int i = 0; i < numJobs; i++) {
        std::random_device rd;
        uint32_t seed1 = rd();
        tempList->addJob_Sorted(tempNode->createJob(seed1));
    }

    return tempList;
}

std::ostream& operator<<(std::ostream& os, const Job& job) {
    os<<"\nPROCESS "<<job.id<<"\n\tARRIVAL\t="<<job.arrivalTime<<"\n\tSIZE=\t"<<job.procSize;
    os<<"\n\tDUR=\t"<<job.serviceTime<<"\n\tREM=\t"<<job.remainingTime<<std::endl;
    return os;
}


bool operator<(const Job& lhs, const Job& rhs) {
        return lhs.arrivalTime < rhs.arrivalTime;
    }
    
//std::list<Process> generateJobs(uint32_t seed, int numJobs) {
//     std::mt19937 gen(seed);
//     std::uniform_int_distribution<int> sizeDist(0, 3);
//     std::uniform_int_distribution<int> durDist(1, 5);
//     std::uniform_int_distribution<int> arrivalDist(0, 599);
//     int sizes[] = {5, 11, 17, 31};

//     std::list<Process> jobs;

//     for (int i = 0; i < numJobs; i++) {
//         jobs.push_back(Process(sizes[sizeDist(gen)], durDist(gen), arrivalDist(gen)));
//     }
//     jobs.sort();

//     return jobs;
// }