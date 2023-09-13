#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "shell.h"
#include "color.h"

void iman(char command_string[][MAX_ARGUMENT_LENGTH], int n_arguments){
    if(n_arguments==1){
        print_error("iMan: missing argument\n");
        return;
    }

    else if(n_arguments>2){
        print_error("iMan: too many arguments\n");
    }

    
}