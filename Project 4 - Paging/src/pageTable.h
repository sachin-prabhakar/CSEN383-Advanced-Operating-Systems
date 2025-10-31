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

<<<<<<< HEAD
    // Statistics
    // int totalReferences;
    // int pageHits;
    // int pageMisses;
    // int currentTime; // Current simulation time in 100ms units
=======
// Page Table Entry structure
struct PageTableEntry {
    int frameNumber;        // Physical frame number (-1 if not in memory)
    bool valid;             // Valid bit (true if page is in memory)
    bool referenced;        // Reference bit (for clock/second chance)
    bool dirty;             // Dirty bit (true if page has been modified)
    int loadTime;           // When page was loaded (for FIFO)
    int lastAccessTime;     // Last access time (for LRU)
    int accessCount;        // Access count (for LFU/MFU)

    PageTableEntry() : frameNumber(-1), valid(false), referenced(false),
                       dirty(false), loadTime(-1), lastAccessTime(-1), accessCount(0) {}
};

// Page Table class - manages virtual to physical address translation
class PageTable {
private:
    // Map from process ID to their page table (which maps VPN to PageTableEntry)
    std::map<int, std::vector<PageTableEntry>> pageTables;

public:
    // Constructor
    PageTable();

    // Initialize page table for a process
    void initializeProcess(int processId, int numPages);

    // Lookup: Get physical frame number for a virtual page
    // Returns -1 if page is not in memory (page fault)
    int lookup(int processId, int virtualPageNumber);

    // Check if a page is valid (in memory)
    bool isValid(int processId, int virtualPageNumber);

    // Update page table entry when a page is loaded into memory
    void updateEntry(int processId, int virtualPageNumber, int frameNumber, int currentTime);

    // Update page table entry when a page is evicted from memory
    void invalidateEntry(int processId, int virtualPageNumber);

    // Update access statistics for a page (for LRU, LFU, MFU)
    void updateAccess(int processId, int virtualPageNumber, int currentTime);

    // Get page table entry (for algorithms that need detailed info)
    PageTableEntry* getEntry(int processId, int virtualPageNumber);

    // Remove process from page table (when process terminates)
    void removeProcess(int processId);

    // Get all valid pages for a process
    std::vector<int> getValidPages(int processId);

    // Get frame number for a virtual page (returns -1 if not valid)
    int getFrameNumber(int processId, int virtualPageNumber);
};