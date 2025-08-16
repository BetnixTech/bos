#include <string.h>
#include "fs.h"

void shell_program(char output[MAX_PROG_ROWS][MAX_PROG_COLS], int rows, int cols) {
    const char* msg="Welcome to Betnix Shell!";
    for(int i=0;i<strlen(msg);i++) output[0][i] = msg[i];

    const char* prompt="Betnix> ";
    for(int i=0;i<strlen(prompt);i++) output[1][i] = prompt[i];
}
