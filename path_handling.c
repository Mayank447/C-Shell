#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "color.h"

//TO DO: Put relative path in the while loop

/* Function to get the relative path */
void relativePath(char* cwd, char* relative_dir){
    
    int home_dir_length = strlen(home_directory);
    int cwd_length = strlen(cwd);

    if(strcmp(cwd, home_directory)==0){
        relative_dir[0] = '~';
        relative_dir[1] = '\0';
        return;
    }

    // If the length of home_directory is longer than cwd, return cwd
    if(home_dir_length > cwd_length) {
        strcpy(relative_dir, cwd);
        return;
    }

    // If any character mismatch from index 0, return cwd 
    for (int i=0; i<home_dir_length; i++){
        if(home_directory[i]!=cwd[i]){
            strcpy(relative_dir, cwd);
            return;
        }
    }

    // If the next chracter is not '/' return cwd (since cwd is another sibling directory with same initials)
    if(home_dir_length!=cwd_length && cwd[home_dir_length]!='/') {
        strcpy(relative_dir, cwd);
        return;
    }

    // Else take the remaining part of cwd and return it concatenated with '~'
    relative_dir[0] = '~';
    strcat(relative_dir + 1, cwd + home_dir_length);
}


/* Function to change the directory */
void changeDirectory(char command_string[][MAX_ARGUMENT_LENGTH], int arguments){

    for (int i=1; i<arguments; i++){
        
        // Checking if the path is relative to the home directory
        if(command_string[i][0] == 126){
            char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
            strcpy(temp, home_directory);
            strcat(temp, command_string[i]+1);
            if(chdir(temp)!=0){
                free(temp);
                sprintf(error_buffer, "ERROR: %s is not a valid directory\n", command_string[i]);
                print_error(error_buffer);
                return;
            }
            free(temp);
        }


        // Checking if the path is relative to the previous directory
        else if(command_string[i][0] == 45){
            char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
            strcpy(temp, home_directory);
            strcat(temp, command_string[i]+1);
            if(chdir(temp)!=0){
                free(temp);
                sprintf(error_buffer, "ERROR: %s is not a valid directory\n", command_string[i]);
                print_error(error_buffer);
                return;
            }
            free(temp);
        }

        // Checking if not a path relative to home dir or previous dir
        else if(chdir(command_string[i])!=0){
            sprintf(error_buffer, "ERROR: %s is not a valid directory\n", command_string[i]);
            print_error(error_buffer);
            return;
        }

        strcpy(previous_directory, current_directory);
        current_directory = getcwd(current_directory, MAX_PATH_LENGTH);
        relativePath(current_directory, relative_dir);
        printf("%s\n", current_directory);
    }
    return;
}