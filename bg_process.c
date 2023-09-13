#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "shell.h"

void store_process(){
    // Storing the process in the process buffer
    process_buffer[process_count].already_exitted = 0;
    process_buffer[process_count].time_initialized = time(NULL);
    strcpy(process_buffer[process_count].status, "R");
}


void bg_process_finished()
{
    int status, result;
    for(int i=0; i<process_count; i++)
    {
        result = waitpid(process_buffer[i].pid, &status, WNOHANG);
        
        //if(result<0) perror("Waitpid failed:");

        if(result==0) continue; //Currently running
        
        else if(process_buffer[i].already_exitted==0)
        {
            process_buffer[i].already_exitted = 1;
            strcpy(process_buffer[i].status, "Z");
            
            if(process_buffer[i].bg==0) {
                int time_taken = time(NULL) - process_buffer[i].time_initialized;
                if(time_taken > 2){
                    sprintf(process_time, ":%s : %d", process_buffer[i].command, time_taken);
                }
            }

            else if(process_buffer[i].bg==1)
            {   
                if(WIFEXITED(status)) 
                    printf("%s exited normally (%d)\n", process_buffer[i].command, process_buffer[i].pid);

                else if(WIFSIGNALED(status))
                    printf("%s with PID (%d) was terminated by a signal (%d)\n", process_buffer[i].command, process_buffer[i].pid, WTERMSIG(status));
            
                else
                    printf("%s exited abnormally (%d)\n", process_buffer[i].command, process_buffer[i].pid);
            }
        }
    }
}


void execute_command(char command_string[][MAX_ARGUMENT_LENGTH], int argument, int is_background)
{
    // Get all the arguments for the command
    char* arguments[MAX_ARGUMENTS];
    for(int i=0; i<argument; i++){
        arguments[i] = command_string[i];
    }
    arguments[argument]=NULL;

    pid_t pid = fork();
    process_buffer[process_count++].pid = pid;
    int status;

    // Child process
    if(pid<0) perror("Fork failed:");
    
    else if(pid==0){
        execvp(command_string[0], arguments);
        perror("Invalid command:");
        exit(1);
    }

    else{
        if(is_background) {
            process_buffer[process_count-1].bg = 1;
            printf("%d\n", pid);
        }
        else{
            waitpid(pid, &status, 0);
        }
    }
}