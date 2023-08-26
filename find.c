#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include "errno.h"
#include "shell.h"
#include "path_handling.h"
#include "color.h"
#include "path_handling.h"

int COUNT;
char* PREVIOUS_FOUND;
int PREVIOUS_TYPE;

char* extractFileName(char* input){
    char* temp = (char*)malloc(sizeof(char) * MAX_FILE_NAME_LENGTH);
    int i = 0;
    while((input[i]!='.' || i==0) && input[i]!='\0'){
        temp[i] = input[i];
        i++;
    }
    temp[i]='\0';
    return temp;
}

void PrintDirectory(char* path){
    print_blue(path);
    printf("\n");
}

void PrintFiles(char* path){
    print_green(path);
    printf("\n");
}

void findFiles_DirectoryRecursively(char* file, char* basePath, int d_flag, int f_flag, int e_flag){

    char path[MAX_PATH_LENGTH];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;
    
    while ((dp = readdir(dir)) != NULL)
    {
        char* name = extractFileName(dp->d_name);
        int search = 1;
        
        // Don't recursively search the current or previous directory
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            
            // If filename doesn't match don't print
            if(strcmp(name, file)!=0) search=0;
            else if (e_flag) {
                if(d_flag && dp->d_type!=DT_DIR) search = 0;
                else if(f_flag && dp->d_type!=DT_REG) search=0;
            }
            else COUNT++;
            free(name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            //If e flag was set
            if(search && e_flag)
            {   
                // First time encountering a file
                if(!PREVIOUS_FOUND && COUNT==0){
                    PREVIOUS_FOUND = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
                    strcpy(PREVIOUS_FOUND, path);
                    PREVIOUS_TYPE = dp->d_type;
                    COUNT++;
                }

                // More than once occurence
                else if(COUNT>0){
                    if(dp->d_type==DT_DIR && !f_flag) {
                        PrintDirectory(path);
                        COUNT++;
                    }
                    else if(dp->d_type==DT_REG && !d_flag) {
                        PrintFiles(path);
                        COUNT++;
                    }
                }
            }

            // Only d or f flag set
            else if(search && (d_flag || f_flag)){

                if(d_flag==1 && dp->d_type==DT_DIR) PrintDirectory(path); //Directory
                else if(f_flag==1 && dp->d_type==DT_REG) PrintFiles(path); //File
            }

            // If no flag was set
            else if (search) {
                if(dp->d_type==DT_DIR) PrintDirectory(path);
                else if(dp->d_type==DT_REG) PrintFiles(path);
            }

            findFiles_DirectoryRecursively(file, path, d_flag, f_flag, e_flag);
        }
    }

    closedir(dir);
}


// Main find function
void find(char** command_string, int arguments){
    
    // Determining the flags used
    int d_flag = 0;
    int f_flag = 0;
    int e_flag = 0;
    int flag_end = 0; // The argument at which the flags end

    for (int i=1; i<arguments; i++){
        if(command_string[i][0]=='-' && strlen(command_string[i])!=1 && flag_end != i-1){
            printf("Incorrect syntax!\n");
            return;
        }
        
        if(command_string[i][0]=='-' && strlen(command_string[i])!=1){

            for(int f=1; f<strlen(command_string[i]); f++){
                if(command_string[i][f]=='d') d_flag = 1;
                else if(command_string[i][f]=='f') f_flag = 1;
                else if(command_string[i][f]=='e') e_flag = 1;
                else{
                    printf("Invalid flag\n");
                    return;
                }
            }
            flag_end = i;
        }
    }

    // Both d and f flags can't be on at the same time
    if(d_flag && f_flag){
        printf("Invalid flags!\n");
        return;
    }

    // Error handling for arguments
    if(arguments > flag_end + 3){
        printf("Too many arguments sepcified\n");
        return;
    }

    else if(flag_end + 1 == arguments){
        printf("No arguments specified\n");
        return;
    }

    char* dir =  "."; char* temp=NULL;
    char* file = command_string[flag_end+1];
    if(flag_end + 2 != arguments)
         dir = command_string[flag_end+2];
    
    // Checking for relative path
    if(dir[0]=='~'){
        char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        strcpy(temp, home_directory);
        strcat(temp, dir+1);
        dir = temp;
    }

    else if(dir[0]=='-'){
        char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        strcpy(temp, previous_directory);
        strcat(temp, dir+1);
        dir = temp;
    }

    COUNT = 0;
    PREVIOUS_FOUND = NULL;
    findFiles_DirectoryRecursively(file, dir, d_flag, f_flag, e_flag);
    
    if(COUNT==0) printf("No match found!\n");

    else if(e_flag && PREVIOUS_FOUND!=NULL) 
    {
        if(COUNT==1 && PREVIOUS_TYPE==DT_DIR) {
            int err = chdir(PREVIOUS_FOUND);
            if (err == -1) fprintf(stderr, "seek : -e : %s\n", strerror(errno));

            current_directory = getcwd(current_directory, MAX_PATH_LENGTH);
            relative_dir = relativePath(current_directory);
        }
        
        else if(COUNT>1 && PREVIOUS_TYPE==DT_DIR) PrintDirectory(PREVIOUS_FOUND);

        else if(PREVIOUS_TYPE==DT_REG){
            PrintFiles(PREVIOUS_FOUND);

            if(COUNT==1) {
                FILE* f = fopen(PREVIOUS_FOUND, "r");
                char * line = NULL;
                size_t len = 0;
                ssize_t read;
                while (f!=NULL && (read = getline(&line, &len, f)) != -1) {
                    printf("%s", line);
                }
                printf("\n");
                fclose(f);
            }
        }
    }

    COUNT = 0;
    if(PREVIOUS_FOUND) {
        free(PREVIOUS_FOUND);
        PREVIOUS_FOUND = NULL;
    }
    if(temp){
        free(temp);
        temp = NULL;
    }
}