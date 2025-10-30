#include <random>
#include <list>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#pragma once

// Process page tracking
struct ProcessPageInfo {
    int processId;
    int currentPage;        // Current page being referenced
    int lastReferenceTime;  // Last time this process made a reference
    std::vector<bool> pagesInMemory; // Which pages are currently in memory
    std::vector<int> pageFrames;     // Which frame each page is in (-1 if not in memory)
};

