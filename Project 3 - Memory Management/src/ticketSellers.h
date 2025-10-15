#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>   //std::string
#include <array>
#include <queue>    //std::queue
#include <random>
#include <barrier>  //syncing threads

struct threadData{
    int num_customers;
    char seller_type;
    threadData(int c, char t) : num_customers(c), seller_type(t){}
};


//Mutex used to lock the datastructure
extern pthread_mutex_t mutex;

//Create a 10x10 array to keep track of the seats
extern std::array<std::array<std::string,10>,10> seats;

//Function for ticket seller
void *ticketSeller(void *arg);