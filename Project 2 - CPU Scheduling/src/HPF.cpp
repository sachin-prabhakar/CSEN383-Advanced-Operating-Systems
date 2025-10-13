#include "HPF.h"
#include <deque>
#include <algorithm>

// Live record to track runtime state
struct Live {
    Process p;
    int remaining;
    bool started;
};

static inline int highest_nonempty(std::deque<int> q[4]) {
    for (int pr = 0; pr < 4; ++pr) if (!q[pr].empty()) return pr;
    return -1;
}

std::vector<Process> HPF_preemptive(std::queue<Process> processes) {
    // Move input queue (sorted by arrival) into vector
    std::vector<Live> all;
    all.reserve(processes.size());
    while (!processes.empty()) {
        Process pr = processes.front(); processes.pop();
        all.push_back(Live{pr, pr.expectedRunTime, false});
    }

    // Four ready queues: priority 1..4 -> q[0]..q[3]
    std::deque<int> q[4];

    auto admit = [&](int t){
        for (int i = 0; i < (int)all.size(); ++i)
            if (all[i].p.arrivalTime == t)
                q[std::max(1,std::min(4,all[i].p.priority))-1].push_back(i);
    };

    auto purge_never_started = [&](){
        for (int pr = 0; pr < 4; ++pr) {
            std::deque<int> keep;
            while (!q[pr].empty()) {
                int idx = q[pr].front(); q[pr].pop_front();
                if (all[idx].started) keep.push_back(idx);
            }
            q[pr].swap(keep);
        }
    };

    auto has_started_work = [&](){
        for (auto &L : all) if (L.started && L.remaining > 0) return true;
        return false;
    };

    int t = 0;
    int running = -1;
    int makespan = 0;

    admit(0);

    auto sim_continue = [&](){
        if (t <= 99) return true;           // we may still first-start new work
        if (running != -1) return true;     // something is running
        if (has_started_work()) return true;// unfinished but started work remains
        // or any already-started item in queues
        for (int pr=0; pr<4; ++pr)
            for (int idx : q[pr])
                if (all[idx].started && all[idx].remaining > 0) return true;
        return false;
    };

    while (sim_continue()) {
        // admit arrivals for this quantum
        admit(t);

        // choose who runs this quantum
        bool allow_first_start = (t <= 99);

        // if nothing running, pick from highest non-empty queue (respect allow_first_start)
        if (running == -1) {
            for (int pr = 0; pr < 4 && running == -1; ++pr) {
                for (int k = 0; k < (int)q[pr].size(); ++k) {
                    int idx = q[pr][k];
                    if (allow_first_start || all[idx].started) {
                        q[pr].erase(q[pr].begin()+k);
                        running = idx;
                        break;
                    }
                }
            }
        } else {
            // priority preemption at boundary: if higher-priority queues have work,
            // we will rotate current after executing this quantum (RR inside level)
        }

        // execute exactly one quantum if we have a running process
        if (running != -1) {
            Live &L = all[running];
            if (!L.started) {
                L.started = true;
                L.p.startTime = t;
                L.p.responseTime = L.p.startTime - L.p.arrivalTime;
            }

            L.remaining -= 1;
            makespan = t + 1;

            if (L.remaining == 0) {
                L.p.completionTime = t + 1;
                L.p.turnaroundTime = L.p.completionTime - L.p.arrivalTime;
                L.p.waitTime       = L.p.turnaroundTime - L.p.expectedRunTime;
                running = -1;
            } else {
                // not finished -> RR within the same priority level (q=1)
                int pr = std::max(1,std::min(4,L.p.priority)) - 1;
                q[pr].push_back(running);
                running = -1;
            }
        }

        // advance time
        ++t;

        // after 99, ensure we will never first-start a new process
        if (t > 99) purge_never_started();
    }

    // collect completed processes (only those that actually ran)
    std::vector<Process> finished;
    finished.reserve(all.size());
    for (auto &L : all)
        if (L.started && L.p.completionTime >= 0)
            finished.push_back(L.p);

    // sort so your printResults() throughput (using back().completionTime) is correct
    std::sort(finished.begin(), finished.end(),
              [](const Process& a, const Process& b){ return a.completionTime < b.completionTime; });

    // your existing printer (does per-run averages + throughput)
    printResults(finished);
    return finished;
}
