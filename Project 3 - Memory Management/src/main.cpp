#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "ticketSellers.h"  //Functions to run the threads on

int main(int argc, char* argv[]){

    if(argc != 2){
        std::cerr<<"Incorrect arguments passed.  Usage: ./main <number of customers at a time>"<<std::endl;
        exit(EXIT_FAILURE);
    }

    //Number of customers to arrive at any given time
    int N;

    //Check to see if argument is an integer
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

    //Struct defined in header file.
    threadData *high = new threadData(N, 'H');
    threadData *medium = new threadData(N, 'M');
    threadData *low = new threadData(N, 'L');

    //Threads for each seller
    pthread_t sellers[10];

    //High Seller
    pthread_create(&sellers[0], NULL, ticketSeller, high);
    //Medium Seller
    for(int s=1; s<4; s++){
        pthread_create(&sellers[s], NULL, ticketSeller, medium);
    }
    //Low Seller
    for(int s=4; s<10; s++){
        pthread_create(&sellers[s], NULL, ticketSeller, low);
    }

    //Wait for all threads
    for(int i=0; i<10; i++){
        pthread_join(sellers[i],NULL);
    }

    //Free memory from the heap
    delete high;
    delete medium;
    delete low;

    /*
    
    Print Results here
    
    */

    std::cout<<std::endl<<"Thread execution finished"<<std::endl;

    return 1;
}