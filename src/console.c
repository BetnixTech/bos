#include "console.h"
#include <stdint.h>

uint16_t* video_memory = (uint16_t*)0xB8000;
int terminal_row = 0;
int terminal_col = 0;

void terminal_clear() {
    for(int y=0;y<25;y++)
        for(int x=0;x<80;x++)
            video_memory[y*80+x] = (0x0F << 8) | ' ';
    terminal_row = terminal_col = 0;
}

void terminal_write(const char* str) {
    while(*str) {
        if(*str=='\n') { terminal_col=0; terminal_row++; }
        else video_memory[terminal_row*80+terminal_col++] = (0x0F<<8)|*str;
        str++;
    }
}
