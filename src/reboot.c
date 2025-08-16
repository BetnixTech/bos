#include "ports.h"

void os_reboot(void) {
    // Wait until keyboard controller input buffer is empty
    while ((inb(KEYBOARD_CTRL) & 0x02) != 0) { }

    // Send pulse reset command to keyboard controller
    outb(KEYBOARD_CTRL, 0xFE);

    // If reboot fails, hang the CPU
    while (1) { }
}
