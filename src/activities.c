#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/shell.h"

int compareByPID(const void *a, const void *b) {
    const struct Process *pid1 = (const struct Process *)a;
    const struct Process *pid2 = (const struct Process *)b;
    return pid1->pid - pid2->pid;
}

void activities()
{
    qsort(process_buffer, process_count, sizeof(struct Process), compareByPID);
    for(int i=0; i < process_count; i++){
        if(strcmp(process_buffer[i].status, "R")==0) 
            printf("%d : %s - Running\n", process_buffer[i].pid, process_buffer[i].entire_command);
        else if(strcmp(process_buffer[i].status, "S")==0)
            printf("%d : %s - Stopped\n", process_buffer[i].pid, process_buffer[i].entire_command);
    }
}