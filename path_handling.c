#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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