#include "pages.h"

Process::Process(uint32_t seed) {
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

std::vector<Process> generateJobs(uint32_t seed, int numJobs) {

    std::vector<Process> jobs;

    for (int i = 0; i < numJobs; i++) {
        jobs.push_back(Process(seed));
    }

    std::sort(jobs.begin(), jobs.end(), &arrivaltimeSort);

    return jobs;
}

//Vector Sorting Function
bool arrivaltimeSort(const Process& proc1, const Process& proc2){
    return proc1.arrivalTime < proc2.arrivalTime;
}

std::ostream& operator<<(std::ostream& os, const Process& job) {
    os<<"\nPROCESS "<<job.id<<"\n\tARRIVAL\t="<<job.arrivalTime<<"\n\tSIZE=\t"<<job.procSize;
    os<<"\n\tDUR=\t"<<job.serviceTime<<"\n\tREM=\t"<<job.remainingTime<<std::endl;
    return os;
}
