#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (uint16_t*)0xB8000

static uint16_t* const vga_buffer = VGA_MEMORY;
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0F; // white on black

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0; // simple wrap, could implement scrolling
        return;
    }
    vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_write(const char* str) {
    while (*str)
        terminal_putchar(*str++);
}
