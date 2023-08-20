#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "shell.h"

/* Function to list all the files and directories in the given path*/
void listFiles_Directory(char** input, int arguments)
{
    // Checking is the last argument is a flag
    int condition = (input[arguments-1][0]==45 && strlen(input[arguments-1])!=1);
    
    // Searching for flags
    int op_l = 0, op_a = 0;
    for (int i=1; i < arguments -1 + condition; i++){
        if(strcmp(input[i], "-l")==0){
            op_l = 1;
        }

        else if(strcmp(input[i], "-a")==0){
            op_a = 1;
        }

        else if(strcmp(input[i], "-la")==0 || strcmp(input[i], "-al")==0){
            op_l = 1;
            op_a = 1;
        }

        else{
            printf("ERROR: %s is not a valid option\n", input[i]);
            return;
        }
    }

    char* path;
    if(arguments==1 || condition) path="."; // ls returns the files in the current directory
    else path = input[arguments-1];

    // Checking for path relative to home directory
    if(path[0]==126){
        char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        strcpy(temp, home_directory);
        strcat(temp, path+1);
        path = temp;
    }

    // Checking for path relative to previous directory
    else if(path[0]==45){
        char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        strcpy(temp, previous_directory);
        strcat(temp, path+1);
        path = temp;
    }

    struct dirent *d;
	DIR *dh = opendir(path);

    // Trying to open the given directory and if it fails, throw error and exit
	if (!dh){
		if (errno == ENOENT) perror("Directory doesn't exist"); // directory not found
        else perror("Unable to read directory"); // directory is not readable
		return;
	}

    while((d=readdir(dh))!=NULL){
        if (!op_a && d->d_name[0] == '.')
			continue;
		printf("%s  ", d->d_name);
		if(op_l) printf("\n");
    }
    if(!op_l) printf("\n");
}