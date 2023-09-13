#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void activities()
{
    for(int i=0; i < process_count; i++){
        if(strcmp(process_buffer[i].status, "Z")!=0) 
            printf("%d : %s - Running\n", process_buffer[i].pid, process_buffer[i].entire_command);
        else
            printf("%d : %s - Stopped\n", process_buffer[i].pid, process_buffer[i].entire_command);
    }
}