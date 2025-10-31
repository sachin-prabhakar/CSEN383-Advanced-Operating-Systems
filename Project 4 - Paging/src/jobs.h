#pragma once
#include <random>
#include <iostream>

int Job::numProcs = 0;  // initialize numProcs

struct Job {
    int id;
    int procSize;       // process size in MB
    int serviceTime;   // service time for process
    int arrivalTime;    // arrival time in ms
    int remainingTime;  // time remaining for process
    static int numProcs; // id for each process
    int currentPage;

    Job() : id(0), procSize(0), serviceTime(0), arrivalTime(0), remainingTime(0) {}
    Job(int size, int service, int arrival) : id(++numProcs), procSize(size), serviceTime(service), arrivalTime(arrival), remainingTime(-1) {}
    Job(uint32_t seed);
    friend std::ostream& operator<<(std::ostream& os, const Job& job);
    int run(uint32_t seed = 42);
};

//Struct to hold each job/process
struct JobNode {
    Job job;
    JobNode* next;
    JobNode* createJob(uint32_t seed);
};

class JobList {
    private:
        JobNode* head;
    public:
        JobList() : head(nullptr) {}

        void addJob_Start(JobNode* newJob);
        void addJob_End(JobNode* newJob);
        void addJob_Sorted(JobNode* newJob);     
};

JobList* generateJobs(uint32_t seed, int numJobs = 150);

int Job::run(uint32_t seed) {
    remainingTime--;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> rdist(0, 9);
    int r = rdist(gen);
    int pageref;
    if (r < 7) {
        std::uniform_int_distribution<int> ddist(-1, 1);
        pageref = (currentPage + ddist(gen)) % procSize;
    }
    else {
        std::uniform_int_distribution<int> ddist(2, 9);
        pageref = (currentPage + ddist(gen)) % procSize;
    }
    // TODO: generate record here
    currentPage = pageref;
    return pageref;
}