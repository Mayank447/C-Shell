#ifndef HISTORY_H
#define HISTORY_H

void ReadHistoryFromFile();
void processPasteventInput(char** command_string, int arguments, char* input);
void AddCommandToHistory(char* input);
void WriteToHistory();
void deleteHistory();

#endif