#ifndef FS_H
#define FS_H

#define MAX_PROG_ROWS 20
#define MAX_PROG_COLS 50

void fs_run_into_buffer(const char* name, char buffer[MAX_PROG_ROWS][MAX_PROG_COLS], int rows, int cols);

#endif
