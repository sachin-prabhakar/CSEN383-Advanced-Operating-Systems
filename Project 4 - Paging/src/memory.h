#pragma once
#include <random>
#include <iostream>
#include <map>
#include <queue>
#include <deque>
#include <array>
#include "pageTable.h"
#include "jobs.h"

struct Memory {
    PageTable pageTable;
    std::deque<Job> jobQueue;  // jobs that have not yet had any memory allocated (sorted by arrival time)
    std::deque<Job> running;   // all currently running jobs (with allocated pages); pagetable stuff here
    std::deque<Job> finished;  // all jobs that have been finished 
    std::array<int, 100> memory;    // array of every page in mem; -1 means free, otherwise shows pid


    Memory(uint32_t seed, int numJobs = 150);

    //Core memory management functions
    int numFree();     // returns number of free pages
    void print();
    int run(std::function<int()> replacementAlgo, uint32_t seed = 42);
    int assignPage(int t, Job &job, std::function<int()> replacementAlgo = nullptr, uint32_t seed = 42);
    void finishJob(Job &job, int t);  // helper for run()
    void startJob(int t);   // helper for run()

    // Page replacement algorithms
    int findLRUVictim();  // Returns frame number of LRU victim page
    int findFIFOVictim(); // Returns frame number of FIFO victim page
};
