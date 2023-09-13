#ifndef SHELL_H
#define SHELL_H

#include "bg_process.h"

extern char* home_directory;
extern char* current_directory;
extern char* relative_dir;
extern char* previous_directory;
extern char* process_time;

extern int MAX_INPUT_LENGTH;
extern int MAX_PATH_LENGTH;
extern int MAX_HISTORY_SIZE;
extern int MAX_FILE_NAME_LENGTH;

extern char** history_buffer;
extern int history_pointer;
extern int history_size;
extern int temp_history_pointer;

extern int MAX_COMMANDS;
extern int MAX_COMMAND_LENGTH;
extern int MAX_ARGUMENTS;
extern int MAX_ARGUMENT_LENGTH;

extern int saved_STDIN;
extern int saved_STDOUT;
extern int saved_STDDERR;

extern int process_count;
extern struct Process process_buffer[1000];

void custom_error(const char* error);
void die(const char *s);

#endif