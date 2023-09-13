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
#include "iman.h"
#include "activities.h"
#include "signal_handling.h"

void addForegroundCommandToHistory(char* command)
{
    if(strcmp(command, "pastevents")!=0 && strcmp(command, "pastevents purge")){
        if(history_size && strcmp(command, history_buffer[history_pointer])) 
            strcat(history_string, command);
    }
    else if(strcmp(command, "pastevents purge")==0){
        purgeHistory();
    }
    // else if(strnstr(command, "pastevents execute")==0){
    //     PrintHistory();
    // }
}


/* Function to Tokenize the input based on semicolon*/
void tokenizeInput(char* input)
{
    char Commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];

    int semicolon_count=1;
    int condition = (input[strlen(input)-1]==';') ? 1:0;
    characterParser(Commands, input, &semicolon_count, ';');
    memset(history_string, '\0', MAX_INPUT_LENGTH);

    for (int i=0; i<semicolon_count - condition; i++){
        categorize_fg_bg_process(lowercase(removeLeadingSpaces(Commands[i])));
        if(semicolon_count > 1){
            strcat(history_string, ";");
        }
    }
    if(condition) strcat(history_string, ";");
    AddCommandToHistory(history_string);
    WriteToHistory();
}


void categorize_fg_bg_process(char input[])
{
    char Commands[MAX_ARGUMENTS][MAX_COMMAND_LENGTH];
    int ampercent_count=0, arguments=1;
    
    int condition = (input[strlen(input)-1]=='&') ? 0:1;
    characterParser(Commands, input, &ampercent_count, '&');

    char command_string[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    int i=0;

    for(; i < ampercent_count; i++)
    {
        // Writing to the history_string
        arguments = 1;
        removeLeadingSpaces(Commands[i]);
        strcat(history_string, Commands[i]);
        strcat(history_string, "& ");

        // Storing the background process in the process buffer
        store_process();
        process_buffer[process_count].bg = 1;
        strcpy(process_buffer[process_count].entire_command, Commands[i]);

        characterParser(command_string, Commands[i], &arguments, ' ');
        strcpy(process_buffer[process_count].command, command_string[0]);
        execute_command(command_string, arguments, 1);
    }

    // If not a bg process
    if(condition){
        removeLeadingSpaces(Commands[i]);
        //addForegroundCommandToHistory(Commands[i]);
        WriteToHistory();
        
        pipeInputString(Commands[i]);
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

    if(strcmp(command_string[0], "exit")==0){
        // WriteToHistory();
        deleteHistory();
        exit(0); // Closing the shell if the user typed exit
    }

    else if(strcmp(command_string[0], "warp")==0){
        changeDirectory(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "peek")==0){
        listFiles_Directory(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "activities")==0){
        activities();
    }

    else if(strcmp(command_string[0], "proclore")==0){
        proclore(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "ping")==0){
        signal_handler(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "seek")==0){
        find(command_string, n_arguments);
    }

    else if(strcmp(command_string[0], "pastevents")==0){
        processPasteventInput(command_string, n_arguments, input);
    }

    else if(strcmp(command_string[0], "iman")==0){
        iman(command_string, n_arguments);
    }

    else{
        execute_command(command_string, n_arguments, 0);
    }
}