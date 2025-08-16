#include "console.h"
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* const vga_buffer = (uint16_t*)VGA_ADDRESS;
static size_t terminal_row = 0;
static size_t terminal_col = 0;
static uint8_t terminal_color = 0x0F; // White on black

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        if (++terminal_row >= VGA_HEIGHT) terminal_row = 0;
        return;
    } else if (c == '\r') {
        terminal_col = 0;
        return;
    }

    const size_t index = terminal_row * VGA_WIDTH + terminal_col;
    vga_buffer[index] = ((uint16_t)terminal_color << 8) | c;

    if (++terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        if (++terminal_row >= VGA_HEIGHT) terminal_row = 0;
    }
}

void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}
