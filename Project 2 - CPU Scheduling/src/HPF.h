/*
Highest Priority First - khushi
*/

#pragma once
#include <queue>
#include <vector>
#include "Processes.h"

// Highest Priority First
// HPF_nonpreemptive -> FCFS within each priority (no preemption)
// HPF_preemptive -> preemptive with RR(q=1) within each priority
std::vector<Process> HPF_nonpreemptive(std::queue<Process> processes, bool aging = true);
std::vector<Process> HPF_preemptive(std::queue<Process> processes, bool aging = true);

