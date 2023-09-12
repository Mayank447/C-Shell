#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "shell.h"

void execute_command(char command_string[][MAX_ARGUMENT_LENGTH], int argument, int is_background)
{
    // Get all the arguments for the command
    char* arguments[MAX_ARGUMENTS];
    for(int i=0; i<argument; i++){
        arguments[i] = command_string[i];
    }
    arguments[argument]=NULL;

    pid_t pid = fork();
    int status;

    // Child process
    if(pid<0) perror("Fork failed:");
    
    else if(pid==0){
        execvp(command_string[0], arguments);
        perror("Invalid command:");
        exit(1);
    }

    else{
        if(is_background) printf("%d\n", pid);
        else{
            waitpid(pid, &status, 0);
        }
    }
}