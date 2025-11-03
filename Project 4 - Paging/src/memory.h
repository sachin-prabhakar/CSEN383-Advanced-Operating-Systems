#pragma once
#include <random>
#include <iostream>
#include <map>
#include <queue>
#include <deque>
#include <array>
#include <functional>
#include "pageTable.h"
#include "jobs.h"


struct Logentry {
    int pid;    // process id
    int t;      // time
    int swap;   // 0 means no replacement algo, 1 means replacement algo

    Logentry(int pid, int t, int swap);
};

struct Log {
    std::deque<Logentry> log;

    Log();

    void print();
    void newentry(int pid, int t, int swap);
};



struct Memory {
    PageTable pageTable;
    Log log;
    std::deque<Job> jobQueue;  // jobs that have not yet had any memory allocated (sorted by arrival time)
    std::deque<Job> running;   // all currently running jobs (with allocated pages); pagetable stuff here
    std::deque<Job> finished;  // all jobs that have been finished 
    std::array<int, 100> memory;    // array of every page in mem; -1 means free, otherwise shows pid

    // generators for paging
    std::mt19937 gen;

    Memory(uint32_t seed, int numJobs = 150);

    //Core memory management functions
    int numFree();     // returns number of free pages
    void print();
    void printFinished();
    int run(std::function<int(int)> replacementAlgo, uint32_t seed = 42);
    int assignPage(int t, Job &job, std::function<int(int)> replacementAlgo = nullptr, uint32_t seed = 42);
    void finishJob(Job &job, int t);  // helper for run()
    void startJob(int t);   // helper for run()
    int getNewVpn(const Job &job, uint32_t seed = 42);

    // Page replacement algorithms
    int findLRUVictim(int vpn);  // Returns frame number of LRU victim page
    int findFIFOVictim(int vpn); // Returns frame number of FIFO victim page
    int findLFUVictim(int vpn);  // Returns frame number of LFU victim page
    int findMFUVictim(int vpn);  // Returns frame number of MFU victim page
    int findRandomVictim(int vpn);  // Returns frame number of a random victim page

};
