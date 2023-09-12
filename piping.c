#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shell.h"
#include "input_handling.h"
#include "helper_functions.h"

void pipeInputString(char* input){
    int pipe_fds[2];
    pid_t pid;
    int status;
    int prev_pipe = STDIN_FILENO;

    int num_commands;
    char Commands[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    characterParser(Commands, input, &num_commands, '|');

    for (int i=0; i<num_commands+1; i++){
        removeLeadingSpaces(Commands[i]);
    }

    for (int i = 0; i < num_commands+1; i++) {
        pipe(pipe_fds);
        pid = fork();

        if(pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        
        else if(pid == 0) { // Child process
            
            // Redirect output to the write end of the pipe
            // dup2(pipe_fds[1], STDOUT_FILENO);
            // close(pipe_fds[0]);

            // if (i > 0) {
            //     // Redirect input from the read end of the previous pipe
            //     dup2(prev_pipe, STDIN_FILENO);
            // }

            processInput(Commands[i]);
            exit(EXIT_SUCCESS);
        } 
        
        else { // Parent process
            
            waitpid(pid, &status, 0);
            // close(prev_pipe);

            // Close the write end of the pipe, and set it as the previous pipe
            // close(pipe_fds[1]);
            // prev_pipe = pipe_fds[0];
        }
    }
}