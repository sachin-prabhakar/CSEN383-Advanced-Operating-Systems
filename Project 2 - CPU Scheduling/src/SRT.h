#pragma once
/*
Shortest Remaining Time
Ethan
*/

#include <queue>
#include "Processes.h"


// Logic for sorting processes.  If true, proc1 goes after proc2	*double check this
// sorted in descending order to make popping easier
bool remainingTimeSort(const Process& job1, const Process& job2);

int SRT(std::queue<Process> &processes);
