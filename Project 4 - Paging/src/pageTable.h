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

    // Statistics
    // int totalReferences;
    // int pageHits;
    // int pageMisses;
    // int currentTime; // Current simulation time in 100ms units