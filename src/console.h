#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include <stdint.h>

// Clear the screen
void terminal_clear(void);

// Print a single character
void terminal_putchar(char c);

// Print a null-terminated string
void terminal_write(const char* str);

#endif
