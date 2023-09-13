#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include "shell.h"
#include "history.h"

void rawModeInput(char c, char* inp, int pt)
{
    int len=0;
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

            else if (c == 4) exit(0); // Ctrl+D (EOF)

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
}