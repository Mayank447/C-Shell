#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include "shell.h"
#include "color.h"
#include "helper_functions.h"
#include "raw_mode.h"

#define BUFFER_LENGTH 1024

void printPID(){
    char path[100] = "/proc/loadavg";
    int file = open(path, O_RDONLY);
    if (file == -1){
        sprintf(error_buffer, "Neonate Error: %s: %s\n", path, strerror(errno));
        print_error(error_buffer);
        return;
    }

    char buffer[BUFFER_LENGTH];
    int bytes_read = read(file, buffer, BUFFER_LENGTH);
    if(bytes_read == -1) perror("STAT_FILE ERROR:");
    close(file);

    pid_t pid;
    char a[32], b[32], c[32], d[32];
    sscanf(buffer, "%s %s %s %s %d", a, b, c, d, &pid);
    printf("%d\n", pid);
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
            // printf("Neonate\n");
            printPID();
        }
        exit(0);
    }

    else{
        char c;
        enableRawMode();
        process_buffer[process_count-1].pid = pid;
        while(1){
            if((c=getchar())=='x' || c!=EOF) {
                kill(pid, SIGKILL);
                break;
            }
        }
        disableRawMode();
    }
}