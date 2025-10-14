#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>   //std::string
#include <array>    //std::array
#include <queue>    //std::queue
#include <pthread.h>

#include "ticketSellers.h"  //Functions to run the threads on

int main(int argc, char* argv[]){

    if(argc != 2){
        std::cerr<<"Incorrect arguments passed.  Usage: ./main <number of customers at a time>"<<std::endl;
        exit(EXIT_FAILURE);
    }

    //Number of customers to arrive at any given time
    int N;

    //Check to see if arugment is an integer
    try{
        N = std::stoi(argv[1]);
        if(N <= 0){
            throw std::out_of_range("Integer must be positive");
        }
    }catch(std::out_of_range){
        std::cerr<<"Integer must be greater than 0"<<std::endl;
        exit(EXIT_FAILURE);
    }catch(...){
        std::cerr<<"Argument must be an integer"<<std::endl;
        exit(EXIT_FAILURE);
    }

    //Create a 10x10 array to keep track of the seats
    std::array<std::array<std::string,10>,10> seats;
    
    //Threads for each seller
    pthread_t H1, M1, M2, M3, L1, L2, L3, L4, L5, L6;

    //(thread, attributes for the thread, function to run, args for function)
    pthread_create(&H1, NULL, highTickets, NULL);
    pthread_create(&M1, NULL, MediumTickets, NULL);
    pthread_create(&M2, NULL, MediumTickets, NULL);
    pthread_create(&M3, NULL, MediumTickets, NULL);
    pthread_create(&L1, NULL, LowTickets, NULL);
    pthread_create(&L2, NULL, LowTickets, NULL);
    pthread_create(&L3, NULL, LowTickets, NULL);
    pthread_create(&L4, NULL, LowTickets, NULL);
    pthread_create(&L5, NULL, LowTickets, NULL);
    pthread_create(&L6, NULL, LowTickets, NULL);

    //(thread to join, return value)
    pthread_join(H1, NULL);
    pthread_join(M1, NULL);
    pthread_join(M2, NULL);
    pthread_join(M3, NULL);
    pthread_join(L1, NULL);
    pthread_join(L2, NULL);
    pthread_join(L3, NULL);
    pthread_join(L4, NULL);
    pthread_join(L5, NULL);
    pthread_join(L6, NULL);

    return 1;
}