#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path_handling.h"
#include "ls.h"
#include "history.h"
#include "shell.h"
#include "find.h"

#define MAX_COMMANDS 100
#define MAX_COMMAND_LENGTH 100


const char sep[] = ";\n\r\v\f";
void categorize_fg_bg_process(char* input);
void processInput(char* input);


/* Function to convert the input to lowercase */
// TO DO: Lowercase only the first command
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
            if(input[i]=='\0') break;
            input[j++] = ' ';
        }
    }
    input[j] = '\0';
    return input;
}


/* Function to Tokenize the input */
void tokenizeInput(char* input)
{
    char* input_copy = strdup(input);
    char** Commands = (char**)malloc(sizeof(char*)*MAX_COMMANDS);
    char* token = strtok(input_copy, sep);
    int i=0;

    while (token!=NULL){
        Commands[i] = removeLeadingSpaces(lowercase(token));
        categorize_fg_bg_process(Commands[i++]);
        token = strtok(NULL, sep);
    }
    free(Commands);
}


void categorize_fg_bg_process(char* input){

    char** Commands = (char**)malloc(sizeof(char*)*MAX_COMMANDS);
    char** bg_command = (char**)malloc(sizeof(char*)*MAX_COMMANDS);

    char* input_copy = (char*)malloc(sizeof(char)*(strlen(input)+1));
    strcpy(input_copy, input);
    free(input);

    char* token = strtok(input_copy, "&");
    int command_count=0;

    while (token!=NULL){
        Commands[command_count++] = token;
        token = strtok(NULL, sep);
    }

    for(int i=0; i<command_count; i++){
        // if(Commands[i][strlen(Commands[i]) - 1]=='&'){
        //     Commands[i][strlen(Commands[i]) - 1] = '\0';
        // }
        bg_command[i] = Commands[i];
    }

    // If not a bg process
    if(Commands[command_count-1][strlen(Commands[command_count-1]) - 1]!=38){
        processInput(Commands[command_count-1]);
    }
    free(Commands);
    free(bg_command);
}


/* Function to handle input i.e. identify the command and arguments */
void processInput(char* input)
{
    char* input_copy = strdup(input);
    char* input_copy2 = strdup(input);
    free(input);

    char** command_string = (char**)malloc(sizeof(char*)*MAX_COMMAND_LENGTH);
    char* command;

    int i=0;
    while  ((command = strsep(&input_copy, " ")) != NULL){
        command_string[i++] = command;
    }
    
    if(strcmp(command_string[0], "pastevents")!=0){
        if(history_size==0 || strcmp(input_copy2, history_buffer[history_pointer])!=0)
        AddCommandToHistory(input_copy2);
    }

    if(strcmp(command_string[0], "exit")==0){
        WriteToHistory();
        free(command_string);
        deleteHistory();
        exit(0); // Closing the shell if the user typed exit
    }

    else if(strcmp(command_string[0], "warp")==0){
        changeDirectory(command_string, i);
    }

    else if(strcmp(command_string[0], "peek")==0){
        listFiles_Directory(command_string, i);
    }

    else if(strcmp(command_string[0], "seek")==0){
        find(command_string, i);
    }

    else if(strcmp(command_string[0], "pastevents")==0){
        processPasteventInput(command_string, i, input_copy2);
    }

    else{
        printf("%s is an Invalid Command\n", command_string[0]);
    }
    free(command_string);
}