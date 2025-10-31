#include "memory.h"
#include "pageTable.h"
#include "jobs.h"




Memory::Memory(uint32_t seed, int numJobs) {

    //Linked List of available jobs
    jobQueue = generateJobs(seed, numJobs);
    freeList = {};
    for (int i = 0; i < 100; i++)
        freeList.push_back(i);

    //List or array, etc... For page Table


    // this->totalPages = frames;
    // this->pageFrames.resize(frames);
    // this->pageFrames.resize(frames);
    // this->jobQueue = generateJobs(seed, numJobs);
    // this->running = {};
    // this->algorithm = PageReplacementAlgorithm::FIFO;
    // this->totalReferences = 0;
    // this->pageHits = 0;
    // this->pageMisses = 0;
    // this->currentTime = 0;

    // // Initialize all frames as free
    // for (int i = 0; i < frames; i++) {
    //     pageFrames[i].processId = -1;
    //     pageFrames[i].pageNumber = -1;
    //     pageFrames[i].lastAccessTime = -1;
    //     pageFrames[i].accessCount = 0;
    //     pageFrames[i].loadTime = -1;
    // }
}

void Memory::print() {
    std::cout<<"====PRINTING MEM====\n";
    for (auto &it : freeList) {
        std::cout<<it<<", ";
    }
    std::cout<<std::endl;
}

int Memory::run() {
    int t = 0;  // time slize (every 100ms)
    while (jobQueue.empty() && !running.empty() && t < 600) {
        // while (numFree() >= 4 && jobQueue.front().arrivalTime <= t) {
        //     // fill up running
        //     running.push_back(jobQueue.front());
        //     jobQueue.pop_front();
        //     jobQueue.front().currentPage = 0;
        // }
        t++;
    }
    while (!running.empty()) {

    }

    return 0;
}




// int Memory::run() {
//     for (auto &it : jobQueue) {}
//     int t = 0;  // time slice (every 100 ms)
//     while (!jobQueue.empty() || !running.empty()) {
//         while (numFree() >= 4 && jobQueue.front().arrival <= t) {
//             // fill up running
//             running.push_back(jobQueue.front());
//             jobQueue.pop_front();
//             jobQueue.front().currentPage = 0;

//             // take some free memory for vpn 0
//         }
//         while (numFree() < 4) {
//             //  one iteration
//                 // decrement all running processes remaining time by 1
//                 // every job op generates memory reference (70% chance reference is on adjacent virtual page)
//                 // should every process have its own virtual page table? I think yes
//                 for (auto &it : running) {
//                     int page = it.run();
//                     if (it.remaining <= 0) {
//                         // problem: how to remove jobs when they finish? might have to change to vector or dequeue
//                     }
//                 }
//             t++;
//         }
//     }
//     // should return a list of all records generated and then we can print all the records
// }

// int Job::run(uint32_t seed) {
//     remainingTime--;
//     std::mt19937 gen(seed);
//     std::uniform_int_distribution<int> rdist(0, 9);
//     int r = rdist(gen);
//     int pageref;
//     if (r < 7) {
//         std::uniform_int_distribution<int> ddist(-1, 1);
//         pageref = (currentPage + ddist(gen)) % procSize;
//     }
//     else {
//         std::uniform_int_distribution<int> ddist(2, 9);
//         pageref = (currentPage + ddist(gen)) % procSize;
//     }
//     // TODO: generate record here
//     currentPage = pageref;
//     return pageref;
// }