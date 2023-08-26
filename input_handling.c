#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "history.h"
#include "bg_process.h"
#include "path_handling.h"
#include "ls.h"
#include "find.h"

#define MAX_COMMANDS 100
#define MAX_COMMAND_LENGTH 100

const char sep[] = ";\n\r\v\f";

void categorize_fg_bg_process(char* input);
void processInput(char* input);


/* Function to convert the input to lowercase */
char* lowercase(char* input){
    int i=0;
    while(input[i]!='\0' && input[i]!=' '){
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

/* Function to count occurences of a character in a string*/
int count_occurences(char* s, char c){
    int res = 0;
    for (int i=0; i < strlen(s); i++){
        if (s[i] == c)
            res++;
    }
    return res;
}

/* Function to Tokenize the input */
void tokenizeInput(char* input){
    char* input_copy = strdup(input);
    char* Commands[MAX_COMMANDS];

    char* token = strtok(input_copy, sep);
    int i=0;

    while (token!=NULL){
        Commands[i] = (char*)malloc(sizeof(char) * MAX_COMMAND_LENGTH);
        strcpy(Commands[i++], lowercase(removeLeadingSpaces(token)));
        printf("Token: %s\n", Commands[i-1]);
        //categorize_fg_bg_process(Commands[i++]);
        token = strtok(NULL, sep);
    }
}

char** getCommandWithArguments(char* input_copy, int* argument){
    char** command_string = (char**)malloc(sizeof(char*)*MAX_COMMAND_LENGTH);
    char* command;

    int i=0;
    while  ((command = strsep(&input_copy, " ")) != NULL){
        command_string[i++] = command;
    }
    *argument = i;
    return command_string;
}

void categorize_fg_bg_process(char* input)
{
    printf("%s\n", input);
    char* Commands[MAX_COMMANDS];
    char* input_copy = strdup(input);
    free(input);

    int count_amphercent = count_occurences(input_copy, '&');
    char* token = strtok(input_copy, "&");
    int command_count=0, argument;

    while (token!=NULL){
        Commands[command_count] = (char*)malloc(sizeof(char) * MAX_COMMAND_LENGTH);
        strcpy(Commands[command_count++], removeLeadingSpaces(token));
        token = strtok(NULL, "&");
    }

    // Checking if the last command is fg or bg
    int condition = command_count - 1;
    if(command_count == count_amphercent) condition=command_count;


    for(int i=0; i<condition; i++){
        char** command_string = getCommandWithArguments(Commands[i], &argument);
        execute_command(command_string, argument, 1);
        free(command_string);
    }

    //If not a bg process
    if(command_count != count_amphercent){
        printf("%s\n", Commands[command_count-1]);
        processInput(Commands[command_count-1]);
    }
}


/* Function to process input string i.e. identify the command*/
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
        execute_command(command_string, i, 0);
    }
    free(input_copy2);
    free(command_string);
}