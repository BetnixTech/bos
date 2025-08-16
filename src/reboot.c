#include "ports.h"

void os_reboot(void) {
    // Wait until keyboard controller input buffer is empty
    while ((inb(KEYBOARD_CTRL) & 0x02) != 0) { }

    // Send 0xFE command to pulse CPU reset line
    outb(KEYBOARD_CTRL, 0xFE);

    // If reboot didn’t work, hang
    while (1) { }
}
