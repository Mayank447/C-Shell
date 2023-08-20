#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_PATH_LENGTH 10000


/* Function to get the relative path */
char* relativePath(char* home_dir, char* cwd){
    
    int home_dir_length = strlen(home_dir);
    int cwd_length = strlen(cwd);

    // If the length of home_directory is longer than cwd, return cwd
    if(home_dir_length > cwd_length) return cwd;

    // If any character mismatch from index 0, return cwd 
    for (int i=0; i<home_dir_length; i++){
        if(home_dir[i]!=cwd[i])
            return cwd;
    }

    // If the next chracter is not / return cwd (since cwd another sibling directory with same initial name)
    if(home_dir_length!=cwd_length && cwd[home_dir_length]!='/') return cwd;

    int relative_dir_length = cwd_length - home_dir_length + 1;
    char* relative_dir = (char*)malloc(sizeof(char)* relative_dir_length);
    relative_dir[0] = '~';

    for (int i = home_dir_length; i<cwd_length; i++){
        relative_dir[i-home_dir_length + 1] = cwd[i];
    }
    return relative_dir;
}


/* Function to change the directory */
void changeDirectory(char** command_string, int arguments){

    for (int i=1; i<arguments; i++){
        // Checking if the path is ~
        if(strcmp(command_string[i],"~")==0){
            if(chdir(home_directory)){
                perror("ERROR: chdir() error");
            }
        }

        // Checking if the path is relative to the home directory
        else if(command_string[i][0] == 126){
            char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
            strcpy(temp, home_directory);
            strcat(temp, command_string[i]+1);
            if(chdir(temp)!=0){
                printf("ERROR: %s is not a valid directory\n", command_string[i]);
                return;
            }
        }

        // Checking if the path is -
        else if(command_string[i][0] == 45 && strlen(command_string[i])==1){
            if(chdir(previous_directory)){
                perror("ERROR: chdir() error");
            }
        }

        // Checking from relative and absolute paths
        else if(chdir(command_string[i])!=0){
            printf("ERROR: %s is not a valid directory\n", command_string[i]);
            return;
        }
        strcpy(previous_directory, current_directory);
        current_directory = getcwd(current_directory, MAX_PATH_LENGTH);
        relative_dir = relativePath(home_directory, current_directory);
    }
    return;
}