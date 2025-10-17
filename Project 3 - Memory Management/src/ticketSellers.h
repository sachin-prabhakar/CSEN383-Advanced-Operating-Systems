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

struct Customer{
    int arrivalTime;
    int processingTime;
    std::string sellerID;

    bool operator<(const Customer &c1) const{
        return arrivalTime > c1.arrivalTime;
    }

    Customer(int a, int p, std::string id) : arrivalTime(a), processingTime(p), sellerID(id) {};
};

//Function for ticket seller
void *ticketSeller(void *arg);

//Function to create a queue of customers for each thread
std::priority_queue<Customer> createCustomerQueue(int c, char id);