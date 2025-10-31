#pragma once

#include <random>
#include <list>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <vector>
#include "pageTable.h"
#include "jobs.h"




struct Memory {
    JobList *JobsHead;    // jobs that do not have memory allocated
    std::vector<Job> jobQueue;  // jobs that have not yet had any memory allocated   
    std::vector<Job> running;   // all currently running jobs (with allocated pages); pagetable stuff here
    std::vector<Job> finished;
    std::vector<int> freeList;


    Memory(uint32_t seed, int numJobs = 150);
    //Core memory management functions
    int numFree();     // returns number of free pages
    void print(){FreeListHead->print();}
    int run();
    void print();
};
