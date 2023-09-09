#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "history.h"
#include "bg_process.h"
#include "path_handling.h"
#include "ls.h"
#include "find.h"
#include "proclore.h"
#include "input_redirection.h"
#include "input_handling.h"
#include "helper_functions.h"


/* Function to Tokenize the input based on semicolon*/
void tokenizeInput(char* input){
    char Commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];

    int semicolon_count=0;
    characterParser(Commands, input, &semicolon_count, ';');
    
    for (int i=0; i<semicolon_count+1; i++){
        categorize_fg_bg_process(lowercase(removeLeadingSpaces(Commands[i++])));
    }
}

void getCommandWithArguments(char command_string[][MAX_COMMAND_LENGTH], char* input_copy, int* argument){
    char* command;

    int i=0;
    while  ((command = strsep(&input_copy, " ")) != NULL){
        strcpy(command_string[i++], command);
    }
    *argument = i;
}

void categorize_fg_bg_process(char input[])
{
    char Commands[MAX_ARGUMENTS][MAX_COMMAND_LENGTH];
    int ampercent_count=0, arguments;
    
    int condition = (input[strlen(input)-1]=='&') ? 0:1;
    characterParser(Commands, input, &ampercent_count, '&');

    char command_string[MAX_ARGUMENTS][MAX_COMMAND_LENGTH];
    int i=0;

    for(; i < ampercent_count; i++){
        input_redirection(Commands[i]);
        getCommandWithArguments(command_string, removeLeadingSpaces(Commands[i]), &arguments);
        execute_command(command_string, arguments, 1);
    }

    // If not a bg process
    if(condition){
        processInput(removeLeadingSpaces(Commands[i]));
    }
}


/* Function to process input string i.e. identify the command*/
void processInput(char input[])
{
    input_redirection(input);
    char input_copy[MAX_COMMAND_LENGTH];
    sprintf(input_copy, "%s", input);

    char* input_copy2 = strdup(input);

    int n_arguments;
    char command_string[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    getCommandWithArguments(command_string, input_copy, &n_arguments);
    
    if(strcmp(command_string[0], "pastevents")!=0){
        if(history_size==0 || strcmp(input_copy2, history_buffer[history_pointer])!=0)
        AddCommandToHistory(input_copy2);
    }

    if(strcmp(command_string[0], "exit")==0){
        WriteToHistory();
        deleteHistory();
        exit(0); // Closing the shell if the user typed exit
    }

    else if(strcmp(command_string[0], "warp")==0){
        changeDirectory(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "peek")==0){
        listFiles_Directory(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "proclore")==0){
        proclore(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "seek")==0){
        find(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "pastevents")==0){
        processPasteventInput(command_string, n_arguments, input_copy2);
    }

    else{
        execute_command(command_string, n_arguments, 0);
    }
    free(input_copy2);
}