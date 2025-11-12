#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include <errno.h>

#define BUFFER_SIZE 128
#define NUM_PROC 5
#define TIMEOPEN 30

//  Struct used to hold pipe and child process details
struct pipeConnection{
    int read_fd;
    int write_fd;
    pid_t pid;
};

//  Struct used to calculate time info of Process
struct timeResult{
    char formatted[16];
    double alive;
};

/*
Function will calculate the current time and use the start time passed in
to calculate two different quantitieis:

    time alive: the elapsed time the child process has been running for
    formatted time: a formatted string that tracks the time a process has been alive for written in the formatin 00:00.000
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
    //  If msec is negative, borrow a second to increase msec.
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

    //  Step 1 - Initialize 5 pipes and save them to the struct array
    for(int i=0; i<NUM_PROC; i++){
        int fd[2];
        //  pipe(fd) creates the pipe and file descriptors
        if(pipe(fd) == -1){
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
        //  save the file descriptors to the struct
        pipes[i].read_fd = fd[0];
        pipes[i].write_fd = fd[1];
    }

    //  Get start time of simulation
    struct timeval startTime;
    gettimeofday(&startTime, NULL);

    //  Print parent is creating children
    std::cout<<"Parent: "<<getpid()<<" opening communication channels for "<<TIMEOPEN<<" seconds."<<std::endl;

    //  Step 2 - Fork 5 child processes - This is the main section of the code where child processes write to their pipes
    for(int i=0; i<NUM_PROC; i++){
        pids[i] = fork();

        if(pids[i] < 0){
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        //  Child Process runs here
        if(pids[i] == 0){
            //  Save process id to the struct
            pipes[i].pid = getpid();

            //  Close file pointer to output file since children don't need to access it
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

            /*
            
                This while loop runs each child process for 30 seconds.  Depending on the type of the child, it will either
                read input from stdin and send this to the parent or it will send a simple message to the parent.
            
            */
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

                //  Check if child process has been alive for 30 seconds and if it has, kill it.
                if(timeAlive > TIMEOPEN){
                    break;
                }

                //  Message buffer for child process
                char msg[BUFFER_SIZE];

                //  Process 0-3 sends simple message
                if(i != 4){
                    //  Print the formatted message to the msg buffer
                    snprintf(msg, sizeof(msg), "%s: Child %d: message #%d\n",formattedTime, i, messageNum);

                    //  Write the msg buffer to the write end of the pipe
                    if(write(write_fd, msg, strlen(msg)) == -1){
                        perror("Failed to write to pipe");
                        break;
                    }
                    messageNum++;
                }

                //  Process 4 takes user input
                if(i == 4){
                /*
                    To fix EIO occuring when a background child process attempts to read 
                    from terminal we need to give the child access to the controlling terminal.

                    This was implemented due to an initial bug when an EIO error was thrown when
                    the child tried to access terminal.  Further testing indicates that the error
                    may have occured due to the parent exiting prematurely.  Since fopen("/dev/tty", "r")
                    is not harmful, leaving it in.
                */
                    FILE *tty_fd = fopen("/dev/tty", "r");
                    if(tty_fd){
                        //  Buffer to store the user input from stdin
                        char userInput[BUFFER_SIZE-56];

                        //  Prompt user to enter a string
                        std::cout<<"Child 4: Please enter a string and press Enter: ";

                        //  If the input is successfully retrieved from terminal:
                        if(fgets(userInput, sizeof(userInput), tty_fd)){
                            //  Change newline char to null terminator to avoid formatting issues in buffer.
                            size_t length = strlen(userInput);
                            if(length > 0 && userInput[length-1] == '\n'){
                                userInput[length-1] = '\0';
                            }
                            //  Print the formatted message to the msg buffer
                            snprintf(msg, sizeof(msg), "%s: Child %d: %s\n",formattedTime, i, userInput);

                                //  Write the msg buffer to the write end of the pipe 
                                if(write(write_fd, msg, strlen(msg)) == -1){
                                    perror("Failed to write to pipe");
                                    break;
                                }
                                messageNum++;
                        }
                        //  Error handling if reading from terminal failed
                        else{
                            perror("Fgets failed to read data from /dev/tty");
                            if(tty_fd) fclose(tty_fd);
                            break;
                        }
                    //  Error handling if opening /dev/tty failed
                    }else{
                        perror("Failed to open /dev/tty properly");
                        exit(EXIT_FAILURE);
                    }
                }//  End of i==4 loop

            }//  End of while loop

            //  Child process finished running. Close fd and exit
            close(write_fd);
            exit(0);
        }
        //  Parent Process will close write end of pipe
        else{    
            pipes[i].pid = getpid();
            close(pipes[i].write_fd);
        }
    }   //  End simulation for loop


    //  Step 3 - Parents reads data from the pipes

    //  Set of file descriptors used in the select() call
    fd_set read_fd, temp_read_fd;
    FD_ZERO(&read_fd);
    int largest_fd = 0;

    //  Track how many pipes are still open for main while loop
    int open_pipes = NUM_PROC;

    //  Structs to handle time values
    struct timeval timeout;
    timeResult timeRead;

    //  Add every read pipe the parent has to the file descriptor set.
    for(int i=0; i< NUM_PROC; i++){
        FD_SET(pipes[i].read_fd, &read_fd);
        //  Track largest fd value.  Required by the select() call
        if(pipes[i].read_fd > largest_fd){
            largest_fd = pipes[i].read_fd;
        }
    }

    //  While there are still pipes open (child's write end)
    while(open_pipes > 0){
        //  Make a copy of the set because select() will edit it
        temp_read_fd = read_fd;

        //  Timeout value of 2.5 seconds
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;

        //  Call select() to determine if any pipes have data on them ready to be read
        int fd = select(largest_fd + 1, &temp_read_fd, NULL, NULL, &timeout);

        //  Error handling: timeout
        if(fd == 0){
            std::cout<<"Timeout has occured.  Skipping this iteration"<<std::endl;
            continue;
        }

        //  Error handling: select() call failed
        if(fd == -1){
            perror("Error with select system call: ");
            exit(EXIT_FAILURE);
        }

        //  Select succeeded:  Search all pipes to determine which one is ready
        for(int i=0; i<NUM_PROC; i++){
            //  If this file descriptor is still in the set it has data to read
            if(FD_ISSET(pipes[i].read_fd, &temp_read_fd)){

                //  Clear buffer before reading as a saftey measure
                memset(read_msg, 0, sizeof(read_msg));

                //  Get current time (used by parent)
                timeRead = getTimeValues(startTime);

                //  Read message from the pipe
                ssize_t numbytes = read(pipes[i].read_fd,read_msg,sizeof(read_msg));
                

                if(numbytes > 0){
                    //  Process buffer line by line to handle multiple messages
                    char *line_start = read_msg;
                    char *line_end;

                    while((line_end = strchr(line_start, '\n')) != NULL){
                        //  Null-terminate this line
                        *line_end = '\0';;

                        //  Skip empty lines
                        if(strlen(line_start) > 0){
                            //  Buffer to hold the timestamped message
                            char readmsg[BUFFER_SIZE+32];
            
                            //  Prepend parent's timestamp to this message
                            snprintf(readmsg, sizeof(readmsg), "%s: %s",timeRead.formatted, line_start);
            
                            //  Output the message to the file output.txt
                            fprintf(output,"%s\n",readmsg);
                        }
                        //  Move to the next line
                        line_start = line_end + 1;
                    }

                    //  Handle any remaining data without newline (partial message)
                    if(strlen(line_start) > 0){
                        char readmsg[BUFFER_SIZE+32];
                        snprintf(readmsg, sizeof(readmsg), "%s: %s",timeRead.formatted, line_start);
                        fprintf(output,"%s\n",readmsg);
                    }

                //  Write end of pipe was closed and there was no data left to read, so remove it from the file descriptor set.
                }else if(numbytes == 0){
                    FD_CLR(pipes[i].read_fd, &read_fd);
                    open_pipes--;
                }else{
                    perror("Error when reading from pipe");
                }
            }
        }
    }

    /*------------------------------
                
                Clean up       

    ------------------------------*/

    //  Step 4 - Close remaining file descriptors
    for(int i=0; i<NUM_PROC; i++){
        close(pipes[i].read_fd);
    }

    //  Step 5 - Close output file descriptor
    if(fclose(output) == EOF){
        perror("Error closing file");
        exit(EXIT_FAILURE);
    } 

    //  Make parent wait for all other children to exit before parent exits. (added for saftey)
    for(int i=0; i<NUM_PROC; i++){
        waitpid(pids[i],NULL,0);
    }

    return 1;
}
