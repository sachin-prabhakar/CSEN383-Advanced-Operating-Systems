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

std::ostream& operator<<(std::ostream& os, const Job& job) {
    os<<"\nPROCESS "<<job.id<<"\n\tARRIVAL\t="<<job.arrivalTime<<"\n\tSIZE=\t"<<job.procSize;
    os<<"\n\tDUR=\t"<<job.serviceTime<<"\n\tREM=\t"<<job.remainingTime<<std::endl;
    return os;
}

bool operator<(const Job& lhs, const Job& rhs) {
    return lhs.arrivalTime < rhs.arrivalTime;
}

std::vector<Job> generateJobs(uint32_t seed, int numJobs) {

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> sizeDist(0, 3);
    std::uniform_int_distribution<int> serviceDist(1, 5);
    std::uniform_int_distribution<int> arrivalDist(0, 599);
    int sizes[] = {5, 11, 17, 31};
    std::vector<Job> jobs = {};

    for (int i = 0; i < numJobs; i++) {
        int procSize = sizes[sizeDist(gen)];
        int arrivalTime = arrivalDist(gen);
        int serviceTime = serviceDist(gen);

        jobs.push_back(Job(procSize, serviceTime, arrivalTime));
    }
    std::sort(jobs.begin(), jobs.end());

    return jobs;
}