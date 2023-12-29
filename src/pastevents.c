#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/libs.h"

// Fn. to read history from file to history buffer
void ReadHistoryFromFile()
{
    history_buffer = (char**)malloc(sizeof(char*) * MAX_HISTORY_SIZE);
    history_string = (char*)malloc(sizeof(char) * MAX_INPUT_LENGTH);

    if (access(".CShell_history", F_OK) != 0){
        FILE* fp = fopen(".CShell_history", "w");
        if(fp==NULL){
            perror("Error creating a history file");
            return;
        }
        fclose(fp);
        history_size = 0;
        history_pointer = 0;
        temp_history_pointer = 0;
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
        temp_history_pointer = 0;
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
    if(history_size) 
        temp_history_pointer = (history_pointer + 1) % MAX_HISTORY_SIZE;
    fclose(fp);
}


// Fn. to write history from history buffer to the file
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
    while(i < history_size - 1){
        fprintf(fp, "%s\n", history_buffer[i++]);
    }
    fprintf(fp, "%s", history_buffer[i]); //Writing the last line
    fclose(fp);
}


// Fn. to add a command to history buffer
int AddCommandToHistory(char* input)
{
    if(history_size == 0){
        history_buffer[history_pointer] = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(history_buffer[history_pointer], input);
        history_size=1;
    }

    else if(strcmp(input, history_buffer[history_pointer])==0){
        return 0;
    }

    else if(history_size == MAX_HISTORY_SIZE){
        history_pointer = (history_pointer+1) % MAX_HISTORY_SIZE;
        free(history_buffer[history_pointer]);
        history_buffer[history_pointer] = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(history_buffer[history_pointer], input);
    }

    else{
        history_buffer[++history_pointer] = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
        strcpy(history_buffer[history_pointer], input);
        history_size++;
    }

    return 1;
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
    WriteToHistory();
    ReadHistoryFromFile();
}

// Fn. to print the history
void PrintHistory(){
    if(history_size == MAX_HISTORY_SIZE){
        int j = (history_pointer+1) % MAX_HISTORY_SIZE;
        for (int i=0; i<history_size; i++){
            printf("%s\n", history_buffer[j]);
            j = (j+1) % MAX_HISTORY_SIZE;
        }
        return;
    }

    for (int i=0; i<history_size; i++){
        printf("%s\n", history_buffer[i]);
    }
}


void processPasteventInput(char command_string[][MAX_ARGUMENT_LENGTH], int arguments)
{
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
            print_error("ERROR: Invalid index\n");
            return;
        }
        char input[MAX_INPUT_LENGTH];
        strcpy(input, history_buffer[(history_pointer-index+1) % MAX_HISTORY_SIZE]);
        tokenizeInput(input, 0);
    }

    else{
        print_error("ERROR: Invalid command\n");
    }
}

void previousCommand(char* input, int* pt){
    if(history_size==0){
        printf(" ");
        return;
    }
    else if(temp_history_pointer!=0) 
        temp_history_pointer = (temp_history_pointer-1) % MAX_HISTORY_SIZE;
    else 
        temp_history_pointer = history_size - 1;
    
    strcpy(input, history_buffer[temp_history_pointer]);
    *pt = strlen(input);
    printf(" %s", input);
}

void nextCommand(char* input, int* pt){
    if(history_size==0 || (temp_history_pointer >= history_size && history_size!=MAX_HISTORY_SIZE)){ 
        printf(" ");
        return;
    }
    else if(temp_history_pointer == history_size-1) 
        temp_history_pointer = 0;
    else 
        temp_history_pointer++;
    
    strcpy(input, history_buffer[temp_history_pointer]);
    *pt = strlen(input);
    printf(" %s", input);
}