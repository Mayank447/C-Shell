#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "path_handling.h"
#include "input_handling.h"


// Defining some length variables(Maximum charater lengths)
#define MAX_INPUT_LENGTH 1000
int MAX_PATH_LENGTH = 1000;

// Global Variables - > System Info
char* systemName;
char* userName;
struct utsname systemInfo;

// Global Variables - > Directory handling
char* home_directory;
char* current_directory;
char* relative_dir;
char* previous_directory;


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
    relative_dir = relativePath(home_directory, current_directory);
    return 0;
}


// Function to print out the error message and exit with value 1
void die(const char *s){
    perror(s);
    exit(1);
}

int main(int argc, char* argv[]){

    // if(argc > 1){
    //     for(int i=1; i<argc; i++){
    //         argv[i];
    //     }
    //     exit(0);
    // }

    if(get_username_syetemname_cwd()) {
        printf("ERROR: Getting Username or SystemName\n");
    }

    char* input = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
    size_t len;

    do{
        printf("<%s@%s:%s> ",userName, systemName, relative_dir);
        fflush(stdout);
        getline(&input, &len,stdin);
        tokenizeInput(input);
        
    } while(1);
    exit(0);
}