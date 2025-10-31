#pragma once

#include <random>
#include <list>
#include <iostream>
#include <map>
#include <queue>
#include <deque>
#include <array>
#include "pageTable.h"
#include "jobs.h"




struct Memory {
    std::deque<Job> jobQueue;  // jobs that have not yet had any memory allocated (sorted by arrival time)
    std::deque<Job> running;   // all currently running jobs (with allocated pages); pagetable stuff here
    std::deque<Job> finished;
    std::array<int, 100> freeList;


    Memory(uint32_t seed, int numJobs = 150);
    //Core memory management functions
    int numFree();     // returns number of free pages
    // void print(){FreeListHead->print();}
    int run();
    void print();
    int assignPage(Job &job, int (*replacementAlgo)(Job) = nullptr);
    int reservePage(Job &job);
};
