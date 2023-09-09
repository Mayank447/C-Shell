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

void print_permissions_file_type(mode_t mode){
    if (S_ISREG(mode)) putchar('-');
    else if (S_ISDIR(mode)) putchar('d');
    else if (S_ISCHR(mode)) putchar('c');
    else if (S_ISBLK(mode)) putchar('b');                                                // this block prints file type
    else if (S_ISFIFO(mode)) putchar('p');
    else if (S_ISLNK(mode)) putchar('l');
    else if (S_ISSOCK(mode)) putchar('s');
    else putchar('-');
}

void print_permissions(mode_t mode) {
    print_permissions_file_type(mode);
    putchar((mode & S_IRUSR) ? 'r' : '-');
    putchar((mode & S_IWUSR) ? 'w' : '-');
    putchar((mode & S_IXUSR) ? 'x' : '-');
    putchar((mode & S_IRGRP) ? 'r' : '-');
    putchar((mode & S_IWGRP) ? 'w' : '-');
    putchar((mode & S_IXGRP) ? 'x' : '-');
    putchar((mode & S_IROTH) ? 'r' : '-');
    putchar((mode & S_IWOTH) ? 'w' : '-');
    putchar((mode & S_IXOTH) ? 'x' : '-');
}

void l_flag_print(struct stat file_stat)
{
    //Permissions
    print_permissions(file_stat.st_mode);

    //Hard links  
    printf("%d\t", file_stat.st_nlink); 

    // User and Group name
    struct passwd *user_info = getpwuid(file_stat.st_uid); 
    struct group *group_info = getgrgid(file_stat.st_gid);

    printf("%s\t%s\t", (user_info != NULL) ? user_info->pw_name : "unknown", 
            (group_info != NULL) ? group_info->gr_name : "unknown");

    // File size
    printf("%lld\t", file_stat.st_size);

    // Last modified time
    char time_buffer[100];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
    printf("%s\t", time_buffer);
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
            fprintf(stderr, "ERROR: %s is not a valid option\n", input[i]);
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


    // Reading the directory and printing the files and directories
    while((d=readdir(dh))!=NULL){
        if (!op_a && d->d_name[0] == '.')
			continue;

        struct stat file_stat;
        if (stat(d->d_name, &file_stat) == -1) {
            perror("stat");
            return;
        }

		if(op_l) l_flag_print(file_stat);

		if(d->d_type == DT_DIR) 
            print_blue(d->d_name);

        else if(file_stat.st_mode & S_IXUSR)
            print_green(d->d_name);
        
        else 
            print_white(d->d_name);

        printf("\n");
    }

    // Freeing the allocated memory
    if(free_temp) free(path);
    free(d);
    free(dh);
}