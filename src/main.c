#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <termios.h>
#include <fcntl.h>
#include "../include/libs.h"

// Defining some length variables(Maximum charater lengths)
int MAX_INPUT_LENGTH = 1000;
int MAX_PATH_LENGTH = 1000;
int MAX_HISTORY_SIZE = 15;
int MAX_FILE_NAME_LENGTH = 256;
int MAX_FILES = 1000;
int MAX_FILE_PROP_LENGTH = 1000;

int MAX_COMMANDS = 100;
int MAX_COMMAND_LENGTH = 1000;
int MAX_ARGUMENTS = 100;
int MAX_ARGUMENT_LENGTH = 1000;

int MAX_PROCESSES = 1000;


// Global Variables - > System Info
char* systemName;
char* userName;
struct utsname systemInfo;
int SHELL_PID;
char* process_time;
char* shell_input;

// Global Variables - > Directory handling
char* home_directory;
char* current_directory;
char* relative_dir;
char* previous_directory;

// Global vraibles for history
char** history_buffer;
int history_pointer;
int history_size;
int temp_history_pointer; //For up and down arrow
char* history_string;

// Saved File descriptors
int saved_STDIN;
int saved_STDOUT;
int saved_STDDERR;

// Terminal Settings
struct termios orig_termios;

//Processes
int process_count;
struct Process process_buffer[1000];
char error_buffer[1024];

int get_username_syetemname_cwd()
{
    userName = getlogin();
    if(uname(&systemInfo)>0){
        perror("uname() error");
        return 1;
    }
    systemName = systemInfo.nodename;
    home_directory = getcwd(home_directory, MAX_PATH_LENGTH);
    
    current_directory = (char*)malloc(sizeof(char)*MAX_PATH_LENGTH);
    previous_directory = (char*)malloc(sizeof(char)*MAX_PATH_LENGTH);
    relative_dir = (char*)malloc(sizeof(char)*MAX_PATH_LENGTH);

    strcpy(current_directory, home_directory);
    strcpy(previous_directory, home_directory);
    relativePath(current_directory, relative_dir);
    
    process_time = (char*)malloc(sizeof(char) * MAX_COMMAND_LENGTH);
    process_time[0] = '\0';
    return 0;
}

void restore_std(int saved_stdout, int saved_stdin, int saved_stderr){
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
}


void exit_shell(){
    free(shell_input);
    free(userName);
    free(home_directory);
    free(current_directory);
    free(previous_directory);
    free(relative_dir);
    free(history_string);
    deleteHistory();
}

void repeat_loop(char* input, int* pt)
{
    memset(input, '\0', MAX_INPUT_LENGTH);
    memset(history_string, '\0', MAX_INPUT_LENGTH);
    setbuf(stdout, NULL);
    restore_std(saved_STDOUT, saved_STDIN, saved_STDDERR);
    
    if(history_size) temp_history_pointer = (history_pointer + 1) % MAX_HISTORY_SIZE;
    *pt = 0;
}


int main(int argc, char* argv[]){

    // if(argc > 1){
    //     for(int i=1; i<argc; i++){
    //         argv[i];
    //     }
    //     exit(0);
    // }

    // PROCESS HANDLING
    process_count = 0;

    // Standard Input/Output/Error
    saved_STDIN = dup(STDIN_FILENO);
    saved_STDOUT = dup(STDOUT_FILENO);
    saved_STDDERR = dup(STDERR_FILENO);

    ReadHistoryFromFile();

    if(get_username_syetemname_cwd()) {
        print_error("ERROR: Getting Username or SystemName\n");
    }

    char* shell_input = (char*)malloc(sizeof(char) * MAX_INPUT_LENGTH);
    char c = '\0';
    int pt = 0;

    signal(SIGINT, SIG_IGN); // Ctrl+C
    signal(SIGTSTP, SIG_IGN); // Ctrl+Z
    signal(SIGTTOU, SIG_IGN); //Ensures background process doesn't output to the terminal

    while(1){
        repeat_loop(shell_input, &pt);
        printf("\033[1;0m<%s@%s:%s%s> ",userName, systemName, relative_dir, process_time);
        
        rawModeInput(c, shell_input, pt);
        if(!strlen(shell_input)) continue;
        memset(process_time, '\0', MAX_COMMAND_LENGTH);
        
        tokenizeInput(removeLeadingSpaces(shell_input), 1);
        process_finished();
    }
    
    exit_shell();
    exit(0);
}