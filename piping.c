#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shell.h"
#include "input_handling.h"
#include "helper_functions.h"
#include "color.h"

void pipeInputString(char* input){
    int pipe_fds[2];
    pid_t pid;
    int status;
    int input_fd = STDIN_FILENO;

    int num_commands = 1;
    char Commands[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    characterParser(Commands, input, &num_commands, '|');

    for (int i=0; i<num_commands; i++){
        removeLeadingSpaces(Commands[i]);
        if(strcmp(Commands[i], "\0")==0) {
            print_error("Invalid use of pipes\n");
            return;
        }
    }

    for (int i=0; i<num_commands; i++)
    {
        pipe(pipe_fds);
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        
        else if (pid == 0) {
            // Child process
            close(pipe_fds[0]); // Close the read end of the pipe
            dup2(input_fd, STDIN_FILENO); // Redirect input
            
            if (i != num_commands - 1) {
                dup2(pipe_fds[1], STDOUT_FILENO); // Redirect output to the write end of the pipe
            }
            close(pipe_fds[1]); // Close the write end of the pipe

            processInput(Commands[i]); // Execute the command
            exit(EXIT_SUCCESS);
        } 
        
        else {
            // Parent process
            close(pipe_fds[1]); // Close the write end of the pipe
            waitpid(pid, &status, 0);
            // Set the input for the next command to be the read end of the pipe
            input_fd = pipe_fds[0];
        }
    }

    dup2(saved_STDOUT, STDOUT_FILENO);
    dup2(saved_STDIN, STDIN_FILENO);
}