#include "memory.h"


Memory::Memory(uint32_t seed, int numJobs) : gen(seed) {
    jobQueue = generateJobs(seed, numJobs); // jobQueue is sorted by arrival time
    running= {};
    finished = {};
    for (auto &it : memory)   it = -1;
    pageTable = PageTable();
    log = Log();
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


int Memory::getNewVpn(const Job &job, uint32_t seed) {
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

int Memory::assignPage(int t, Job &job, std::function<int(int)> replacementAlgo, uint32_t seed) { // returns ppn if successful, else -1
    int frameno = -1;
    int newvpn = getNewVpn(job, seed);
    if (pageTable.isValid(job.id, newvpn)) {
        pageTable.updateAccess(job.id, newvpn, t);
        job.currentPage = newvpn;
        log.newentry(job.id, t, 0);
        return pageTable.lookup(job.id, newvpn);
    }
    if (numFree() == 0) {
        log.newentry(job.id, t, 1);
        frameno = replacementAlgo(newvpn);
        int invalidid = memory.at(frameno);
        int invalidvpn = pageTable.lookup(invalidid, pageTable.getVpn(invalidid, frameno));

        pageTable.invalidateEntry(invalidid, invalidvpn);
        std::cout<<"Page Replacement ";
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
    if (job.currentPage == -1) {
        // job is being run for the first time
        job.currentPage = 0;
        pageTable.updateEntry(job.id, 0, frameno, t);
    } 
    else {  // job has run before
        pageTable.updateAccess(job.id, newvpn, t);
        job.currentPage = newvpn;
    }
    std::cout<<"Assigning job "<<job.id<<" vpn "<<job.currentPage<<" to frame "<<frameno<<std::endl;
    log.newentry(job.id, t, 0);
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

int Memory::run(std::function<int(int)> replacementAlgo, uint32_t seed) {
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
                int frameno = assignPage(t, *it, replacementAlgo, seed);
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
        std::cout<<"t = "<<t<<std::endl;
        printq(jobQueue);
        printq(running);
        for (auto it = running.begin(); it != running.end();) { // go through all running processes
            if (it->remainingTime == 0) {   // remove all jobs that are done
                finishJob(*it, t);
                pageTable.removeProcess(it->id);
                it = running.erase(it);
            }
            else {
                int frameno = assignPage(t, *it, replacementAlgo, seed);
                memory.at(frameno) = it->id;
                it->remainingTime--;
                ++it;
            }
        }
        for (auto &it : memory) std::cout<<it<<" ";
        std::cout<<"\n"<<std::endl;
        t++;
    }

    std::cout<<"still in jobQueue proc "<<jobQueue.front().id<<" with arrivaltime= "<<jobQueue.front().arrivalTime<<std::endl;
    return 0;
}

void Memory::printFinished() {
    std::cout<<"PRINTING FINISHED JOBS\nid\tat\tsize\tst\tft"<<std::endl;
    for (auto &it : finished) {
        std::cout<<it.id<<"\t"<<it.arrivalTime<<"\t"<<it.procSize<<"\t"<<it.serviceTime<<"\t"<<it.finishTime<<std::endl;
    }
}
