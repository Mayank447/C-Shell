#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "shell.h"

void extractFilename(char* input, char file_name[], int i){
    int j=1;
    if(input[i+1]==' ' || input[i+1]=='\t') j=2; // Space after '>'
    int k=j;

    while(input[i+k]!='\0' && (input[i+k]!=' ' && input[i+k]!='\t')) k++;
    input[i+k] = '\0';
    
    if(input[i+j]=='~'){
        strcpy(file_name, home_directory);
        strcat(file_name,input + i + j + 1);
    }

    else if(input[i+j]=='-'){
        strcpy(file_name, previous_directory);
        strcat(file_name,input + i + j + 1);
    }
    
    else {
        strcpy(file_name,input + i + j);
    }
}


void input_redirection(char* input){
    // Split the input string into two parts based on > >> < 
    int single_quotes = 0;
    int double_quotes = 0;
    int l = strlen(input);
    
    if(input[l-1]=='>' || input[l-1]=='<'){
        fprintf(stderr, "Filename not specified\n");
        exit(1);
    }

    for(int i=0; i<l; i++){
        
        if(input[i]=='\'')  single_quotes = (single_quotes+1)%2;

        else if(input[i]=='\"') double_quotes = (double_quotes+1)%2;

        else if(input[i]=='>' && input[i+1]=='>' && !single_quotes && !double_quotes){ //>> append
            input[i] = '\0';
            input[i+1] = '\0';
            char file_name[MAX_FILE_NAME_LENGTH];
            extractFilename(input, file_name, i+1);

            int fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
            if(dup2(fd, STDOUT_FILENO) < 0) {
                fprintf(stderr, "Invalid filename: %s", file_name);
                exit(1);
            }
            close(fd);
        }

        else if(input[i]=='>' && !single_quotes && !double_quotes){
            input[i] = '\0';
            char file_name[MAX_FILE_NAME_LENGTH];
            extractFilename(input, file_name, i);
            int fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if(dup2(fd, STDOUT_FILENO) < 0) {
                fprintf(stderr, "Invalid filename: %s", file_name);
                exit(1);
            }
            close(fd);
        }

        else if(input[i]=='<' && !single_quotes && !double_quotes){
            input[i] = '\0';
            char file_name[MAX_FILE_NAME_LENGTH];
            extractFilename(input, file_name, i);;
            
            int fd = open(file_name, O_RDONLY, 0644);
            if(fd<0){
                fprintf(stderr, "No such input file found!\n");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
        }
    }
}