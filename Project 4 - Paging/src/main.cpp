#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "page.h"  //Functions to run the threads on


int main(int argc, char* argv[]){
    u_int32_t seed = 0;
    const int numJobs = 150;
    if (argc > 2 || argc <= 0) {
        std::cout<<"USAGE: ./main [seed]"<<std::endl;
        return 1;
    }
    else if (argc == 2) {
        seed = std::stoi(argv[1]);
        if (seed <= 0) {
            std::random_device rd;
            seed = rd();
        }
    }

    Memory mem(seed, 100, numJobs);
    for (auto &it : mem.jobQueue) {	// jobs that are ready but not running are waiting
        std::cout<<it<<std::endl;
    }

    return 0;
}