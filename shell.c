#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "path_handling.h"
#include "input_handling.h"
#include "history.h"


// Defining some length variables(Maximum charater lengths)
int MAX_INPUT_LENGTH = 1000;
int MAX_PATH_LENGTH = 1000;
int MAX_HISTORY_SIZE = 15;
int MAX_FILE_NAME_LENGTH = 256;

int MAX_COMMANDS = 100;
int MAX_COMMAND_LENGTH = 1000;
int MAX_ARGUMENTS = 100;

// Global Variables - > System Info
char* systemName;
char* userName;
struct utsname systemInfo;
int SHELL_PID;

// Global Variables - > Directory handling
char* home_directory;
char* current_directory;
char* relative_dir;
char* previous_directory;

// Global history
char** history_buffer;
int history_pointer;
int history_size;

int get_username_syetemname_cwd(){
    userName = getlogin();
    if(uname(&systemInfo)>0){
        perror("uname() error");
        return 1;
    }
    systemName = systemInfo.nodename;
    home_directory = getcwd(home_directory, MAX_PATH_LENGTH);
    
    current_directory = (char*)malloc(sizeof(char)*MAX_PATH_LENGTH);
    previous_directory = (char*)malloc(sizeof(char)*MAX_PATH_LENGTH);

    strcpy(current_directory, home_directory);
    strcpy(previous_directory, home_directory);
    relative_dir = relativePath(current_directory);
    return 0;
}


// Function to print out the error message and exit with value 1
void die(const char *s){
    perror(s);
    exit(1);
}

// Function to print custom error
void custom_error(const char *s){
    fprintf(stderr, s);
    fprintf(stderr, "\n");
}

int main(int argc, char* argv[]){

    // if(argc > 1){
    //     for(int i=1; i<argc; i++){
    //         argv[i];
    //     }
    //     exit(0);
    // }

    ReadHistoryFromFile();

    if(get_username_syetemname_cwd()) {
        printf("ERROR: Getting Username or SystemName\n");
    }

    char* input = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
    size_t len;

    do{
        printf("\033[1;0m<%s@%s:%s> ",userName, systemName, relative_dir);
        fflush(stdout);
        getline(&input, &len,stdin);
        tokenizeInput(input);
        WriteToHistory();
        
    } while(1);
    free(input);
    deleteHistory();
    exit(0);
}