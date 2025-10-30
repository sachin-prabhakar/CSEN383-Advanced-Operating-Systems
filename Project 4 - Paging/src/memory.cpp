#include "memory.h"
#include "pageTable.h"
#include "jobs.h"

int Memory::numFree() {     // returns number of free pages
    int freePages = 0;
    for (int i = 0; i < totalPages; i++){
        if (pageFrames[i].processId == -1) freePages++;
    }
    return freePages;
}

Memory::Memory(std::list<Process>& jq, int frames) {
    this->totalPages = frames;
    this->pageFrames.resize(frames);
    this->pageFrames.resize(frames);
    this->jobQueue = jq;
    this->running = {};
    this->algorithm = PageReplacementAlgorithm::FIFO;
    this->totalReferences = 0;
    this->pageHits = 0;
    this->pageMisses = 0;
    this->currentTime = 0;

    // Initialize all frames as free
    for (int i = 0; i < frames; i++) {
        pageFrames[i].processId = -1;
        pageFrames[i].pageNumber = -1;
        pageFrames[i].lastAccessTime = -1;
        pageFrames[i].accessCount = 0;
        pageFrames[i].loadTime = -1;
    }
    this->algorithm = PageReplacementAlgorithm::FIFO;
    this->totalReferences = 0;
    this->pageHits = 0;
    this->pageMisses = 0;
    this->currentTime = 0;

    // Initialize all frames as free
    for (int i = 0; i < frames; i++) {
        pageFrames[i].processId = -1;
        pageFrames[i].pageNumber = -1;
        pageFrames[i].lastAccessTime = -1;
        pageFrames[i].accessCount = 0;
        pageFrames[i].loadTime = -1;
    }
}

Memory::Memory(uint32_t seed, int frames, int numJobs) {
    this->totalPages = frames;
    this->pageFrames.resize(frames);
    this->pageFrames.resize(frames);
    this->jobQueue = generateJobs(seed, numJobs);
    this->running = {};
    this->algorithm = PageReplacementAlgorithm::FIFO;
    this->totalReferences = 0;
    this->pageHits = 0;
    this->pageMisses = 0;
    this->currentTime = 0;

    // Initialize all frames as free
    for (int i = 0; i < frames; i++) {
        pageFrames[i].processId = -1;
        pageFrames[i].pageNumber = -1;
        pageFrames[i].lastAccessTime = -1;
        pageFrames[i].accessCount = 0;
        pageFrames[i].loadTime = -1;
    }
}

int Memory::run() {
    // for (auto &it : jobQueue) {}
    int t = 0;  // time slice (every 100 ms)
    while (!jobQueue.empty() || !running.empty()) {
        while (numFree() >= 4 && jobQueue.front().arrival <= t) {
            // fill up running
            running.push_back(jobQueue.front());
            jobQueue.pop_front();
            jobQueue.front().currentPage = 0;

            // take some free memory for vpn 0
        }
        while (numFree() < 4) {
            //  one iteration
                // decrement all running processes remaining time by 1
                // every job op generates memory reference (70% chance reference is on adjacent virtual page)
                // should every process have its own virtual page table? I think yes
                for (auto &it : running) {
                    int page = it.run();
                    if (it.remaining <= 0) {
                        // problem: how to remove jobs when they finish? might have to change to vector or dequeue
                    }
                }
            t++;
        }
    }
    // should return a list of all records generated and then we can print all the records
}