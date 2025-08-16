#include <string.h>
#include "fs.h"

void editor_program(char output[MAX_PROG_ROWS][MAX_PROG_COLS], int rows, int cols) {
    const char* msg="Simple Text Editor";
    for(int i=0;i<strlen(msg);i++) output[0][i]=msg[i];

    const char* note="(Edit here)";
    for(int i=0;i<strlen(note);i++) output[1][i]=note[i];
}
