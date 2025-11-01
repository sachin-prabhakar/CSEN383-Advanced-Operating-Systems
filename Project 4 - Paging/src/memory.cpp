#include "memory.h"


Memory::Memory(uint32_t seed, int numJobs) {
    jobQueue = generateJobs(seed, numJobs); // jobQueue is sorted by arrival time
    running= {};
    finished = {};
    for (auto &it : memory)   it = -1;
    pageTable = PageTable();
}

int Memory::numFree() {
    int count = 0;
    for (auto &it : memory) count+= it==-1 ? 1 : 0;
    return count;
}


void Memory::print() {
    std::cout<<"========PRINTING MEM========\n";
    for (auto &it : memory)   std::cout<<it<<", ";
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


int getNewVpn(const Job &job, uint32_t seed = 42) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> rdist(0, 10);
    std::uniform_int_distribution<int> ldist(-1, 1);
    std::uniform_int_distribution<int> jdist(2, job.procSize-1);
    int r = rdist(gen);
    int vpn;    // virtual page number
    if (r < 7) vpn = ldist(gen)%job.procSize;
    else vpn = jdist(gen)%job.procSize;
    if (vpn < 0) vpn = job.procSize+vpn;
    return vpn;
}

int Memory::assignPage(int t, Job &job, std::function<int()> replacementAlgo, uint32_t seed) { // returns ppn if successful, else -1
    int frameno = -1;
    if (numFree() == 0) {
        frameno = replacementAlgo();
        pageTable.invalidateEntry(memory.at(frameno), getNewVpn(job, seed));
    }
    else if (numFree() > 0) {   // if there is at least one free frame...
        for (size_t i = 0; i < memory.size(); i++) {
            if (memory.at(i) == -1) {
                frameno = i;
                break;
            }
        }
    }
    memory[frameno] = job.id;
    if (job.currentPage == -1) {    // job is being run for the first time
        job.currentPage = 0;
        pageTable.updateEntry(job.id, 0, frameno, t); // should this be updateAccess()?
    } 
    else {  // job has run before
        int newvpn = getNewVpn(job, seed);
        pageTable.updateAccess(job.id, newvpn, t, frameno);
        job.currentPage = newvpn;
    }
    std::cout<<"Assigning job "<<job.id<<" vpn "<<job.currentPage<<" to frame "<<frameno<<std::endl;
    return frameno;  
}

void Memory::finishJob(Job &job, int t) {
    finished.push_back(job);
    finished.back().finishTime = t;
    finished.back().finRecord();
    for (auto &it1 : memory) 
        if (it1 == job.id)   it1 = -1;
    
}

void Memory::startJob(int t) {
    running.push_back(jobQueue.front());
    jobQueue.pop_front();
    running.back().remainingTime = running.back().serviceTime;
    pageTable.initializeProcess(running.back().id, running.back().procSize);
    running.back().startRecord();
} 

void printq(std::deque<Job> q) {
    std::cout<<"printing deque: ";
    for (auto &it : q) {
        std::cout<<it.id<<" ";
    }
    std::cout<<std::endl;
}

int Memory::run(uint32_t seed) {
    int t = 0;  // time slize (every 100ms)
    while ((!jobQueue.empty() || !running.empty()) && t < 600) {
        std::cout<<"t = "<<t<<std::endl;
        printq(jobQueue);
        printq(running);
        while (!jobQueue.empty() && numFree() > 4 && running.size() < 26 && jobQueue.front().arrivalTime <= t) {  
            // fill up running queue with all jobs from jobQue that have arrived
            // this doesn't work if we arent clearing out finished jobs
            startJob(t);
        }
        std::sort(running.begin(), running.end(), &jobcmp); // maybe this sorting algo isnt great

        for (auto it = running.begin(); it != running.end();) { // go through all running processes
            if (it->remainingTime == 0) {   // remove all jobs that are done
                finishJob(*it, t);
                pageTable.removeProcess(it->id);
                it = running.erase(it);
            }
            else {
                int frameno = assignPage(t, *it, [this]() { return this->findLRUVictim(); }, seed);
                memory.at(frameno) = it->id;
                it->remainingTime--;
                ++it;
            }
        }
        for (auto &it : memory) std::cout<<it<<" ";
        std::cout<<"\n"<<std::endl;
        t++;
    }
    std::cout<<"out of time! just finishing up all the already started procs"<<std::endl;
    while (!running.empty()) {
        for (auto it = running.begin(); it != running.end();) { // go through all running processes
            if (it->remainingTime == 0) {   // remove all jobs that are done
                finishJob(*it, t);
                pageTable.removeProcess(it->id);
                it = running.erase(it);
            }
            else {
                std::cout<<"HAHAHAHAHA"<<std::endl;
                int frameno = assignPage(t, *it, [this]() { return this->findLRUVictim(); }, seed);
                memory.at(frameno) = it->id;
                it->remainingTime--;
                ++it;
            }
        }
        t++;
    }
    return 0;
}


int Memory::findLRUVictim() {
    std::cout<<"LRU"<<std::endl;
    // LRU: Find the frame with the oldest (minimum) lastAccessTime
    int lruFrame = -1;
    int minAccessTime = -1;
    bool foundValidAccessTime = false;
    
    // Scan through all frames in memory
    for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
        const int pid = memory[frameNum];
        
        // Skip free frames (processId == -1 means frame is free)
        if (pid == -1) {
            continue;
        }
        
        // Get the corresponding PTE using processId and pageNumber from frame
        PageTableEntry* pte = pageTable.getEntry(pid, frameNum);
        
        if (pte == nullptr || !pte->valid) {
            // This shouldn't happen if memory is consistent, but handling it either way
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
        for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
            if (memory[frameNum] != -1) {
                lruFrame = frameNum;
                break;
            }
        }
    }
    return lruFrame;
}

int Memory::findFIFOVictim(){
    std::cout<<"FIFO"<<std::endl;
    // FIFO: Find the oldest frame (first one to be added)
    int FIFOFrame = -1;
    int minloadTime = -1;
    bool foundValidloadTime = false;
    
    // Scan through all frames in memory
    for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
        const int pid = memory[frameNum];
        
        // Skip free frames (processId == -1 means frame is free)
        if (pid == -1) {
            continue;
        }
        
        // Get the corresponding PTE using processId and pageNumber from frame
        PageTableEntry* pte = pageTable.getEntry(pid, frameNum);
        
        if (pte == nullptr || !pte->valid) {
            // This shouldn't happen if memory is consistent, but handling it either way
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
        for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
            if (memory[frameNum] != -1) {
                FIFOFrame = frameNum;
                break;
            }
        }
    }
    return FIFOFrame;
}
