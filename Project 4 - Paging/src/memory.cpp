#include "memory.h"


Memory::Memory(uint32_t seed, int numJobs) {

    //Initialize Vectors
    jobQueue = generateJobs(seed, numJobs); //Vector of available jobs
    running = {};                           //Vector of running jobs
    finished = {};                          //Vector of finished jobs
    freeList = {};                          //Vector to track free Pages
    pageTable;                              //Page Table to track all PTE's

    //initializing Frame Table with 100 PageFrames (all initially free)
    frameTable.resize(100);
    for (int i = 0; i < 100; i++) {
        frameTable[i] = PageFrame();  // processId=-1, pageNumber=-1 (free)
    }

    //Populate Free List with 100 Pages
    for (int i = 0; i < 100; i++)
        freeList.push_back(i);
}
/*
Need to Implement tracking

totalReferences = 0;
pageHits = 0;
pageMisses = 0;
currentTime = 0;

*/
int Memory::numFree() {
    return freeList.size();
}

void Memory::print() {
    std::cout<<"====PRINTING MEM====\n";
    std::cout<<"Free frames: ";
    for (auto &it : freeList) {
        std::cout<<it<<", ";
    }
    std::cout<<std::endl;
}

int Memory::findLRUVictim() {
    // LRU: Find the frame with the oldest (minimum) lastAccessTime
    int lruFrame = -1;
    int minAccessTime = -1;
    bool foundValidAccessTime = false;
    
    // Scan through all frames in frameTable
    for (size_t frameNum = 0; frameNum < frameTable.size(); frameNum++) {
        const PageFrame& frame = frameTable[frameNum];
        
        // Skip free frames (processId == -1 means frame is free)
        if (frame.processId == -1) {
            continue;
        }
        
        // Get the corresponding PTE using processId and pageNumber from frame
        PageTableEntry* pte = pageTable.getEntry(frame.processId, frame.pageNumber);
        
        if (pte == nullptr || !pte->valid) {
            // This shouldn't happen if frameTable is consistent, but handling it either way
            continue;
        }
        
        // For LRU, we want the frame with minimum lastAccessTime
        // If lastAccessTime is valid (not -1), use it for comparison
        if (pte->lastAccessTime != -1) {
            if (!foundValidAccessTime || pte->lastAccessTime < minAccessTime) {
                lruFrame = frameNum;
                minAccessTime = pte->lastAccessTime;
                foundValidAccessTime = true;
            }
        } else if (!foundValidAccessTime) {
            // If we haven't found any frame with valid access time yet,
            // use this frame as candidate (though lastAccessTime is -1)
            // This handles edge cases where pages haven't been accessed yet
            if (lruFrame == -1) {
                lruFrame = frameNum;
            }
        }
    }
    
    // If no valid frame found (all frames are free), return -1
    // Otherwise return the LRU frame (or first occupied frame if no valid access times found)
    if (lruFrame == -1) {
        // Fallback: find first occupied frame (shouldn't happen if memory is full)
        for (size_t frameNum = 0; frameNum < frameTable.size(); frameNum++) {
            if (frameTable[frameNum].processId != -1) {
                lruFrame = frameNum;
                break;
            }
        }
    }
    
    return lruFrame;
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