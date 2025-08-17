#include <stdint.h>
#include "console.h"

static volatile uint16_t* vga = (uint16_t*)0xB8000;
static int row = 0, col = 0;
static uint8_t color = 0x0F;

static void putc(char c) {
    if (c=='\n') { row++; col=0; return; }
    vga[row*80 + col] = ((uint16_t)color<<8) | (uint8_t)c;
    if(++col >= 80){ col=0; row++; }
}

void terminal_clear(void){
    for(int i=0;i<25*80;i++) vga[i] = ((uint16_t)color<<8) | ' ';
    row=0; col=0;
}

void terminal_write(const char* s){
    for(; *s; s++) putc(*s);
}
