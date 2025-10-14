#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <string>

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
    
    


    return 1;
}