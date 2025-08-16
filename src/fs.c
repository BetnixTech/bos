#include "fs.h"
#include <string.h>
#include "shell.c" // include programs directly for simplicity
#include "editor.c"
#include "game.c"

void fs_run_into_buffer(const char* name, char buffer[MAX_PROG_ROWS][MAX_PROG_COLS], int rows, int cols) {
    // clear buffer
    for(int r=0;r<rows;r++)
        for(int c=0;c<cols;c++)
            buffer[r][c]=0;

    if(strcmp(name,"shell")==0) shell_program(buffer,rows,cols);
    else if(strcmp(name,"editor")==0) editor_program(buffer,rows,cols);
    else if(strcmp(name,"game")==0) game_program(buffer,rows,cols);
    else {
        const char* msg = "Program not found!";
        for(int i=0;i<strlen(msg);i++) buffer[0][i] = msg[i];
    }
}
