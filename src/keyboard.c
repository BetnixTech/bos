#include <stdint.h>
#include "ports.h"

// Basic US QWERTY scancode map
static char keymap[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',   0, '\\',
    'z','x','c','v','b','n','m',',','.','/',   0,   '*', 0, ' '
};

char keyboard_read(void) {
    uint8_t status = inb(KEYBOARD_CTRL);
    if (status & 0x01) {
        uint8_t scancode = inb(0x60);
        if (scancode < 128) {
            return keymap[scancode];
        }
    }
    return 0;
}
