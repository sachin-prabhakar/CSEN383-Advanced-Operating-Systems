#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <limits>

#define BUFFER_SIZE 64
#define NUM_PROC 5
#define TIMEOPEN 30

struct pipeConnection{
    int read_fd;
    int write_fd;
    pid_t pid;
};

struct timeResult{
    char formatted[16];
    double alive;
};

/*

Function will calculate the current time and use the start time passed in
to calculate two different quantitieis:

    time alive: the elapsed time the child process has been running for
    formatted time: a formatted string that tracks time process has been alive for
                    written in the formatin 00:00.000

*/
timeResult getTimeValues(struct timeval start){

    //  Struct to return both timeAlive and formattedTime
    timeResult temp;

    //  Get the current time
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    //  Calculate time alive as currene time - start time
    double timeAlive = (currentTime.tv_sec - start.tv_sec) + (currentTime.tv_usec - start.tv_usec) / 1000000.0;

    //  Format time to be mm:ss.msec
    int sec = currentTime.tv_sec - start.tv_sec;
    int msec = currentTime.tv_usec - start.tv_usec;
    if(msec < 0){
        sec --;
        msec += 1000000;
    }
    msec = msec/1000;

    //  Save the formatted time to the char object in struct temp
    snprintf(temp.formatted, sizeof(temp.formatted), "00:%02d.%03d",sec,msec);

    //  Save time alive value
    temp.alive = timeAlive;

    return temp;
}

int main(){

    //  Buffer for parent process to read data
    char read_msg[BUFFER_SIZE];

    //  Pipes and processes
    pipeConnection pipes[NUM_PROC];
    pid_t pids[NUM_PROC];

    //  File for final output by parent process
    FILE *output = fopen("output.txt", "w");
    if(output == NULL){
        perror("File creatiion failed");
        exit(EXIT_FAILURE);
    }

    //  Step 1 - Initialize 5 pipes
    for(int i=0; i<NUM_PROC; i++){
        int fd[2];
        if(pipe(fd) == -1){
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
        pipes[i].read_fd = fd[0];
        pipes[i].write_fd = fd[1];
    }

    //  Get start time of simulation
    struct timeval startTime;
    gettimeofday(&startTime, NULL);

    //  Print paremt is creating children
    std::cout<<"Parent: "<<getpid()<<" opening communication channels for "<<TIMEOPEN<<" seconds."<<std::endl;

    //  Step 2 - Fork 5 child processes
    for(int i=0; i<NUM_PROC; i++){
        pids[i] = fork();

        if(pids[i] < 0){
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        //  Child Process
        if(pids[i] == 0){
            pipes[i].pid = getpid();

            //  Close output file pointer since children don't need to access it
            fclose(output);

            //  Close all other available pipes except this child's write pipe
            for(int j=0; j<NUM_PROC; j++){
                close(pipes[j].read_fd);
                if(j != i){
                    close(pipes[j].write_fd);
                }
            }

            //  Initialize variables used to track writes to the pipe
            int write_fd = pipes[i].write_fd;
            int messageNum = 1;

            //  Endless loop will exit after 30 seconds
            while(1){
                //  Call function to get timeAlive and formattedTime for child process
                timeResult time = getTimeValues(startTime);
                double timeAlive = time.alive;
                char *formattedTime = time.formatted;

                //  Calculate sleep time for child processes 0-3
                if(i != 4){
                    int sleepTime = rand() % 3;
                    usleep(sleepTime * 1000000);
                }

                //  Check if child process has been alive for 30 seconds
                if(timeAlive > TIMEOPEN){
                    break;
                }

                //  Message buffer for child process
                char msg[BUFFER_SIZE];

                //  Process sends simple message
                if(i != 4){
                    snprintf(msg, sizeof(msg), "%s: Child %d message #%d",formattedTime, i, messageNum);  
                /*------------------------------------------------------------------------

                    //  ------------- Uncomment when reading is impmeneted ------------- 
                    if(write(write_fd, msg, strlen(msg)) == -1){
                        perror("Failed to write to pipe");
                        break;
                    }

                ------------------------------------------------------------------------*/
                }

                //  Process takes user input
                if(i == 4){
                /*
                    To fix EIO occuring when a background child process attempts to read 
                    from terminal we need to give the child access to the main terminal.
                */
                    FILE *tty_fd = fopen("/dev/tty", "r");
                    if(tty_fd){
                        char userInput[BUFFER_SIZE-40];
                        std::cout<<"Child 5: Please enter a string and press Enter: ";
                        if(fgets(userInput, sizeof(userInput), tty_fd)){
                            //  Change newline char to null terminator to avoid formatting issues in buffer.
                            size_t length = strlen(userInput);
                            if(length > 0 && userInput[length-1] == '\n'){
                                userInput[length-1] = '\0';
                            }
                            snprintf(msg, sizeof(msg), "%s: Child %d:  %s",formattedTime, i, userInput);
                             /*------------------------------------------------------------------------

                                //  ------------- Uncomment when reading is impmeneted ------------- 
                                if(write(write_fd, msg, strlen(msg)) == -1){
                                    perror("Failed to write to pipe");
                                    break;
                                }

                            ------------------------------------------------------------------------*/   
                        }else{
                            perror("Fgets failed to read data from /dev/tty");
                            if(tty_fd) fclose(tty_fd);
                            break;
                        }
                    }else{
                        perror("Failed to open /dev/tty properly");
                    }
                    //  Sleep for .5 seconds
                    usleep(500000);
                }   //  End of i==4 loop

            }   //  End of while loop
            //  Child process finished running. Close fd and exit
            close(write_fd);
            exit(0);
        }//  Parent
        else{    
            pipes[i].pid = getpid();
            close(pipes[i].write_fd);
        }
    }   //  End simulation for loop


    //  Step 3 - Parents reads data from the pipes

    /*------------------------------------
    
            NEEDS TO BE IMPLEMENTED
            
            MUST USE SELECT()
    
    ------------------------------------*/


    

    //  Step 4 - Close remaining file descriptors
    for(int i=0; i<NUM_PROC; i++){
        close(pipes[i].read_fd);
    }

    //  Step 5 - Close output file descriptor
    if(fclose(output) == EOF){
        perror("Error closing file");
        exit(EXIT_FAILURE);
    } 

    for(int i=0; i<NUM_PROC; i++){
        waitpid(pids[i],NULL,0);
    }

    return 1;

}