#include "ticketSellers.h"
#include <iostream>

//Mutex used to lock the datastructure
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Create a 10x10 array to keep track of the seats
static std::string seats[10][10] = {""};

//Used to synchronize the 10 threads before they begin the main loop
std::barrier sync_threads(10);

static int thread_id_M = 0;
static int thread_id_L = 0;


void printTable(const std::string (&seat)[10][10]){
    std::cout<<"--------------------BEGIN NEW TABLE ----------------------"<<std::endl;

    for(int row = 0; row < 10; row++){
        for(int col = 0; col < 10; col++){
            if(seat[row][col].length() == 0){
                std::cout<<"----"<<"\t";
            }else{
                std::cout<<seat[row][col]<<"\t";
            }
        }
        std::cout<<"\n";
    }
    std::cout<<std::endl;
}



//Function for ticket seller
void *ticketSeller(void *arg){
   // printTable(seats);
    int trackedTime = 0;

    threadData *data = static_cast<threadData*>(arg);

    int N = data->num_customers;
    char t = data->seller_type;

    int thread_M;
    int thread_L;

    if(t == 'M')
        thread_M = thread_id_M++;
    if(t == 'L')
        thread_L = thread_id_L++;

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

    int customer = 0;
    //Loop condition for ticket sellers
    while(!Customers.empty() && trackedTime < 60){
        

        if(Customers.top() <= trackedTime){
            pthread_mutex_lock(&mutex);
             customer++;


            switch(t){
                case 'H':
                    for(int row = 0; row < 10; row++){
                        for(int col = 0; col < 10; col++){
                            if(seats[row][col].length() == 0){
                                seats[row][col] = t+std::to_string(1) + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                                row = 10;
                                break;
                            }
                        }
                    }
                    //std::cout<<"Time: "<<trackedTime<<"Seller H serving customer"<<std::endl;
                    /*
                    Search for an empty seat depending on which type of seller this is
                    
                    */
                    trackedTime = trackedTime + H_time(seed);
                    break;
                case 'M':
                    for(int i=1; i<10; i++){
                        int row= 5 + i/2 * pow(-1,i);
                        for(int col = 0; col<10; col++){
                            if(seats[row][col].length() == 0){
                                // seats[row][col] = t+thread_M;
                                seats[row][col] = (t+std::to_string(thread_M) + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer)));
                                i = 10;
                                break;
                            }   
                        }
                    }
                    //std::cout<<"Time: "<<trackedTime<<"Seller M serving customer"<<std::endl;
                    trackedTime = trackedTime + M_time(seed);
                    break;
                case 'L':
                    for(int row = 9; row >= 0; row--){
                        for(int col = 9; col >= 0; col--){
                            if(seats[row][col].length() == 0){
                            //    seats[row][col] = t+thread_L;
                                seats[row][col] = t+std::to_string(thread_L) + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                                row = -1;
                                break;
                            }
                        }
                    }
                    //std::cout<<"Time: "<<trackedTime<<"Seller L serving customer"<<std::endl;
                    trackedTime = trackedTime + L_time(seed);
                    break;
            }
            Customers.pop();
            pthread_mutex_unlock(&mutex);
        }else{
            trackedTime = trackedTime + 1;
        } 
        
    }
    pthread_mutex_lock(&mutex);
    printTable(seats);
    pthread_mutex_unlock(&mutex);



    return NULL;

}