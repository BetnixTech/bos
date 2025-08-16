#include <string.h>
#include "fs.h"

void game_program(char output[MAX_PROG_ROWS][MAX_PROG_COLS], int rows, int cols) {
    const char* msg="Mini Game Loaded!";
    for(int i=0;i<strlen(msg);i++) output[0][i]=msg[i];
}
