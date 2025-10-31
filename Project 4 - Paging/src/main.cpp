#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "memory.h"
#include "jobs.h"
#include "pageTable.h"



int main(int argc, char* argv[]){
    uint32_t seed = 0;
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


    Memory mem(seed, numJobs);
    mem.print();

    return 0;
}