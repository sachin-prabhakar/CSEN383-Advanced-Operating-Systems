#include <random>
#include <list>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#pragma once

// Page replacement algorithm types
enum class PageReplacementAlgorithm {
    FIFO,
    LRU,
    LFU,
    MFU,
    RANDOM
};

// Page frame structure
struct PageFrame {
    int processId;      // Process ID using this frame (-1 if free)
    int pageNumber;     // Page number within the process
    int lastAccessTime; // For LRU
    int accessCount;    // For LFU/MFU
    int loadTime;       // For FIFO
};

// Process page tracking
struct ProcessPageInfo {
    int processId;
    int currentPage;        // Current page being referenced
    int lastReferenceTime;  // Last time this process made a reference
    std::vector<bool> pagesInMemory; // Which pages are currently in memory
    std::vector<int> pageFrames;     // Which frame each page is in (-1 if not in memory)
};

struct Process {
    int id;
    int size;       // process size in MB (set at construction)
    int duration;   // initial process duration (set at construction)
    int remaining;  // remaining time in process (starts at -1 until process begins)
    int arrival;    // jobs arrive between 0 and 599 (60 seconds measured every 100ms)
    static int numProcs;

    Process(int size, int duration, int arrival);
    Process(u_int32_t seed);
    friend bool operator<(const Process& lhs, const Process& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Process& job);
};

struct Memory {
    int totalPages;
    std::list<Process> jobQueue;    // jobs that do not have memory allocated
    std::list<Process> running;     // all currently running jobs (with allocated pages)
    std::vector<PageFrame> pageFrames;  // all page frames available in hardware and the Process ID using it
    std::map<int, ProcessPageInfo> processPages; // Track pages for each running process

    // Page replacement algorithm state
    PageReplacementAlgorithm algorithm;
    std::queue<int> fifoQueue;          // For FIFO algorithm
    std::map<int, int> lruMap;          // For LRU algorithm (frame -> last access time)
    std::map<int, int> lfuMap;          // For LFU algorithm (frame -> access count)
    std::map<int, int> mfuMap;          // For MFU algorithm (frame -> access count)

    // Statistics
    int totalReferences;
    int pageHits;
    int pageMisses;
    int currentTime; // Current simulation time in 100ms units

    Memory(u_int32_t seed, int frames = 100, int numJobs = 150);
    Memory(std::list<Process>& jq, int frames = 100);

    //Core memory management functions
    int numFree();     // returns number of free pages

    // TODO: Add other function declarations as needed
};

std::list<Process> generateJobs(u_int32_t seed, int numJobs = 150);
