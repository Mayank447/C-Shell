#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "../include/shell.h"
#include "../include/color.h"

void signal_handler(char command_string[][MAX_ARGUMENT_LENGTH], int arguments)
{
    if(arguments!=3){
        print_error("Invalid no. of arguments given to ping\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = atoi(command_string[1]);
    int signal_number = atoi(command_string[2]) % 32;
    printf("%d %d\n", pid, signal_number);
    
    // Checking if the signal number is valid
    if (kill(pid, 0) == 0 || errno != ESRCH) {
        if (kill(pid, signal_number) == 0) {
            printf("Sent signal %d to process with pid %d.\n", signal_number, pid);
        } 
        else {
            perror("Error sending signal:");
            exit(EXIT_FAILURE);
        }
    }

    else{
        perror("Process doesn't exist:");
    }
}