#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>   //std::string
#include <array>
#include <queue>    //std::queue


//Mutex used to lock the datastructure
extern pthread_mutex_t mutex;

//Create a 10x10 array to keep track of the seats
extern std::array<std::array<std::string,10>,10> seats;

//Function for High priced ticket seller
void *highTickets(void *arg);

//Function for Medium priced ticket seller
void *MediumTickets(void *arg);

//Function for Low priced ticket seller
void *LowTickets(void *arg);

//Function to add customers to ticket sellers queues
void *moreCustomers(void *arg);