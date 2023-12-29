#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <signal.h>
#include "../include/shell.h"
#include "../include/history.h"

// Function to print out the error message and exit with value 1
void die(const char *s){
    perror(s);
    exit(1);
}

void disableRawMode(){
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1){
        die("tcsetattr");
    }
}

void enableRawMode(){
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void rawModeInput(char c, char* inp, int pt){
    int len=0;
    enableRawMode();

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (iscntrl(c)) {
            if (c == 10) {
                printf("\n");
                break;
            }

            // Handling all the arrow keys
            else if (c == '\x1b') {
                char seq[3];
                if (read(STDIN_FILENO, seq, sizeof(seq)) == -1) {
                    perror("read");
                    break;
                }

                if (seq[0] == '[' && seq[1] == 'A') { //Up arrow key
                    printf("\x1b[%dD", pt+1);
                    printf("\x1b[K");
                    previousCommand(inp, &pt);
                    len = pt;
                } 

                else if (seq[0] == '[' && seq[1] == 'B') { //Down arrow key
                    printf("\x1b[%dD", pt+1);
                    printf("\x1b[K");
                    nextCommand(inp, &pt);
                    len = pt;
                }

                else if (seq[1] == 'C' && pt<len) { // Right arrow key pressed
                    printf("\x1b[C");
                    pt++;
                } 

                else if (seq[1] == 'D' && pt>0) { // Left arrow key pressed
                    printf("\x1b[D");
                    pt--;
                }
            }

            else if (c == 127) { // Backspace
                if (pt > 0) {
                    if (inp[pt-1] == 9) { // if the last character was a tab
                        for (int i = 0; i < 3; i++) {
                            printf("\b");
                        }
                    }
                    printf("\b \b");
                    inp[--pt] = '\0';
                }
            }

            else if (c == '\t') { // TAB character
                inp[pt++] = c;
                for (int i = 0; i < 4; i++) { // TABS should be 4 spaces
                    printf(" ");
                }
            } 

            else if (c == 4 || c==26) { // Ctrl+D (EOF)
                for(int i=0; i<process_count; i++){
                    if(!process_buffer[i].already_exitted){
                        kill(-process_buffer[i].pid, SIGKILL);
                    }
                }
                disableRawMode();
                exit(0); 
            }

            //else if(c == 3)

            else {
                printf("%d\n", c);
            }
        } 

        else { //no control character
            inp[pt++] = c;
            len = pt;
            printf("%c", c);
        }
    }
    disableRawMode();
}