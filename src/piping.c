#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/shell.h"
#include "../include/input_handling.h"
#include "../include/helper_functions.h"
#include "../include/color.h"
#include "../include/bg_process.h"

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
        if(strlen(Commands[i])==0) {
            print_error("Invalid use of pipes\n");
            return;
        }
    }

    for (int i=0; i<num_commands; i++)
    {
        if(pipe(pipe_fds)==-1){
            perror("Pipe error : ");
            return;
        }
        pid = fork();
        int initial_time = time(NULL);

        if (pid == -1) {
            perror("fork");
            return;
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

            char command[MAX_COMMAND_LENGTH];
            getCommandfromString(Commands[i], command);

            int time_taken = time(NULL) - initial_time;
            if(time_taken > 2){
                sprintf(process_time, ":%s : %d", command, time_taken);
            }
        }
    }

    dup2(saved_STDOUT, STDOUT_FILENO);
    dup2(saved_STDIN, STDIN_FILENO);
}