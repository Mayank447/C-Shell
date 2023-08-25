#ifndef SHELL_H
#define SHELL_H

extern char* home_directory;
extern char* current_directory;
extern char* relative_dir;
extern char* previous_directory;

extern int MAX_INPUT_LENGTH;
extern int MAX_PATH_LENGTH;
extern int MAX_HISTORY_SIZE;

extern char** history_buffer;
extern int history_pointer;
extern int history_size;

#endif