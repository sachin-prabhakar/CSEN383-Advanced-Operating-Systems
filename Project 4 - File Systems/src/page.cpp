#include "page.h"

int Process::numProcs = 0;  // initialize numProcs

Process::Process(int size, int duration) {
    this->id = ++numProcs;
    this->size = size;
    this->duration = duration;
    this->remaining = -1;
}

Process::Process(std::mt19937& gen) {
    std::uniform_int_distribution<int> sizeDist(1, 4);
    std::uniform_int_distribution<int> durDist(1, 6);
    int sizes[] = {5, 11, 17, 31};

    this->id = ++numProcs;
    this->size = sizes[sizeDist(gen)];
    this->duration = durDist(gen);
    this->remaining = -1;  
}

std::list<Process> generateJobs(int numJobs, u_int32_t seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> sizeDist(1, 4);
    std::uniform_int_distribution<int> durDist(1, 6);
    int sizes[] = {5, 11, 17, 31};

    std::list<Process> jobs;

    for (int i = 0; i < numJobs; i++) {
        jobs.push_back(Process(sizes[sizeDist(gen)], durDist(gen)));
    }
    jobs.sort();

    return jobs;
}

bool operator<(const Process& lhs, const Process& rhs) {
        return lhs.duration < rhs.duration;
    }
