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

// // Page frame structure
// struct PageFrame {
//     int processId;      // Process ID using this frame (-1 if free)
//     int pageNumber;     // Page number within the process
//     int lastAccessTime; // For LRU
//     int accessCount;    // For LFU/MFU
//     int loadTime;       // For FIFO
//     PageFrame() : processId(-1), pageNumber(-1), lastAccessTime(-1), accessCount(-1), loadTime(-1){}
// };

// // Process page tracking
// struct ProcessPageInfo {
//     int processId;
//     int currentPage;        // Current page being referenced
//     int lastReferenceTime;  // Last time this process made a reference
//     std::vector<bool> pagesInMemory; // Which pages are currently in memory
//     std::vector<int> pageFrames;     // Which frame each page is in (-1 if not in memory)
// };

struct PageFrame {
    int processId;      // Process ID using this frame (-1 if free)
    int pageNumber;     // Page number within the process
    PageFrame(): processId(-1), pageNumber(-1){}
};



struct PageTableEntry {
    int frameNumber;        // PPN; -1 if not in memory
    bool valid;
    bool referenced;
    bool dirty;
    int loadTime;           // for FIFO
    int lastAccessTime;     // for LRU
    int accessCount;        // for LFU/MFU

    PageTableEntry() : frameNumber(-1), valid(false), referenced(false),
                       dirty(false), loadTime(-1), lastAccessTime(-1), accessCount(0) {}
};

class PageTable {
private:
    // Map from process ID to their page table (which maps VPN to PageTableEntry)
    std::map<int, std::vector<PageTableEntry>> pageTables;

public:

    PageTable();

    void initializeProcess(int processId, int numPages);
    int lookup(int processId, int virtualPageNumber);   // returns ppn
    bool isValid(int processId, int virtualPageNumber);
    void updateEntry(int processId, int virtualPageNumber, int frameNumber, int currentTime);
    void invalidateEntry(int processId, int virtualPageNumber);
    void updateAccess(int processId, int virtualPageNumber, int currentTime);
    PageTableEntry* getEntry(int processId, int virtualPageNumber);
    void removeProcess(int processId);
    std::vector<int> getValidPages(int processId);
    int getFrameNumber(int processId, int virtualPageNumber);
    int getProcessSize(int processId);
    int getVpn(int processId, int frameNumber);
    // int referencePage(int processId, int virtualPageNumber);
};