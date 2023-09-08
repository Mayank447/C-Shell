#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "history.h"
#include "bg_process.h"
#include "path_handling.h"
#include "ls.h"
#include "find.h"
#include "proclore.h"

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

void ampercentParser(char commands[][MAX_COMMAND_LENGTH], char input[], int* ampercent){
    int l = strlen(input), j=0;
    int command_count=0;
    
    int single_quotes = 0;
    int double_quotes = 0;

    for(int i=0; i<l; i++){
        if(input[i]=='\''){
            single_quotes = (single_quotes+1)%2;
        }

        else if(input[i]=='\"'){
            double_quotes = (double_quotes+1)%2;
        }

        else if(input[i]=='&' && !single_quotes && !double_quotes) {
            input[i]='\0';
            strcpy(commands[command_count++], input + j);
            j=i+1;
            *ampercent += 1;
        }
    }
    strcpy(commands[command_count], input + j);
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
    char Commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];

    char* token = strtok(input_copy, sep);
    int i=0;

    while (token!=NULL){
        strcpy(Commands[i], lowercase(removeLeadingSpaces(token)));
        // printf("Token: %s\n", Commands[i]);
        categorize_fg_bg_process(Commands[i++]);
        token = strtok(NULL, sep);
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
    char Commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int ampercent_count=0, arguments;
    
    int condition = (input[strlen(input)-1]=='&') ? 0:1;
    ampercentParser(Commands, input, &ampercent_count);

    char command_string[MAX_ARGUMENTS][MAX_COMMAND_LENGTH];
    int i=0;
    for(; i < ampercent_count; i++){
         getCommandWithArguments(command_string, removeLeadingSpaces(Commands[i]), &arguments);
        //printf("%s\n", removeLeadingSpaces(Commands[i]));
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
    char* input_copy = strdup(input);
    char* input_copy2 = strdup(input);

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

    else if(strcmp(command_string[0], "proclore")==0){
        proclore(command_string, i);
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