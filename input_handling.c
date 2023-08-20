#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path_handling.h"

#define MAX_COMMANDS 100
#define MAX_COMMAND_LENGTH 100

void processInput(char* input);
const char sep[] = ";\n\r\v\f";


/* Function to convert the input to lowercase */
char* lowercase(char* input){
    int i=0;
    while(input[i]!='\0'){
        if(input[i]>='A' && input[i]<='Z'){
            input[i] = input[i] + 32;
        }
        i++;
    }
    return input;
}


/* Function to remove leading spcae and replace multiple or tab spaces with a single space */
char* removeLeadingSpaces(char* input){
    int i=0, j=0;
    while(input[i]==' ' || input[i]=='\t'){
        i++;
    }
    while(input[i]!='\0'){
        input[j++] = input[i++];
        if(input[i]==' ' || input[i]=='\t'){
            while(input[i]==' ' || input[i]=='\t'){
                i++;
            }
            input[j++] = ' ';
        }
    }
    input[j] = '\0';
    return input;
}


/* Function to Tokenize the input */
char** tokenizeInput(char* input)
{
    char** Commands = (char**)malloc(sizeof(char*)*MAX_COMMANDS);
    char* token = strtok(input, sep);
    int i=0;

    while (token!=NULL){
        Commands[i] = removeLeadingSpaces(lowercase(token));
        processInput(Commands[i++]);
        token = strtok(NULL, sep);
    }
    return Commands;
}


/* Function to handle input i.e. identify the command and arguments */
void processInput(char* input)
{
    char* command;
    char** command_string = (char**)malloc(sizeof(char*)*MAX_COMMAND_LENGTH);

    int i=0;
    while  ((command = strsep(&input, " ")) != NULL){
        command_string[i++] = command;
    }

    if(strcmp(command_string[0], "exit")==0){
        exit(0); // Closing the shell if the user typed exit
    }

    else if(strcmp(command_string[0], "warp")==0){
        changeDirectory(command_string, i);
    }

    else{
        printf("%s is an Invalid Command\n", command_string[0]);
    }
}