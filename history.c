#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "input_handling.h"

// Fn. to read history from file to history buffer
void ReadHistoryFromFile()
{
    history_buffer = (char**)malloc(sizeof(char*)*MAX_HISTORY_SIZE);

    if (access(".CShell_history", F_OK) != 0){
        FILE* fp = fopen(".CShell_history", "w");
        if(fp==NULL){
            perror("Error creating a history file");
            return;
        }
        fclose(fp);
        history_size = 0;
        history_pointer = 0;
        return;
    }

    FILE* fp = fopen(".CShell_history", "r");
    if(fp==NULL){
        perror("Error opening history file");
        return;
    }

    // If the file is empty
    int c = fgetc(fp);
    if (c == EOF) {
        history_pointer = 0;
        history_size = 0;
        return;
    }

    fseek(fp, 0, SEEK_SET);
    fscanf (fp, "%d", &c);
    history_pointer = c;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    getline(&line, &len, fp); // reading the null character
    
    int i=0;
    while((read = getline(&line, &len, fp))!=-1){
        history_buffer[i] = (char*)malloc(sizeof(char) * MAX_INPUT_LENGTH);
        strcpy(history_buffer[i], line);

        if (history_buffer[i][read - 1]=='\n') {
            history_buffer[i][read-1] = '\0';
        }
        i+=1;
    }
    history_size = i;
    fclose(fp);
}


// Fn. to write history from history buffer to the file
/* Need to append the commands and change the file pointer to the beginning */
void WriteToHistory()
{
    char* path = (char*)malloc(sizeof(char)*MAX_PATH_LENGTH);
    strcpy(path, home_directory);
    strcat(path, "/.CShell_history");

    FILE* fp = fopen(path, "w");
    if(fp==NULL){
        perror("Error opening history file");
        return;
    }
    fprintf(fp, "%d\n", history_pointer);
    
    int i=0;
    while(i<history_size-1){
        fprintf(fp, "%s\n", history_buffer[i++]);
    }
    if(history_size!=0) fprintf(fp, "%s", history_buffer[i]);
    fclose(fp);
}


// Fn. to add a command to history buffer
void AddCommandToHistory(char* input){

    if(history_size == MAX_HISTORY_SIZE){
        history_pointer = (history_pointer+1)%15;
        free(history_buffer[history_pointer]);
        history_buffer[history_pointer] = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(history_buffer[history_pointer], input);
    }

    else if(history_size == 0){
        history_buffer[history_pointer] = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(history_buffer[history_pointer], input);
        history_size=1;
    }

    else{
        history_buffer[++history_pointer] = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(history_buffer[history_pointer], input);
        history_size++;
    }
}

void deleteHistory(){
    for (int i=0; i<history_size; i++){
        if(history_buffer[i]!=NULL)
            free(history_buffer[i]);
    }
    if(history_buffer!=NULL) free(history_buffer);
}


// Fn. to clear the history
void purgeHistory(){
    for (int i=0; i<history_size; i++){
        free(history_buffer[i]);
    }
    history_size = 0;
    history_pointer = 0;
}


// Fn. to print the history
void PrintHistory(){
    if(history_size == MAX_HISTORY_SIZE){
        int j = (history_pointer+1)%15;
        for (int i=0; i<history_size; i++){
            printf("%s\n", history_buffer[j]);
            j = (j+1)%15;
        }
        return;
    }

    for (int i=0; i<history_size; i++){
        printf("%s\n", history_buffer[i]);
    }
}


void processPasteventInput(char** command_string, int arguments, char* input){
    
    if(arguments == 1){ // Print the history
        PrintHistory();
    }

    else if(arguments == 2 && strcmp(command_string[1], "purge")==0){
        purgeHistory(); // Purge the history
    }

    // Executing a command from history
    else if(arguments == 3 && strcmp(command_string[1], "execute")==0){
        int index = atoi(command_string[2]);
        if(index>history_size || index<=0){
            fprintf(stderr, "ERROR: Invalid index\n");
            return;
        }
        if(index!=1) AddCommandToHistory(input);
        char* input = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(input, history_buffer[(history_pointer-index+1)%15]);
        tokenizeInput(input);
    }

    else{
        fprintf(stderr, "ERROR: Invalid command\n");
    }
}