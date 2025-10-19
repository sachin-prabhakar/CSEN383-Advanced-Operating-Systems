#include "ticketSellers.h"

//Mutex used to lock critical regions
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Create a 10x10 array to keep track of the seats
static std::string seats[10][10] = {""};

//Variables for naming each Seller Thread
static int thread_id_M = 0;
static int thread_id_L = 0;

//Vecotrs for final calcs
std::vector<Customer> hSellers;
std::vector<Customer> mSellers;
std::vector<Customer> lSellers;

//Track total time
static int timeElapsed = 0;

//Track the number of customers who have/haven't been served
static int unservedcustomers = 0;
static int handledcustomers = 0;

//Track the total number of customers being helped at a given time
static int curentCustomers = 0;

//Track the tickets being sold in a given time interval
static std::vector<std::string> sold;


/*
    === Barriers and Barrier Functions ===
*/

//Sync 10 threads
std::barrier sync_threads(10);

//Sync 10 threads and increment clock
void incrementWhile() noexcept{timeElapsed++; std::cout<<std::endl;}
std::barrier increment(10,incrementWhile);

//Sync 10 threads and print new time
void pt() noexcept{
    if(timeElapsed < 10){
        std::cout<<"0:0"<<timeElapsed<<" -\t";
    }else{
        std::cout<<"0:"<<timeElapsed<<" -\t";
    }
}
std::barrier printTime(10,pt);

