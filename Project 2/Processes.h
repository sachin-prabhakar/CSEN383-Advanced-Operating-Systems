#include <random>
#include <queue>

struct Process {

    float arrivalTime;
    float expectedRunTime;
    int priority;
    char id;
};

Process createProcess(){
    Process process;

    std::random_device rd;
    std::mt19937 gen(rd());

    //Float from 0 inclusive to 99 exclusive
    std::uniform_real_distribution<float> arrival(0, 99);

    //Float from 1 inclusive to 10 exclusive
    std::uniform_real_distribution<float> runtime(1, 10);

    //Int from 1 to 4 inclusive
    std::uniform_int_distribution<int> prio(1, 4);

    process.arrivalTime = arrival(gen);
    process.expectedRunTime = runtime(gen);
    process.priority = prio(gen);

    return process;
}

/*

Still need to implement sorting the queue so processes are in order by arrival time.

*/
std::queue<Process> createProcessQueue(int numProcesses){

    std::queue<Process> procs;
    int iterator = 0;

    for(char id = 'A'; id < 'Z'; id++){
        Process proc = createProcess();
        proc.id = id;

        procs.push(proc);

        iterator++;

        if(iterator == numProcesses){
            break;
        }     
    }

    return procs;
}