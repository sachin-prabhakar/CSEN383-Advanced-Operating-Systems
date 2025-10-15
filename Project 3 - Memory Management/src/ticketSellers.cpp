#include "ticketSellers.h"

//Mutex used to lock the datastructure
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Create a 10x10 array to keep track of the seats
std::array<std::array<std::string,10>,10> seats;

//Used to synchronize the 10 threads before they begin the main loop
std::barrier sync_threads(10);

//Function for ticket seller
void *ticketSeller(void *arg){
    int trackedTime = 0;

    threadData *data = static_cast<threadData*>(arg);

    int N = data->num_customers;
    char t = data->seller_type;

    //Generate a random seed
    std::random_device rd;
    std::mt19937 gen(rd());

    //Arrival time: Int between 0 and 60 non inclusive
    std::uniform_int_distribution<int> arrival(0, 59);

    //Distributions for threads
    std::mt19937 seed(rd());
    std::uniform_int_distribution<int> H_time(1, 2);
    std::uniform_int_distribution<int> M_time(2, 4);
    std::uniform_int_distribution<int> L_time(4, 7);


    std::priority_queue<int, std::vector<int>, std::greater<int>> Customers;

    for(int j=0; j<N; j++){
        Customers.push(arrival(gen));
    }

    //Wait for all threads to arrive at this point before continuing.
    sync_threads.arrive_and_wait();

    //Loop condition for ticket sellers
    while(!Customers.empty() || trackedTime < 60){

        if(Customers.top() <= trackedTime){
            pthread_mutex_lock(&mutex);

            switch(t){
                case 'H':
                    /*
                    Search for an empty seat depending on which type of seller this is
                    
                    */
                    trackedTime = trackedTime + H_time(seed);

                case 'M':
                    /*
                    Search for an empty seat depending on which type of seller this is
                    
                    */
                    trackedTime = trackedTime + M_time(seed);

                case 'L':
                    /*
                    Search for an empty seat depending on which type of seller this is
                    
                    */
                    trackedTime = trackedTime + L_time(seed);
            }
            pthread_mutex_unlock(&mutex);
        }else{
            trackedTime = trackedTime + 1;
        }   
    }

    return NULL;

}