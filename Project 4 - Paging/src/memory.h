#pragma once

#include <random>
#include <list>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "pageTable.h"

//
struct PageNode {
    PageFrame page;
    PageNode* next;
    PageNode* createPage(uint32_t seed);
};

class MemList {
    private:
        PageNode* head;
    public:
        MemList() : head(nullptr) {}

        void addJob_Start(PageNode* newJob);
        void addJob_End(PageNode* newJob);
        void addJob_Sorted(PageNode* newJob);     
};


struct Memory {
    int totalPages;
    std::list<Process> jobQueue;    // jobs that do not have memory allocated
    std::list<Process> running;     // all currently running jobs (with allocated pages)
    std::vector<PageFrame> pageFrames;  // all page frames available in hardware and the Process ID using it
    std::map<int, ProcessPageInfo> processPages; // Track pages for each running process


    // Page replacement algorithm state
    PageReplacementAlgorithm algorithm;
    std::queue<int> fifoQueue;          // For FIFO algorithm
    std::map<int, int> lruMap;          // For LRU algorithm (frame -> last access time)
    std::map<int, int> lfuMap;          // For LFU algorithm (frame -> access count)
    std::map<int, int> mfuMap;          // For MFU algorithm (frame -> access count)

    // Statistics
    int totalReferences;
    int pageHits;
    int pageMisses;
    int currentTime; // Current simulation time in 100ms units

    Memory(uint32_t seed, int frames = 100, int numJobs = 150);
    Memory(std::list<Process>& jq, int frames = 100);

    //Core memory management functions
    int numFree();     // returns number of free pages

    int run();
};
