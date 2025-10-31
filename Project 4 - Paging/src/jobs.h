#pragma once
#include <random>
#include <iostream>


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
    // int run(uint32_t seed = 42);
};

//Struct to hold each job/process
struct JobNode {
    Job job;
    JobNode* next;
    JobNode(uint32_t seed) : job(Job(seed)), next(nullptr){}
    JobNode(Job temp) : job(temp), next(nullptr){}
};

class JobList {
    private:
        JobNode* head;
    public:
        JobList() : head(nullptr) {}

        void addJob_Start(JobNode* newJob);
        void addJob_End(JobNode* newJob);
        void addJob_Sorted(JobNode* newJob);    // always adds job in its sorted pos
        bool empty();
};

std::vector<Job> generateJobs(uint32_t seed, int numJobs = 150);