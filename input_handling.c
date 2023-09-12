#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "history.h"
#include "input_redirection.h"
#include "input_handling.h"
#include "helper_functions.h"
#include "piping.h"
#include "bg_process.h"
#include "proclore.h"
#include "ls.h"
#include "path_handling.h"
#include "find.h"


/* Function to Tokenize the input based on semicolon*/
void tokenizeInput(char* input){
    char Commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];

    int semicolon_count=1;
    characterParser(Commands, input, &semicolon_count, ';');
    
    for (int i=0; i<semicolon_count; i++){
        categorize_fg_bg_process(lowercase(removeLeadingSpaces(Commands[i++])));
    }
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
        characterParser(command_string, removeLeadingSpaces(Commands[i]), &arguments, ' ');
        arguments += 1;
        execute_command(command_string, arguments, 1);
    }

    // If not a bg process
    if(condition){
        //processInput(removeLeadingSpaces(Commands[i]));
        pipeInputString(removeLeadingSpaces(Commands[i]));
    }
}


/* Function to process input string i.e. identify the command*/
void processInput(char input[])
{
    input_redirection(input);
    removeLeadingSpaces(input);
    
    char input_copy[MAX_COMMAND_LENGTH];
    sprintf(input_copy, "%s", input);

    int n_arguments=1;
    char command_string[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    characterParser(command_string, input_copy, &n_arguments, ' ');
    deleteQuotes(command_string, n_arguments);

    if(strcmp(command_string[0], "pastevents")!=0){
        if(history_size==0 || strcmp(input, history_buffer[history_pointer])!=0)
        AddCommandToHistory(input);
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
        processPasteventInput(command_string, n_arguments, input);
    }

    else{
        execute_command(command_string, n_arguments, 0);
    }
}