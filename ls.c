#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "shell.h"
#include "color.h"

int compare_lexo(void* a, void* b){
    struct lexoFileDir *file1 = (const struct lexoFileDir*)a;
    struct lexoFileDir *file2 = (const struct lexoFileDir*)b;
    return strcmp(file1->filename, file2->filename);
}

void print_permissions_file_type(mode_t mode, char* print_string){
    if (S_ISREG(mode)) strcat(print_string, "-");
    else if (S_ISDIR(mode)) strcat(print_string, "d");
    else if (S_ISCHR(mode)) strcat(print_string, "c");
    else if (S_ISBLK(mode)) strcat(print_string, "b");                                                // this block prints file type
    else if (S_ISFIFO(mode)) strcat(print_string, "p");
    else if (S_ISLNK(mode)) strcat(print_string, "l");
    else if (S_ISSOCK(mode)) strcat(print_string, "s");
    else strcat(print_string, "-");
}

void print_permissions(mode_t mode, char* print_string) {
    print_permissions_file_type(mode, print_string);
    strcat(print_string, (mode & S_IRUSR) ? "r" : "-");
    strcat(print_string, (mode & S_IWUSR) ? "w" : "-");
    strcat(print_string, (mode & S_IXUSR) ? "x" : "-");
    strcat(print_string, (mode & S_IRGRP) ? "r" : "-");
    strcat(print_string, (mode & S_IWGRP) ? "w" : "-");
    strcat(print_string, (mode & S_IXGRP) ? "x" : "-");
    strcat(print_string, (mode & S_IROTH) ? "r" : "-");
    strcat(print_string, (mode & S_IWOTH) ? "w" : "-");
    strcat(print_string, (mode & S_IXOTH) ? "x" : "-");
}

void l_flag_print(struct stat file_stat, char* print_string)
{
    //Permissions
    print_permissions(file_stat.st_mode, print_string);

    //Hard links 
    sprintf(print_string + strlen(print_string), "%d\t",file_stat.st_nlink); 

    // User and Group name
    struct passwd *user_info = getpwuid(file_stat.st_uid); 
    struct group *group_info = getgrgid(file_stat.st_gid);

    strcat(print_string, (user_info != NULL) ? user_info->pw_name : "unknown");
    strcat(print_string, "\t");
    strcat(print_string, (group_info != NULL) ? group_info->gr_name : "unknown");
    strcat(print_string, "\t");
    
    // File size
    sprintf(print_string + strlen(print_string), "%lld\t", file_stat.st_size);

    // Last modified time
    char time_buffer[100];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
    strcat(print_string, time_buffer);
    strcat(print_string, "\t");
}


// Function to list all the files and directories in the given path
void listFiles_Directory(char input[][MAX_ARGUMENT_LENGTH], int arguments)
{
    // Checking is the last argument is a flag
    int condition = (input[arguments-1][0]==45 && strlen(input[arguments-1])!=1);
    
    // Searching for flags
    int op_l = 0, op_a = 0;
    for (int i=1; i < arguments -1 + condition; i++){
        if(strcmp(input[i], "-l")==0)
            op_l = 1;

        else if(strcmp(input[i], "-a")==0)
            op_a = 1;

        else if(strcmp(input[i], "-la")==0 || strcmp(input[i], "-al")==0){
            op_l = 1;
            op_a = 1;
        }

        else{
            fprintf(stderr, "\033[0;31mERROR: %s is not a valid option\n\033[0;0m", input[i]);
            return;
        }
    }

    char* path = ".";
    if(arguments!=1 && !condition) path = input[arguments-1];

    // Checking for path relative to home directory
    int free_temp = 0;
    if(path[0]=='~'){
        char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        strcpy(temp, home_directory);
        strcat(temp, path+1);
        path = temp;
        free_temp = 1;
    }

    // Checking for path relative to previous directory
    else if(path[0]=='-'){
        char* temp = (char*)malloc(sizeof(char) * MAX_PATH_LENGTH);
        strcpy(path, previous_directory);
        strcat(temp, path+1);
        path = temp;
        free_temp = 1;
    }

    // Trying to open the given directory and if it fails, throw error and exit
    struct dirent *d;
	DIR *dh = opendir(path);

	if (!dh){
		if (errno == ENOENT) 
            perror("Directory doesn't exist");
        else 
            perror("Unable to read directory"); 
		if(free_temp) free(path);
        free(dh);
        return;
	}
    
    struct lexoFileDir files_and_directories[MAX_FILES]; //Storing for lexographic printing
    int i = 0;

    // Reading the directory and printing the files and directories
    while((d=readdir(dh))!=NULL){
        char temp[MAX_FILE_PROP_LENGTH];
        memset(temp, '\0', sizeof(temp));

        if (!op_a && d->d_name[0] == '.') continue;

        struct stat file_stat;
        if (stat(d->d_name, &file_stat) == -1) {
            perror("stat");
            return;
        }

		if(op_l) l_flag_print(file_stat, temp);

		if(d->d_type == DT_DIR)
            sprintf(files_and_directories[i++].filename, "\033[0;34m%s\033[0;0m", d->d_name);

        else if(file_stat.st_mode & S_IXUSR)
            sprintf(files_and_directories[i++].filename, "\033[0;32m%s\033[0;0m", d->d_name);
        
        else 
            sprintf(files_and_directories[i++].filename, "\033[0;37m%s\033[0;0m", d->d_name);
        
        strcpy(files_and_directories[i].all_details, temp);
    }

    //Sorting it lexographically
    qsort(files_and_directories, i, sizeof(struct lexoFileDir), compare_lexo);

    for(int j=0; j<i; j++){
        printf("%s%s\n", files_and_directories[j].all_details, files_and_directories[j].filename);
    }

    // Freeing the allocated memory
    if(free_temp) free(path);
    free(d);
    free(dh);
}