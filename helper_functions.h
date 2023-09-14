#ifndef HELPER_FN
#define HELPER_FN

char* lowercase(char* input);
char* removeLeadingSpaces(char* input);
void characterParser(char commands[][1000], char input[], int* ampercent, char c);
void deleteQuotes_Slashes(char input[][1000], int n_arguments);
void getCommandfromString(char* input, char* command);
int isNumber(char* s);

#endif