#include <random>
#include <list>
#include <iostream>
# pragma once

struct Process {
    int id;
    int size;       // process size in MB (set at construction)
    int serviceTime;   // initial process duration (set at construction)
    int remaining;  // remaining time in process (starts at -1 until process begins)
    int arrival;    // jobs arrive between 0 and 599 (60 seconds measured every 100ms)
    static int numProcs;

    Process(int size, int duration, int arrival);
    Process(uint32_t seed);
    friend bool operator<(const Process& lhs, const Process& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Process& job);
};

struct Memory {
    int totalPages;
    std::list<Process> jobQueue;    // jobs that do not have memory allocated
    std::list<Process> running;     // all currently running jobs (with allocated pages)
    int pageFrames[];        // all page frames available in hardware and the Process ID using it

    Memory(uint32_t seed, int frames = 100, int numJobs = 150);
    Memory(std::list<Process>& jq, int frames = 100);
    int numFree();     // returns number of free pages
};

std::list<Process> generateJobs(uint32_t seed, int numJobs = 150);
