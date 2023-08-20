#ifndef PATH_HANDLING_H
#define PATH_HANDLING_H

char* relativePath(char* home_dir, char* cwd);
void changeDirectory(char** command_string, int arguments);

#endif