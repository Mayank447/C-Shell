#ifndef BG_PROCESS_H
#define BG_PROCESS_H

#include <time.h>

struct Process{
  char status[3];
  int pid;
  int bg;
  int already_exitted;
  char command[100];
  char entire_command[1000];
  time_t time_initialized;
};

void store_process_background();
void process_finished();
void execute_command(char command_string[][1000], int argument, int is_background);
void bring_to_foreground(char command_string[][1000], int arguments);
void bg_command(char command_string[][1000], int arguments);

#endif