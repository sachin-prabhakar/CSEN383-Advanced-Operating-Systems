#include <random>
#include <list>

struct Process {
    int id;
    int size;       // process size in MB (set at construction)
    int duration;   // initial process duration (set at construction)
    int remaining;  // remaining time in process (starts at -1 until process begins)
    static int numProcs;

    Process(int size, int duration);
    Process(std::mt19937& gen);
    friend bool operator<(const Process& lhs, const Process& rhs);
};

std::list<Process> generateJobs(int numJobs = 150, u_int32_t seed = 42);