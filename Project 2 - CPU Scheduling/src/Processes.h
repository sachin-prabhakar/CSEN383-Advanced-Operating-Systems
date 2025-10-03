#include <random>
#include <queue>
#include <vector>
#include <algorithm>

//Data structure for each process
struct Process {

    float arrivalTime;
    float expectedRunTime;
    int priority;
    char id;
};

//Function to create a new process and initialize with random values
Process createProcess(){
    Process process;

    //Random number generator to produce non-deterministic random numbers
    std::random_device rd;

    //Mersenne Twister pseudo-random generator of 32-bit numbers
    std::mt19937 gen(rd());

    //Float from 0 inclusive to 99 exclusive
    std::uniform_real_distribution<float> arrival(0, 99);

    //Float from 1 inclusive to 10 exclusive
    std::uniform_real_distribution<float> runtime(1, 10);

    //Int from 1 to 4 inclusive
    std::uniform_int_distribution<int> prio(1, 4);

    //Initialize variables
    process.arrivalTime = arrival(gen);
    process.expectedRunTime = runtime(gen);
    process.priority = prio(gen);

    return process;
}

//Logic for sorting processes.  If true, proc1 goes before proc2
bool arrivaltimeSort(const Process& proc1, const Process& proc2){
    return proc1.arrivalTime < proc2.arrivalTime;
}

//Function to return a queue of processes sorted by arrival time
std::queue<Process> createProcessQueue(int numProcesses){

    //Input checking
    if(numProcesses <= 0 ){
        std::cerr << "\033[31m" <<"ERROR: INVALID NUMBER OF PROCESSES.  MUST ENTER A POSITIVE NUMBER." << "\033[0m" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Create a Vector which can be sorted with std::sort
    std::vector<Process> procs;

    //Logic to assign alphabetical id's to each process
    int iterator = 0;
    for(char id = 'A'; id < 'Z'; id++){
        Process proc = createProcess();
        proc.id = id;

        procs.push_back(proc);

        iterator++;

        if(iterator == numProcesses){
            break;
        }     
    }

    //Sort the Vector in-place by arrival time.
    std::sort(procs.begin(), procs.end(), &arrivaltimeSort);

    //Create output queue
    std::queue<Process> procsout;
    
    //Copy the Vector into the queue
    for(int i = 0; i < numProcesses; i++){
        procsout.push(procs[i]);
    }

    return procsout;
}

//Depreciated 
std::vector<Process> create_Processlist(int numProcesses){

    std::vector<Process> procs;
    int iterator = 0;

    for(char id = 'A'; id < 'Z'; id++){
        Process proc = createProcess();
        proc.id = id;

        procs.push_back(proc);

        iterator++;

        if(iterator == numProcesses){
            break;
        }     
    }

    std::sort(procs.begin(), procs.end(), &arrivaltimeSort);

    return procs;
}

//Depreciated 
std::queue<Process> create_ProcessQueue(int numProcesses){

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