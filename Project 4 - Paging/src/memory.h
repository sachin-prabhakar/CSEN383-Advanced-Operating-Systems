#pragma once

#include <random>
#include <list>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "pageTable.h"
#include "jobs.h"


struct MemNode {
    int start_loc;
    int end_loc;
    static int nodeid;
    //Check to see if each node in memory has proper start and end address
    MemNode() : start_loc(nodeid++), end_loc(nodeid), next(nullptr) {}
    MemNode* next;
    friend std::ostream& operator<<(std::ostream& os, const MemNode& node);
};

class MemList {
    private:
        MemNode* head;
        int totalFrames = 100;
        int freeFrames = 100;
    public:
        MemList() : head(nullptr) {}
        void addMem(MemNode* newNode);
        int getTotalFrames() {return totalFrames;}
        int getFreeFrames() {return freeFrames;}
        void increaseFreeFrames(int f){ if(freeFrames + f < 100){freeFrames +=f;}}
        void decreaseFreeFrames(int f){ if(freeFrames - f > 0){freeFrames -=f;}}
        void print();
};

MemList* generateFreeList();



struct Memory {
    JobList *JobsHead;    // jobs that do not have memory allocated
    std::vector<Job> running;     // all currently running jobs (with allocated pages); pagetable stuff here
    MemList *FreeListHead;
    // std::vector<PageFrame> pageFrames;  // all page frames available in hardware and the Process ID using it
    // std::map<int, ProcessPageInfo> processPages; // Track pages for each running process


    // Page replacement algorithm state
    // PageReplacementAlgorithm algorithm;
    // std::queue<int> fifoQueue;          // For FIFO algorithm
    // std::map<int, int> lruMap;          // For LRU algorithm (frame -> last access time)
    // std::map<int, int> lfuMap;          // For LFU algorithm (frame -> access count)
    // std::map<int, int> mfuMap;          // For MFU algorithm (frame -> access count)

    // Statistics
    // int totalReferences;
    // int pageHits;
    // int pageMisses;
    // int currentTime; // Current simulation time in 100ms units

    Memory(uint32_t seed, int numJobs = 150);
    //Core memory management functions
    int numFree();     // returns number of free pages

    int run();
};
