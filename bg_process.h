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

void execute_command(char command_string[][1000], int argument, int is_background);
void bg_process_finished();
void store_process();

#endif