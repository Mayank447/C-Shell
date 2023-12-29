#ifndef PASTEVENTS_H
#define PASTEVENTS_H

void ReadHistoryFromFile();
void processPasteventInput(char command_string[][1000], int arguments);
int AddCommandToHistory(char* input);
void WriteToHistory();
void purgeHistory();
void deleteHistory();
void previousCommand(char* input, int* pt);
void nextCommand(char* input, int* pt);

#endif