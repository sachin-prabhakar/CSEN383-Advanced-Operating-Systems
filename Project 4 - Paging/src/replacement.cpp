#include "memory.h"


int Memory::findLRUVictim() {
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
        PageTableEntry* pte = pageTable.getEntry(pid, pageTable.getVpn(pid,frameNum));
        
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
        PageTableEntry* pte = pageTable.getEntry(pid,  pageTable.getVpn(pid,frameNum));
        
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

int Memory::findLFUVictim() {
    // LFU: Find the frame with the lowest accessCount
    int lfuFrame = -1;
    int minAccessCount = -1;
    bool foundValidAccessTime = false;
    
    // Scan through all frames in memory
    for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
        const int pid = memory[frameNum];
        
        // Skip free frames (processId == -1 means frame is free)
        if (pid == -1) {
            continue;
        }
        
        // Get the corresponding PTE using processId and pageNumber from frame
        PageTableEntry* pte = pageTable.getEntry(pid, pageTable.getVpn(pid,frameNum));
        
        if (pte == nullptr || !pte->valid) {
            // This shouldn't happen if memory is consistent, but handling it either way
            continue;
        }
        
        // For LFU, we want the frame with minimum accessCount
        // If accessCount is valid (not -1), use it for comparison
        if (pte->accessCount != -1) {
            if (!foundValidAccessTime || pte->accessCount < minAccessCount) {
                lfuFrame = frameNum;
                minAccessCount = pte->accessCount;
                foundValidAccessTime = true;
            }
        } else if (!foundValidAccessTime) {
            // If we haven't found any frame with valid access time yet,
            // use this frame as candidate (though accessCount is -1)
            // This handles edge cases where pages haven't been accessed yet
            if (lfuFrame == -1) {
                lfuFrame = frameNum;
            }
        }
    }
    
    // If no valid frame found (all frames are free), return -1
    // Otherwise return the LFU frame (or first occupied frame if no valid access times found)
    if (lfuFrame == -1) {
        // Fallback: find first occupied frame (shouldn't happen if memory is full)
        for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
            if (memory[frameNum] != -1) {
                lfuFrame = frameNum;
                break;
            }
        }
    }
    return lfuFrame;
}


int Memory::findMFUVictim() {
    // MFU: Find the frame with the highest accessCount
    int mfuFrame = -1;
    int maxAccessCount = -1;
    bool foundValidAccessTime = false;
    
    // Scan through all frames in memory
    for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
        const int pid = memory[frameNum];
        
        // Skip free frames (processId == -1 means frame is free)
        if (pid == -1) {
            continue;
        }
        
        // Get the corresponding PTE using processId and pageNumber from frame
        PageTableEntry* pte = pageTable.getEntry(pid, pageTable.getVpn(pid,frameNum));
        
        if (pte == nullptr || !pte->valid) {
            // This shouldn't happen if memory is consistent, but handling it either way
            continue;
        }
        
        // For MFU, we want the frame with maximum accessCount
        // If accessCount is valid (not -1), use it for comparison
        if (pte->accessCount != -1) {
            if (!foundValidAccessTime || pte->accessCount > maxAccessCount) {
                mfuFrame = frameNum;
                maxAccessCount = pte->accessCount;
                foundValidAccessTime = true;
            }
        } else if (!foundValidAccessTime) {
            // If we haven't found any frame with valid access time yet,
            // use this frame as candidate (though accessCount is -1)
            // This handles edge cases where pages haven't been accessed yet
            if (mfuFrame == -1) {
                mfuFrame = frameNum;
            }
        }
    }
    
    // If no valid frame found (all frames are free), return -1
    // Otherwise return the MFU frame (or first occupied frame if no valid access times found)
    if (mfuFrame == -1) {
        // Fallback: find first occupied frame (shouldn't happen if memory is full)
        for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
            if (memory[frameNum] != -1) {
                mfuFrame = frameNum;
                break;
            }
        }
    }
    return mfuFrame;
}

int Memory::findRandomVictim() {
    // Random: Select a random frame that is currently occupied
    std::vector<int> occupiedFrames;

    // Collect indices of all occupied frames
    for (size_t frameNum = 0; frameNum < memory.size(); frameNum++) {
        if (memory[frameNum] != -1) { // occupied frame
            occupiedFrames.push_back(frameNum);
        }
    }

    // If no occupied frames, return -1
    if (occupiedFrames.empty()) {
        return -1;
    }

    std::uniform_int_distribution<int> dist(0, occupiedFrames.size() - 1);
    int randomIndex = dist(gen);

    return occupiedFrames[randomIndex];
}