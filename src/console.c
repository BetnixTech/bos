#include "console.h"
#include <stdio.h> // For now, print to host terminal

void terminal_write(const char* str) {
    printf("%s", str);
}

void terminal_clear(void) {
    printf("\033[2J\033[H"); // Clear screen ANSI escape
}
