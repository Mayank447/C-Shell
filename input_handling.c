#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 100

const char sep[] = ";\n\r\v\f";

// Function to convert the input to lowercase
char* lowercase(char* input){
    int i=0;
    while(input[i]!='\0'){
        if(input[i]>='A' && input[i]<='Z'){
            input[i] = input[i] + 32;
        }
        i++;
    }
    return input;
}


//Function to remove leading spcae and replace multiple or tab spaces with a single space
char* removeLeadingSpaces(char* input){
    int i=0, j=0;
    while(input[i]==' ' || input[i]=='\t'){
        i++;
    }
    while(input[i]!='\0'){
        input[j++] = input[i++];
        if(input[i]==' ' || input[i]=='\t'){
            while(input[i]==' ' || input[i]=='\t'){
                i++;
            }
            input[j++] = ' ';
        }
    }
    input[j] = '\0';
    return input;
}


// Function to Tokenize the input
char** tokenizeInput(char* input)
{
    char** Commands = (char**)malloc(sizeof(char*)*MAX_COMMANDS);
    char* token = strtok(input, sep);
    int i=0;

    while (token!=NULL){
        Commands[i++] = removeLeadingSpaces(lowercase(token));
        token = strtok(NULL, sep);
    }
    return Commands;
}