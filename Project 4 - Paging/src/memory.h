#pragma once
#include <random>
#include <iostream>
#include <vector>
#include <map>
#include "pageTable.h"
#include "jobs.h"

struct Memory {
    PageTable pageTable;

    std::vector<Job> jobQueue;  // jobs that have not yet had any memory allocated   
    std::vector<Job> running;   // all currently running jobs (with allocated pages); pagetable stuff here
    std::vector<Job> finished;
    std::vector<int> freeList;
    std::vector<PageFrame> frameTable;  //Track physical memory

    Memory(uint32_t seed, int numJobs = 150);

    //Core memory management functions
    int numFree();     // returns number of free pages
    void print();
    int run();
    
    // Page replacement algorithms
    int findLRUVictim();  // Returns frame number of LRU victim page
    int findFIFOVictim(); // Returns frame number of FIFO victim page
};
