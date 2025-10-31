#include "memory.h"
#include "pageTable.h"
#include "jobs.h"


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