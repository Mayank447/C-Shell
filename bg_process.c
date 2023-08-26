#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "shell.h"

#define MAX_JOBS 10
#define MAX_ARGUMENTS 16

struct Job {
    pid_t pid;
    char command[1000];
    char status;
    int is_background;
    int virtual_memory;
    char path_of_process[4000];
};

int num_jobs = 0;

void print_proclore(struct Job* job){
    // Printing the job details in the required format
    printf("PID : %d\n", job->pid);
    printf("Process Status : %c%c\n", job->status, (job->is_background==1) ? '+' : ' ');
    printf("Process Group: %d", getpgid(job->pid));
    printf("Virtual Memory : %d\n", job->virtual_memory);
    printf("Command : %s\n", job->path_of_process);
}

void proclore(char* argument){
    int pid = atoi(argument);
}

void execute_command(char** command_string, int argument, int is_background){

    // Get all the arguments for the command
    char* arguments[MAX_ARGUMENTS];
    for(int i=0; i<argument; i++){
        arguments[i] = command_string[i];
    }
    arguments[argument]=NULL;

    pid_t pid = fork();
    int status;

    // Child process
    if(pid==0){
        execvp(command_string[0], arguments);
        perror("Invalid command\n");
    }

    else{
        if(is_background) printf("%d\n", pid);
        else{
            waitpid(pid, &status, 0);
        }
    }
}