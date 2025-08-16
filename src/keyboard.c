#include "ports.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

unsigned char keyboard_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*',0,' ',0
};

void keyboard_init() {
    // Placeholder if needed
}

char keyboard_read() {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    if(scancode & 0x80) return 0; // key release
    return keyboard_map[scancode];
}
