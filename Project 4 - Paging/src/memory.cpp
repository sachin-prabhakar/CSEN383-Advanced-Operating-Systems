#include "page.h"

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
}

Memory::Memory(u_int32_t seed, int frames, int numJobs) {
    this->totalPages = frames;
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