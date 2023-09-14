#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "shell.h"
#include "color.h"
#include "helper_functions.h"
#include "raw_mode.h"

void printPID(){

}

void neonate(char commands[][MAX_ARGUMENT_LENGTH], int arguments)
{
    if(arguments!=3){
        print_error("ERROR: neonate requires exactly three argument\n");
        return;
    }

    else if(strcmp(commands[1], "-n")!=0){
        print_error("ERROR: Invalid flag\n");
        return;
    }

    if(!isNumber(commands[2])){
        print_error("ERROR: Invalid time step\n");
        return;
    }
    long time_step = atoi(commands[2]);

    pid_t pid = fork();

    if(pid<0){
        print_error("ERROR: Forking failed\n");
        return;
    }

    else if(pid==0){
        while(1) {
            sleep(time_step);
            printf("Neonate\n");
            //printPID();
        }
        exit(0);
    }

    else{
        char c;
        enableRawMode();
        while(1){
            if((c=getchar())=='x' || c!=EOF) {
                kill(pid, SIGKILL);
                break;
            }
        }
        disableRawMode();
    }
}