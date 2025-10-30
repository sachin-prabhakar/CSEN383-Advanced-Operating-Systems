# pragma once
#include <random>
#include <forward_list>
#include <iostream>

int Job::numProcs = 0;  // initialize numProcs

struct Job {
    int id;
    int procSize;       // process size in MB
    int serviceTime;   // service time for process
    int arrivalTime;    // arrival time in ms

    int remainingTime;  // time remaining for process
    static int numProcs; // id for each process

    Job() : id(0), procSize(0), serviceTime(0), arrivalTime(0), remainingTime(0) {}
    Job(int size, int service, int arrival) : id(++numProcs), procSize(size), serviceTime(service), arrivalTime(arrival), remainingTime(-1) {}
    Job(uint32_t seed);
    friend std::ostream& operator<<(std::ostream& os, const Job& job);
};

struct JobNode {
    Job job;
    JobNode * next;
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



JobNode* generateJobs(uint32_t seed, int numJobs = 150);