#include <stdio.h>

void print_green(char* text){
    printf("\033[0;32m%s\033[0;m", text);
}

void print_blue(char* text){
    printf("\033[0;34m%s\033[0;m", text);
}

void print_red(char* text){
    printf("\033[0;31m%s\033[0;0m", text);   
}

void print_error(char* text){
    fprintf(stderr, "\033[0;31m%s\033[0;0m", text);   
}

void print_white(char* text){
    printf("\033[0;37m%s\033[0;0m", text);
}

void print_default(char* text){
    printf("\033[0;0m%s\033[0;0m", text);
}