#include <random>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

//Data structure for each process
struct Process {

    float arrivalTime;
    float expectedRunTime;
    int priority;
    char id;
};

//Function to create a new process and initialize with random values
Process createProcess(std::mt19937& gen){
    Process process;

    //Float from 0 to 99 inclusive
    std::uniform_real_distribution<float> arrival(0, std::nextafter(99.0f, std::numeric_limits<float>::max()));

    //Float from 0.1 to 10 inclusive
    std::uniform_real_distribution<float> runtime(0.1, std::nextafter(10.0f, std::numeric_limits<float>::max()));

    /*
    std::nextafter(XX.0f, std::numeric_limits<float>::max())

    This line of code will return the next representable float value of XX in the direction of <float>::max().
    So this line returns the next representable float value after 10.0.  This trick allows us to make the upper bound
    of std::uniform_real_distribution inclusive.
    
    */

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
std::queue<Process> createProcessQueue(int numProcesses, uint32_t seed = UINT_MAX){

    //Input checking
    if(numProcesses <= 0 || numProcesses > 26){
        std::cerr << "\033[31m" <<"ERROR: INVALID NUMBER OF PROCESSES.  MUST ENTER A POSITIVE NUMBER AND LESS THAN 27." << "\033[0m" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Create a Vector which can be sorted with std::sort
    std::vector<Process> procs;

    //Logic to assign alphabetical id's to each process
    int iterator = 0;

    //Logic to either select random or set seed.
    uint32_t generator_seed;
    if(seed == UINT_MAX){
        //Random number generator to produce a seed for the number generator below
        std::random_device rd;
        generator_seed = rd();
    }else{
        generator_seed = seed;
    }

    //Mersenne Twister pseudo-random generator of 32-bit numbers
    std::mt19937 gen(generator_seed);

    for(char id = 'A'; id <= 'Z'; id++){

        Process proc = createProcess(gen);
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

/*

WARNING: Depreciated

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
*/