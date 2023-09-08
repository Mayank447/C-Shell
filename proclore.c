#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#include "shell.h"
#include "path_handling.h"

#define ERROR_BUFFER 1024
#define BUFFER_LENGTH 10000

void print_proclore(pid_t pid, char* process_status, pid_t group_pid,
                    unsigned long int virtual_memory_size, char* relative_exec_path){
    // Printing the job details in the required format
    printf("pid : %d\n", pid);
    printf("Process Status : %s\n", process_status);
    printf("Process Group : %d\n", group_pid);
    printf("Virtual Memory : %lu\n", virtual_memory_size);
    printf("Executable Path : %s\n", relative_exec_path);
}

void getProcessFiles(int pid){
    char stat_path[100];
    char exe_path[BUFFER_LENGTH];
    char exe_buffer[BUFFER_LENGTH];

    sprintf(stat_path, "/proc/%d/stat", pid);
    sprintf(exe_path, "/proc/%d/exe", pid);

    ssize_t len = readlink(exe_path, exe_buffer, sizeof(exe_buffer)-1);
    if(len!=-1) exe_buffer[len] = '\0';
    else{
        fprintf(stderr, "ERROR: %s", strerror(errno));
        return;
    }

    char stat_buffer[BUFFER_LENGTH];
    int stat_file = open(stat_path, O_RDONLY);
    read(stat_file, stat_buffer, BUFFER_LENGTH);
    if (stat_file == -1){
        fprintf(stderr, "proclore: %s: %s\n",
                stat_path, strerror(errno));
        return;
    }

    pid_t ppid;                            //* pid of parent process
    pid_t pgrp;                            //* process group id of process
    pid_t tpgid;                           //*The ID of the foreground process group of the controlling terminal of the process.
    char state;                            //* state of the process
    char buffer_stat[BUFFER_LENGTH];    //* buffer for stat stuff
    int session;                           //* session id of process
    int tty_nr;                            //* controlling terminal of process
    unsigned long int virtual_memory_size; //* virtual memory size in bytes

    sscanf(stat_buffer, "%d %s %c %d %d %d %d %d", &pid, buffer_stat, &state,
           &ppid, &pgrp, &session, &tty_nr, &tpgid);

    char process_state[3];
    process_state[0] = state;
    process_state[1] = '\0';
    if (pid == tpgid){
        strcat(process_state, "+\0");
    }

    close(stat_file);
    char *relative_process_exec_path = relativePath(exe_path);

    // Virtual memory
    char process_statm_path[BUFFER_LENGTH];
    char buffer_statm[BUFFER_LENGTH];
    sprintf(process_statm_path, "/proc/%d/statm", pid);
    int fd_process_statm = open(process_statm_path, O_RDONLY); //* for virtual memory size
    if (fd_process_statm == -1){
        fprintf(stderr, "proclore: %s: %s\n",
                process_statm_path, strerror(errno));
        return;
    }

    read(fd_process_statm, buffer_statm, BUFFER_LENGTH);
    sscanf(buffer_statm, "%lu", &virtual_memory_size);
    close(fd_process_statm);

    print_proclore(pid, process_state, pgrp, virtual_memory_size, relative_process_exec_path);
}

void proclore(char** input, int arguments)
{
    pid_t pid;

    if(arguments==1) pid = getpid();
    else if(arguments ==2) pid = atoi(input[1]);
    else {
        custom_error("Too many argunents");
        return;
    }

    int exists = kill(pid, 0);
    if(exists!=0){
        if(errno == ESRCH){
            char error_string[ERROR_BUFFER];
            sprintf(error_string, "ERROR: process with PID %d does not exits\n", pid);
            custom_error(error_string);
        }
        else{
            perror("ERROR");
        }
        return;
    }

    getProcessFiles(pid);
}