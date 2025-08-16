#include "ports.h"
#include <stdint.h>

#define KEYBOARD_CTRL 0x64
#define KEYBOARD_DATA 0x60

static uint8_t shift_pressed = 0;

unsigned char keyboard_read() {
    unsigned char status = inb(KEYBOARD_CTRL);
    if (!(status & 0x01)) return 0; // no data
    unsigned char scancode = inb(KEYBOARD_DATA);

    // Shift pressed/released
    if(scancode == 0x2A || scancode == 0x36) shift_pressed = 1;
    else if(scancode == 0xAA || scancode == 0xB6) shift_pressed = 0;

    // Map scancode to ASCII
    if(scancode < 0x3A) {
        char c = "abcdefghijklmnopqrstuvwxyz1234567890"[scancode%0x3A];
        if(shift_pressed && c >= 'a' && c <= 'z') c -= 32;
        return c;
    }
    return 0;
}

