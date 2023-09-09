#include <stdio.h>
#include <string.h>
#include "shell.h"

/* Function to convert the input to lowercase */
char* lowercase(char* input){
    int i=0;
    while(input[i]!='\0' && input[i]!=' '){
        if(input[i]>='A' && input[i]<='Z'){
            input[i] = input[i] + 32;
        }
        i++;
    }
    return input;
}

/* Function to remove leading spcae and replace multiple or tab spaces with a single space */
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
            if(input[i]=='\0') break;
            input[j++] = ' ';
        }
    }
    input[j] = '\0';
    return input;
}

/* Function to parse input string and tokenize it based on chracter provided*/
void characterParser(char commands[][MAX_COMMAND_LENGTH], char input[], int* ampercent, char c){
    int l = strlen(input), j=0;
    if(input[l-1]=='\n') input[l-1]='\0'; // Removing the '\n' character from the end of the input string
    int command_count=0;
    
    int single_quotes = 0;
    int double_quotes = 0;

    for(int i=0; i<l; i++){
        if(input[i]=='\''){
            single_quotes = (single_quotes+1)%2;
        }

        else if(input[i]=='\"'){
            double_quotes = (double_quotes+1)%2;
        }

        else if(input[i]==c && !single_quotes && !double_quotes) {
            input[i]='\0';
            strcpy(commands[command_count++], input + j);
            j=i+1;
            *ampercent += 1;
        }
    }
    strcpy(commands[command_count], input + j);
}