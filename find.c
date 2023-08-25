#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// TODO: Relative path showing wrt ~ 

#include "shell.h"
#include "path_handling.h"
#include "color.h"

int FOUND;


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
            else FOUND = 1;

            struct stat sb;
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            //If a flag was set
            if(search && (d_flag || f_flag || e_flag))
            { 
                //Directory
                if(d_flag==1 && dp->d_type==DT_DIR) {
                    print_blue(path);
                    printf("\n");
                }

                //File
                if(f_flag==1 && dp->d_type==DT_REG) {
                    print_green(path);
                    printf("\n");
                }
                
                //Executable
                if(e_flag==1 && stat(dp->d_name, &sb) == 0 && sb.st_mode & S_IXUSR){
                    printf("%s/%s %d\n", basePath, dp->d_name, dp->d_type);
                }
            }

            // If no flag was set
            else if (search) {
                if(dp->d_type==DT_DIR) {
                    print_blue(path);
                    printf("\n");
                }
                else if(dp->d_type==DT_REG) {
                    print_green(path);
                    printf("\n");
                }
                else {
                    print_red(path);
                    printf("\n");
                }
            }

            free(name);
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
                if(command_string[i][f]=='d'){
                    d_flag = 1;
                }
                else if(command_string[i][f]=='f'){
                    f_flag = 1;
                }
                else if(command_string[i][f]=='e'){
                    e_flag = 1;
                }
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

    char* dir =  ".";
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

    FOUND = 0;
    findFiles_DirectoryRecursively(file, dir, d_flag, f_flag, e_flag);
    if(!FOUND) printf("No match found!\n");
}