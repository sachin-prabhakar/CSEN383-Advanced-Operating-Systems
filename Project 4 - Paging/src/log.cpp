#include "memory.h"

Log::Log() {
    log = {};
}

void Log::newentry(int pid, int t, int swap) {
    log.push_back(Logentry(pid, t, swap));
}

void Log::print() {
    int replacements = 0, other = 0;
    for (auto &it : log) {
        if (it.swap == 1) replacements++;
        else if (it.swap == 0) other++;
    }
    std::cout<<"memory had "<<replacements<<" calls to replacement algo and "<<other<<" memory accesses"<<std::endl;
}

Logentry::Logentry(int pid, int t, int swap) {
    this->pid = pid;
    this->t = t;
    this->swap = swap;
}