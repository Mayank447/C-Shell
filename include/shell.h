#ifndef SHELL_H
#define SHELL_H

#include "bg_process.h"

struct lexoFileDir {
    char all_details[1000];
    char filename[1000];
};

extern char* home_directory;
extern char* current_directory;
extern char* relative_dir;
extern char* previous_directory;
extern char* process_time;

extern char** history_buffer;
extern int history_pointer;
extern int history_size;
extern int temp_history_pointer;
extern char* history_string;
extern char* shell_input;

extern int saved_STDIN;
extern int saved_STDOUT;
extern int saved_STDDERR;

extern struct termios orig_termios;
extern int process_count;
extern struct Process process_buffer[1000];
extern char error_buffer[1024];

void exit_shell();

#endif