#include "keyboard.h"
#include <stdio.h>

char keyboard_read(void) {
    int c = getchar();
    if (c == EOF) return 0;
    return (char)c;
}
