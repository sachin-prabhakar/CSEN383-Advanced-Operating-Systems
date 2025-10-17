#include "ticketSellers.h"

//Mutex used to lock the datastructure
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Create a 10x10 array to keep track of the seats
static std::string seats[10][10] = {""};

//Variables for naming each Seller Thread
static int thread_id_M = 0;
static int thread_id_L = 0;


/*

    === Barriers and Barrier Functions ===

*/

//Sync 10 threads
std::barrier sync_threads(10);

//Sync 10 threads and increment clock
static int timeElapsed = 0;
void incrementWhile() noexcept {timeElapsed++; std::cout<<std::endl;}
std::barrier increment(10,incrementWhile);

//Sync 10 threads and print new time
void pt() noexcept {
    if(timeElapsed < 10){
        std::cout<<"Time: 0:0"<<timeElapsed;
    }else{
        std::cout<<"Time: 0:"<<timeElapsed;
    }
}
std::barrier printTime(10,pt);

//Sync 10 threads and print seating chart
static int unservedcustomers = 0;
static int handledcustomers = 0;
void printTable(){
    std::cout<<unservedcustomers<<" Customers could not be served in the allotted time."<<std::endl;
    std::cout<<handledcustomers<<" Customers were processed."<<std::endl;
    for(int row = 0; row < 10; row++){
        for(int col = 0; col < 10; col++){
            if(seats[row][col].length() == 0){
                std::cout<<"----"<<"\t";
            }else{
                std::cout<<seats[row][col]<<"\t";
            }
        }
        std::cout<<"\n";
    }
    std::cout<<std::endl;
}
std::barrier printResultsTable(10,printTable);


//Vector to print out who sold a ticket
static std::vector<std::string> sold;
void ps(){

    if(sold.size() > 0 ){
        std::cout<<" Sellers: ";
        for(unsigned long i=0; i<sold.size();i++){
            std::cout<<sold.at(i)<<", ";
        }
        std::cout<<"serving customers."<<std::endl;
        sold.clear();
    }else{
        std::cout<<" No active sellers at this time"<<std::endl;;
    }
}    
std::barrier printSellers(10, ps);

/*

    === End of Barriers and Barrier Functions ===

*/


std::priority_queue<Customer> createCustomerQueue(int c, char id, int num){
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

    for(int j=0; j<c; j++){
        if(id == 'H'){
            std::string inId = 'H' + std::to_string(num);
            Customer tempCustomer = Customer(arrival(gen),H_time(seed), inId);
            Customers.push(tempCustomer);
        }else if(id == 'M'){
            std::string inId = 'M' + std::to_string(num);
            Customer tempCustomer = Customer(arrival(gen),M_time(seed), inId);
            Customers.push(tempCustomer);
        }else{
            std::string inId = 'L' + std::to_string(num);
            Customer tempCustomer = Customer(arrival(gen),L_time(seed), inId);
            Customers.push(tempCustomer);
        }        
    }
    return Customers;
}

//Function for ticket seller
void *ticketSeller(void *arg){

    //Thread type Data
    threadData *data = static_cast<threadData*>(arg);
    int N = data->num_customers;
    char t = data->seller_type;

    //Thread specific Data
    int customer = 0; //Keep track of how many customers each thread served
    int thread_M;
    int thread_L;

    //Assign a unique numeric id to each thread
    std::priority_queue<Customer> Customers;
    if(t == 'M'){
        thread_M = thread_id_M++;
        //Call function to create the Customer queued
        Customers = createCustomerQueue(N,t,thread_M);
    }
     if(t == 'L'){
        thread_L = thread_id_L++;
        //Call function to create the Customer queue
        Customers = createCustomerQueue(N,t,thread_L);
    }
    if(t == 'H'){
        //Call function to create the Customer queue
        Customers = createCustomerQueue(N,t,1);
    }
    
    
    //Sync 10 threads before continuing
    sync_threads.arrive_and_wait();

    //Loop until time is up
    while(timeElapsed < 60){

        //Sync 10 threads and print time before continuing
        printTime.arrive_and_wait();

        if(!Customers.empty() && Customers.top().arrivalTime <= timeElapsed && Customers.top().processingTime == 0){
            pthread_mutex_lock(&mutex);
            //Increment number of customers that the current thread has served
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
                    handledcustomers++;
                    sold.push_back(name);
                    //std::cout<<"Seller "<<name<<" serving customer\t";
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
                    handledcustomers++;
                    sold.push_back(name);
                    //std::cout<<"Seller "<<name<<" serving customer\t";
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
                    handledcustomers++;
                    sold.push_back(name);
                    //std::cout<<"Seller "<<name<<" serving customer\t";
                    break;
                }
            }
            Customers.pop();
            pthread_mutex_unlock(&mutex);
        }

        if(!Customers.empty() && Customers.top().arrivalTime <= timeElapsed && Customers.top().processingTime > 0){
            Customer temp = Customers.top();
            Customers.pop();
            temp.processingTime --;
            Customers.push(temp);
        }

        //Sync 10 threads and print sellers before continuing
        printSellers.arrive_and_wait();

        //Sync 10 threads and increment time before continuing
        increment.arrive_and_wait();    

    }//end while

    pthread_mutex_lock(&mutex);
    if(!Customers.empty()){   
        unservedcustomers = unservedcustomers + Customers.size();   
    }
    pthread_mutex_unlock(&mutex);

    //Sync 10 threads and print seats before continuing
    printResultsTable.arrive_and_wait();

    return NULL;

}