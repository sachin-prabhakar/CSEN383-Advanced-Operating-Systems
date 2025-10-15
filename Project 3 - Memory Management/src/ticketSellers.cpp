#include "ticketSellers.h"

//Mutex used to lock the datastructure
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Create a 10x10 array to keep track of the seats
std::array<std::array<std::string,10>,10> seats;

//Function for High priced ticket seller
void *highTickets(void *arg){
    int N = *static_cast<int*>(arg);

    for(int minute = 0; minute < 60; minute ++){

        //If there is a customer ....

        pthread_mutex_lock(&mutex);
        /*
        Find a ticket for the customer and take, randomly,
        1 or 2 minutes to complete a sale.
        
        Start from row 1 and search towards the back
        */
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

//Function for Medium priced ticket seller
void *MediumTickets(void *arg){
    int N = *static_cast<int*>(arg);

    for(int minute = 0; minute < 60; minute ++){

        //If there is a customer ....

        pthread_mutex_lock(&mutex);
        /*
        Find a ticket for the customer and take, randomly,
        2, 3, or 4 minutes to complete a sale.
        
        Start from row 5 then 6 then 4 then 7 ...
        */
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

//Function for Low priced ticket seller
void *LowTickets(void *arg){
    int N = *static_cast<int*>(arg);

    for(int minute = 0; minute < 60; minute ++){

    
        //If there is a customer ....

        pthread_mutex_lock(&mutex);
        /*
        Find a ticket for the customer and take, randomly,
        4, 5, 6, or 7 minutes to complete a sale.
        
        Start from row 10 and search towards the front
        */
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

//Function to add customers to ticket sellers queues
void *moreCustomers(void *arg){

    return NULL;
}