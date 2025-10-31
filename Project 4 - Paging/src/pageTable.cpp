#include "pageTable.h"

PageTable::PageTable() {}

void PageTable::initializeProcess(int processId, int numPages) {
    std::vector<PageTableEntry> pageTable;
    pageTable.resize(numPages);

    // initialize all entries as not in memory
    for (int i = 0; i < numPages; i++) {
        pageTable[i] = PageTableEntry();
    }

    pageTables[processId] = pageTable;
}

int PageTable::lookup(int processId, int virtualPageNumber) {

    if (pageTables.find(processId) == pageTables.end()) {
        return -1; // process not found
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    if (virtualPageNumber < 0 || virtualPageNumber >= (int)pageTable.size()) {
        return -1; // invalid page number
    }

    PageTableEntry& entry = pageTable[virtualPageNumber];

    if (entry.valid) {
        return entry.frameNumber;
    }

    return -1; // page not in memory
}

bool PageTable::isValid(int processId, int virtualPageNumber) {

    if (pageTables.find(processId) == pageTables.end()) {
        return false;
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    if (virtualPageNumber < 0 || virtualPageNumber >= (int)pageTable.size()) {
        return false;
    }

    return pageTable[virtualPageNumber].valid;
}

void PageTable::updateEntry(int processId, int virtualPageNumber, int frameNumber, int currentTime) {

    if (pageTables.find(processId) == pageTables.end()) {
        return; // process not found
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    if (virtualPageNumber < 0 || virtualPageNumber >= (int)pageTable.size()) {
        return; // invalid page number
    }

    PageTableEntry& entry = pageTable[virtualPageNumber];
    entry.frameNumber = frameNumber;
    entry.valid = true;
    entry.referenced = true;
    entry.loadTime = currentTime;
    entry.lastAccessTime = currentTime;
    entry.accessCount = 1;
}


void PageTable::invalidateEntry(int processId, int virtualPageNumber) {

    if (pageTables.find(processId) == pageTables.end()) {
        return; // process not found
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    if (virtualPageNumber < 0 || virtualPageNumber >= (int)pageTable.size()) {
        return; // invalid page number
    }

    PageTableEntry& entry = pageTable[virtualPageNumber];
    entry.valid = false;
    entry.frameNumber = -1;
    entry.referenced = false;
}

void PageTable::updateAccess(int processId, int virtualPageNumber, int currentTime) {

    if (pageTables.find(processId) == pageTables.end()) {
        return; // process not found
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    if (virtualPageNumber < 0 || virtualPageNumber >= (int)pageTable.size()) {
        return; // invalid page number
    }

    PageTableEntry& entry = pageTable[virtualPageNumber];

    if (entry.valid) {
        entry.referenced = true;
        entry.lastAccessTime = currentTime;
        entry.accessCount++;
    }
}

PageTableEntry* PageTable::getEntry(int processId, int virtualPageNumber) {

    if (pageTables.find(processId) == pageTables.end()) {
        return nullptr; // process not found
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    if (virtualPageNumber < 0 || virtualPageNumber >= (int)pageTable.size()) {
        return nullptr; // invalid page number
    }

    return &pageTable[virtualPageNumber];
}

void PageTable::removeProcess(int processId) {
    pageTables.erase(processId);
}

std::vector<int> PageTable::getValidPages(int processId) {
    std::vector<int> validPages;

    if (pageTables.find(processId) == pageTables.end()) {
        return validPages; // empty vector
    }

    std::vector<PageTableEntry>& pageTable = pageTables[processId];

    for (size_t i = 0; i < pageTable.size(); i++) {
        if (pageTable[i].valid) {
            validPages.push_back(i);
        }
    }

    return validPages;
}

int PageTable::getFrameNumber(int processId, int virtualPageNumber) {
    return lookup(processId, virtualPageNumber);
}
