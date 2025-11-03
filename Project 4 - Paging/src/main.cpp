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
    if (argc > 3 || argc <= 1) {
        std::cout<<"USAGE: ./main algo [seed]"<<std::endl;
        std::cout<<"seed<=0 -> random seed\n";
        std::cout<<"no seed -> seed=0\n";
        std::cout<<"algos: lru, lfu, mfu, fifo, rand\n";
        return 1;
    }
    else if (argc == 3) {
        seed = std::stoi(argv[2]);
        if (seed <= 0) {
            std::random_device rd;
            seed = rd();
        }
    }
    std::string algcode = argv[1];
    Memory mem(seed, numJobs); 

    std::function<int(int)> algo;
    if      (algcode == "lru") algo = [&mem](int vpn) { return mem.findLRUVictim(vpn); };
    else if (algcode == "lfu") algo = [&mem](int vpn) { return mem.findLFUVictim(vpn); };
    else if (algcode == "mfu") algo = [&mem](int vpn) { return mem.findMFUVictim(vpn); };
    else if (algcode == "fifo") algo = [&mem](int vpn) { return mem.findFIFOVictim(vpn); };
    else if (algcode == "rand") algo = [&mem](int vpn) { return mem.findRandomVictim(vpn); };

    else {
        std::cout<<"Please enter valid algorithm"<<std::endl;
        return 1;
    }

    mem.run(algo);
    mem.printFinished();
    mem.log.print();

    return 0;
}