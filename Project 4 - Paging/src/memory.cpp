#include "memory.h"


Memory::Memory(uint32_t seed, int numJobs) {

    jobQueue = generateJobs(seed, numJobs); // jobQueue is sorted by arrival time
    running= {};
    finished = {};
    for (auto &it : freeList)   it = -1;
    //List or array, etc... For page Table

}

int Memory::numFree() {
    int count = 0;
    for (auto &it : freeList) count+= it==-1 ? 1 : 0;
    return count;
}


void Memory::print() {
    std::cout<<"========PRINTING MEM========\n";
    for (auto &it : freeList)   std::cout<<it<<", ";
    std::cout<<std::endl;

    std::cout<<"========PRINTING JOB QUEUE========\n";
    for (auto &it : jobQueue)   std::cout<<it;
    std::cout<<std::endl;

    std::cout<<"========PRINTING RUNNING========\n";
    for (auto &it : running)   std::cout<<it;
    std::cout<<std::endl;

    std::cout<<"========PRINTING FINISHED========\n";
    for (auto &it : finished)   std::cout<<it;
    std::cout<<std::endl;
}



int Memory::reservePage(Job &job) {
    for (size_t i = 0; i < freeList.size(); i++) 
        if (freeList[i] == -1) {
            freeList[i] = job.id;
            // here add page i to job's pagetable i think
            return i;
        }
    return -10; // failed to find free page
}

int Memory::assignPage(Job &job, int (*replacementAlgo)(Job)) {
    if (job.pageTable.size() == 0) {

    }
    
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

int Memory::findFIFOVictim(){
    // FIFO: Find the oldest frame (first one to be added)
    int FIFOFrame = -1;
    int minloadTime = -1;
    bool foundValidloadTime = false;
    
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
        
        // For FIFO, we want the frame with smallest (oldest) loadTime
        // If loadTime is valid (not -1), use it for comparison
        if (pte->loadTime != -1) {
            if (!foundValidloadTime || pte->loadTime < minloadTime) {
                FIFOFrame = frameNum;
                minloadTime = pte->loadTime;
                foundValidloadTime = true;
            }
        } else if (!foundValidloadTime) {
            // If we haven't found any frame with valid load time yet,
            // use this frame as candidate (though loadTime is -1)
            // This handles edge cases where pages haven't been accessed yet
            if (FIFOFrame == -1) {
                FIFOFrame = frameNum;
            }
        }
    }
    
    // If no valid frame found (all frames are free), return -1
    // Otherwise return the FIFO frame (or first occupied frame if no valid load times found)
    if (FIFOFrame == -1) {
        // Fallback: find first occupied frame (shouldn't happen if memory is full)
        for (size_t frameNum = 0; frameNum < frameTable.size(); frameNum++) {
            if (frameTable[frameNum].processId != -1) {
                FIFOFrame = frameNum;
                break;
            }
        }
    }
    return FIFOFrame;
}

int Memory::run() {
    int t = 0;  // time slize (every 100ms)
    while ((!jobQueue.empty() || !running.empty()) && t < 600) {
        while (numFree() > 4 && running.size() < 26 && jobQueue.front().arrivalTime < t) {  
            // fill up running vector
            running.push_back(jobQueue.front());
            jobQueue.pop_front();
            running.front().remainingTime = running.front().serviceTime;
            assignPage(running.front());
        }
        std::sort(running.begin(), running.end(), &jobcmp); // maybe this sorting algo isnt great

        for (auto it = running.begin(); it != running.end();) { // go through all running processes
            // reserve page here
            
            if (it->remainingTime == 0) {   // remove all jobs that are done
                finished.push_back(*it);
                finished.back().finishTime = t;
                it = running.erase(it);
            }
            else {
                it->remainingTime--;
                ++it;
            }
        }
        

        t++;
    }
    // while (!running.empty()) {



    //     t++;
    // }

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