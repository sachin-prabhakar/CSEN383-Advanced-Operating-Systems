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
    int remainingTime;  // time remaining for process
    static int numProcs; // id for each process
    int currentPage;

    Job() : id(0), procSize(0), serviceTime(0), arrivalTime(0), remainingTime(0) {}
    Job(int size, int service, int arrival) : id(++numProcs), procSize(size), serviceTime(service), arrivalTime(arrival), remainingTime(-1) {}
    Job(uint32_t seed);
    friend std::ostream& operator<<(std::ostream& os, const Job& job);
    friend bool operator<(const Job& lhs, const Job& rhs);
};

std::vector<Job> generateJobs(uint32_t seed, int numJobs = 150);