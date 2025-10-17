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

struct Customer{
    int arrivalTime;
    int processingTime;

    //Min heap sort
    bool operator<(const Customer &c1) const{
        return arrivalTime > c1.arrivalTime;
    }

    Customer(int a, int p) : arrivalTime(a), processingTime(p) {};

};

static int timeElapsed = 0;


//Function for ticket seller
void *ticketSellers1(void *arg){

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


    std::priority_queue<Customer> Customers;

    for(int j=0; j<N; j++){
        if(t == 'H'){
            Customer tempCustomer = Customer(arrival(gen),H_time(seed));
            Customers.push(tempCustomer);
        }else if(t == 'M'){
            Customer tempCustomer = Customer(arrival(gen),M_time(seed));
            Customers.push(tempCustomer);
        }else{
            Customer tempCustomer = Customer(arrival(gen),L_time(seed));
            Customers.push(tempCustomer);
        }        
    }
    
    //Wait for all threads to arrive at this point before continuing.
    sync_threads.arrive_and_wait();

    int customer = 0;
    //Loop condition for ticket sellers
    while(!Customers.empty() && timeElapsed < 60){

        std::cout<<"Time: "<<timeElapsed<<std::endl;
        if(Customers.top().arrivalTime <= timeElapsed && Customers.top().processingTime == 0){
            pthread_mutex_lock(&mutex);
            customer++;
            switch(t){
                case 'H':
                {
                    std::string name = t+std::to_string(1) + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                    for(int row = 0; row < 10; row++){
                        for(int col = 0; col < 10; col++){
                            if(seats[row][col].length() == 0){
                                seats[row][col] = name;
                                row = 10;
                                break;
                            }
                        }
                    }
                    std::cout<<"Time: "<<timeElapsed<<" Seller H serving customer"<<std::endl;
                    /*
                    Search for an empty seat depending on which type of seller this is
                    
                    */
                    break;
                }
                case 'M':
                {
                    std::string name = t+std::to_string(thread_M) + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                    for(int i=1; i<10; i++){
                        int row= 5 + i/2 * pow(-1,i);
                        for(int col = 0; col<10; col++){
                            if(seats[row][col].length() == 0){
                                seats[row][col] = name;
                                i = 10;
                                break;
                            }   
                        }
                    }
                    std::cout<<"Time: "<<timeElapsed<<" Seller M serving customer"<<std::endl;
                    break;
                }
                case 'L':
                {
                    std::string name = t+std::to_string(thread_L) + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                    for(int row = 9; row >= 0; row--){
                        for(int col = 9; col >= 0; col--){
                            if(seats[row][col].length() == 0){
                                seats[row][col] = name;
                                row = -1;
                                break;
                            }
                        }
                    }
                    std::cout<<"Time: "<<timeElapsed<<" Seller L serving customer"<<std::endl;
                    break;
                }
            }
            Customers.pop();
            pthread_mutex_unlock(&mutex);
        }

        if(Customers.top().arrivalTime <= timeElapsed && Customers.top().processingTime > 0){
            Customer temp = Customers.top();
            Customers.pop();
            temp.processingTime --;
            Customers.push(temp);
        }

        timeElapsed++;         
    }

    pthread_mutex_lock(&mutex);
    printTable(seats);
    pthread_mutex_unlock(&mutex);



    return NULL;

}