//Sync 10 threads and print seating chart
void printTable(){
    std::cout<<"\n"<<unservedcustomers<<" Customers could not be served in the allotted time."<<std::endl;
    std::cout<<handledcustomers<<" Customers were processed.\n"<<std::endl;
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


//Sync 10 threads and print out which one is selling a ticket
void ps(){

    if(sold.size() > 0 ){
        std::cout<<"Sellers: ";
        for(unsigned long i=0; i<sold.size();i++){
            std::cout<<sold.at(i)<<", ";
        }
        std::cout<<"serving customers."<<std::endl;
        sold.clear();
    }else{
        std::cout<<"No active sellers at this time"<<std::endl;;
    }
}    
std::barrier printSellers(10, ps);

//Function to print out metrics in the function below
void helperPrint(double rt, double tat, char t, std::vector<Customer> C){
    double size = C.size();
    std::cout<<"\n-------- Results - "<<t<<" Sellers --------\n";
    std::cout<<"Avg Response Time: "<<rt/size<<"\n";
    std::cout<<"Avg Turn Around Time: "<<tat/size<<"\n";
    std::cout<<"Avg Throughput: "<<static_cast<double>(size)/60<<"\n";
    std::cout<<size<<" Customers of "<<t<<" got seats"<<"\n";
}
//Sync 10 threads and print out final metrics
void printMetric(){

    double RT = 0, TAT = 0;

    for(auto i : hSellers){
        RT += (i.startservice- i.officialArrivalTime);
        TAT += (i.completionTime - i.officialArrivalTime);
    }
    helperPrint(RT,TAT,'H',hSellers);
    RT = 0, TAT = 0;

    for(auto i : mSellers){
        RT += (i.startservice- i.officialArrivalTime);
        TAT += (i.completionTime - i.officialArrivalTime);

    }
    helperPrint(RT,TAT,'M',mSellers);
    RT = 0, TAT = 0;

    for(auto i : lSellers){
         RT += (i.startservice- i.officialArrivalTime);
        TAT += (i.completionTime - i.officialArrivalTime);
    }
    helperPrint(RT,TAT,'L',lSellers);

}
std::barrier printMetrics(10, printMetric);

/*
    === End of Barriers and Barrier Functions ===
*/

/*
    === Event Functions ===
*/
void printArrivals(std::string id, std::priority_queue<Customer> C, int time){
    while(!C.empty() && C.top().arrivalTime == time){
        std::cout<<"New customer has arrived at seller "<<id<<std::endl<<"\t";
        C.pop();
    }
}

void printunservedCustomers(std::string id, std::priority_queue<Customer> C){
    std::cout<<"Seller "<<id<<" had to turn away "<<C.size()<<" customers"<<std::endl;
}

/*
    === End of Event Functions ===
*/

//Function called when a seat is found
void seatFound(int r, int c, std::string n, bool &f, std::priority_queue<Customer> &C){
    seats[r][c] = n;
    handledcustomers++;
    curentCustomers++;
    sold.push_back(n);
    Customer temp = C.top();
    C.pop();
    temp.arrivalTime = 0; //Artificially decrease arrival time to keep entry at top of pq
    temp.helped = true;
    temp.startservice = timeElapsed;
    C.push(temp);
    f = true;
}

//Function to generate customer queues with random runtime pre-calculated
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

    //ID for each thread
    std::string threadID;

    if(t == 'M'){
        thread_M = thread_id_M++;
        //Call function to create the Customer queued
        Customers = createCustomerQueue(N,t,thread_M);
        threadID = t+std::to_string(thread_M);
    }
     if(t == 'L'){
        thread_L = thread_id_L++;
        //Call function to create the Customer queue
        Customers = createCustomerQueue(N,t,thread_L);
        threadID = t+std::to_string(thread_L);
    }
    if(t == 'H'){
        //Call function to create the Customer queue
        Customers = createCustomerQueue(N,t,1);
        threadID = t+std::to_string(1);
    }
    
    
    //Sync 10 threads before continuing
    sync_threads.arrive_and_wait();

    //Loop until time is up
    while((timeElapsed < 60 || curentCustomers !=0) && (hSellers.size() + mSellers.size() + lSellers.size()) != 100){

        //Sync 10 threads and print time before continuing
        printTime.arrive_and_wait();

        pthread_mutex_lock(&mutex);
        printArrivals(threadID,Customers,timeElapsed);
        pthread_mutex_unlock(&mutex);
        
        if(!Customers.empty() && Customers.top().arrivalTime <= timeElapsed && !Customers.top().helped && timeElapsed < 60){
            pthread_mutex_lock(&mutex);
            //Increment number of customers that the current thread has served
            customer++;
            bool found = false;
            switch(t){
                case 'H':{
                    std::string name = threadID + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                    for(int row = 0; row < 10 && !found; row++){
                        for(int col = 0; col < 10; col++){
                            if(seats[row][col].empty()){
                                seatFound(row, col, name, found, Customers);
                                break;
                            }
                        }
                    }
                    break;
                }case 'M':{
                    std::string name = threadID + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                    for(int i=1; i<10 && !found; i++){
                        int row= 5 + i/2 * pow(-1,i);
                        for(int col = 0; col<10; col++){
                            if(seats[row][col].empty()){
                                seatFound(row, col, name, found, Customers);
                                break;
                            }   
                        }
                    }
                    break;
                }case 'L':{
                    std::string name = threadID + (customer < 10 ? "0"+ std::to_string(customer) : std::to_string(customer));
                    for(int row = 9; row >= 0 && !found; row--){
                        for(int col = 9; col >= 0; col--){
                            if(seats[row][col].empty()){
                                seatFound(row, col, name, found, Customers);
                                break;
                            }
                        }
                    }
                    break;
                }
            }//end switch
            if(!found){std::cout<<threadID<<" Sorry customer "<<customer<<" all seats are full."<<std::endl<<"\t";}
            pthread_mutex_unlock(&mutex);
        }//end if

        if(!Customers.empty() && Customers.top().helped){
            pthread_mutex_lock(&mutex);
            if(Customers.top().processingTime > 0){
                Customer temp = Customers.top();
                Customers.pop();
                temp.processingTime --;
                temp.arrivalTime = 0; //Artificially decrease arrival time to keep entry at top of pq
                Customers.push(temp);
            }else{
                std::cout<<threadID<<" Finished ticket purchase #"<<customer<<std::endl<<"\t";
                curentCustomers--;
                Customer temp = Customers.top();
                temp.completionTime = timeElapsed;
                Customers.pop();
                if(t=='H'){
                    hSellers.push_back(temp);
                }else if(t=='M'){
                    mSellers.push_back(temp);
                }else{
                    lSellers.push_back(temp);
                }
            }
            pthread_mutex_unlock(&mutex);
        }

        //Debugging statements BELOW

        // pthread_mutex_lock(&mutex);
        // std::cout<<"Current Customers are "<<curentCustomers<<" Helping value is "<<Customers.top().helped<<
        // "Processing time remaining is "<<Customers.top().processingTime<<" Size is this: "<<Customers.size()<<std::endl;
        // pthread_mutex_unlock(&mutex);

        //Debugging statements ABOVE

        //Sync 10 threads and print sellers before continuing
        printSellers.arrive_and_wait();

        //Sync 10 threads and increment time before continuing
        increment.arrive_and_wait();    

    }//end while

    pthread_mutex_lock(&mutex);
    printunservedCustomers(threadID,Customers);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    if(!Customers.empty()){   
        unservedcustomers = unservedcustomers + Customers.size();
    }
    pthread_mutex_unlock(&mutex);

    //Sync 10 threads and print seats before continuing
    printResultsTable.arrive_and_wait();

    printMetrics.arrive_and_wait();

    return NULL;
}