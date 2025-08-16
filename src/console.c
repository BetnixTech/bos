#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEM 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80

static uint16_t* vmem = (uint16_t*)VIDEO_MEM;
static size_t cursor_row = 0;
static size_t cursor_col = 0;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void terminal_clear() {
    for(size_t r=0;r<MAX_ROWS;r++)
        for(size_t c=0;c<MAX_COLS;c++)
            vmem[r*MAX_COLS + c] = vga_entry(' ', 0x07);
    cursor_row = 0;
    cursor_col = 0;
}

void terminal_put_char(char c) {
    if(c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if(c == '\b') {
        if(cursor_col>0) cursor_col--;
        vmem[cursor_row*MAX_COLS + cursor_col] = vga_entry(' ',0x07);
    } else {
        vmem[cursor_row*MAX_COLS + cursor_col] = vga_entry(c,0x07);
        cursor_col++;
        if(cursor_col >= MAX_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if(cursor_row >= MAX_ROWS) {
        // Scroll up
        for(size_t r=1;r<MAX_ROWS;r++)
            for(size_t c=0;c<MAX_COLS;c++)
                vmem[(r-1)*MAX_COLS + c] = vmem[r*MAX_COLS + c];
        for(size_t c=0;c<MAX_COLS;c++)
            vmem[(MAX_ROWS-1)*MAX_COLS + c] = vga_entry(' ',0x07);
        cursor_row = MAX_ROWS-1;
    }
}
