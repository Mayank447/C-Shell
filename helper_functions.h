#ifndef HELPER_FN
#define HELPER_FN

char* lowercase(char* input);
char* removeLeadingSpaces(char* input);
void characterParser(char commands[][1000], char input[], int* ampercent, char c);
void deleteQuotes(char input[][1000], int n_arguments);

#endif