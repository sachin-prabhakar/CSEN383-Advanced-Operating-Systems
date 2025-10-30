#include "page.h"

int Process::numProcs = 0;  // initialize numProcs

Process::Process(int size, int duration, int arrival) {
    this->id = ++numProcs;
    this->size = size;
    this->duration = duration;
    this->arrival = arrival;
    this->remaining = -1;
}

Process::Process(u_int32_t seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> sizeDist(0, 3);
    std::uniform_int_distribution<int> durDist(1, 5);
    std::uniform_int_distribution<int> arrivalDist(0, 599);
    int sizes[] = {5, 11, 17, 31};

    this->id = ++numProcs;
    this->arrival = arrivalDist(gen);
    this->size = sizes[sizeDist(gen)];
    this->duration = durDist(gen);
    this->remaining = -1;
}

std::list<Process> generateJobs(u_int32_t seed, int numJobs) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> sizeDist(0, 3);
    std::uniform_int_distribution<int> durDist(1, 5);
    std::uniform_int_distribution<int> arrivalDist(0, 599);
    int sizes[] = {5, 11, 17, 31};

    std::list<Process> jobs;

    for (int i = 0; i < numJobs; i++) {
        jobs.push_back(Process(sizes[sizeDist(gen)], durDist(gen), arrivalDist(gen)));
    }
    jobs.sort();

    return jobs;
}

bool operator<(const Process& lhs, const Process& rhs) {
        return lhs.arrival < rhs.arrival;
    }

std::ostream& operator<<(std::ostream& os, const Process& job) {
    os<<"\nPROCESS "<<job.id<<"\n\tARRIVAL\t="<<job.arrival<<"\n\tSIZE=\t"<<job.size;
    os<<"\n\tDUR=\t"<<job.duration<<"\n\tREM=\t"<<job.remaining<<std::endl;
    return os;
}
