#include "page.h"

int Memory::numFree() {     // returns number of free pages
        int freePages = 0;
        for (int i = 0; i < totalPages; i++){
            if (pageFrames[i] < 1) freePages++;
        }
        return freePages;
    }

Memory::Memory(std::list<Process>& jq, int frames) {
    this->totalPages = frames;
    this->pageFrames[frames] = {0};
    this->jobQueue = jq;
    this->running = {};
}

Memory::Memory(uint32_t seed, int frames, int numJobs) {
    this->totalPages = frames;
    this->pageFrames[frames] = {0};
    this->jobQueue = generateJobs(seed, numJobs);
    this->running = {};
}