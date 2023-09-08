#ifndef SHELL_H
#define SHELL_H

extern char* home_directory;
extern char* current_directory;
extern char* relative_dir;
extern char* previous_directory;

extern int MAX_INPUT_LENGTH;
extern int MAX_PATH_LENGTH;
extern int MAX_HISTORY_SIZE;
extern int MAX_FILE_NAME_LENGTH;

extern char** history_buffer;
extern int history_pointer;
extern int history_size;

extern int MAX_COMMANDS;
extern int MAX_COMMAND_LENGTH;
extern int MAX_ARGUMENTS;

void custom_error(const char* error);

#endif