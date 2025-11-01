#pragma once
#include <random>
#include <iostream>
#include <algorithm> //std::sort
#include "pageTable.h"

struct Job {
    int id;
    int procSize;       // process size in MB
    int serviceTime;   // service time for process 
    int arrivalTime;    // arrival time in ms
    int remainingTime;  // time remaining for process (starts as -1 and changes to serviceTime when proc starts)
    static int numProcs; // id for each process
    int currentPage;
    int finishTime;
    std::deque<int> pageTable;  // index is VPN, value is PPN

    Job() : id(0), procSize(0), serviceTime(0), arrivalTime(0), remainingTime(0) {}
    Job(int size, int service, int arrival) : id(++numProcs), procSize(size), serviceTime(service), arrivalTime(arrival), remainingTime(-1) {}
    Job(uint32_t seed);
    friend std::ostream& operator<<(std::ostream& os, const Job& job);
    // int run(uint32_t seed = 42);
};

std::deque<Job> generateJobs(uint32_t seed, int numJobs = 150);
bool jobcmp(const Job &j1, const Job &j2);