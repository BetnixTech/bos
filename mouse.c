#include "ports.h"
#include <stdint.h>

static int mouse_x = 0;
static int mouse_y = 0;
static uint8_t buttons = 0;

void mouse_init() {
    outb(0x64, 0xD4);
    outb(0x60, 0xF4); // enable mouse
}

void mouse_update() {
    // read from 0x60 when mouse sends packet
    if(inb(0x64) & 1) {
        uint8_t packet = inb(0x60);
        buttons = packet & 0x07;
        int dx = (int8_t)packet; // relative movement
        int dy = (int8_t)packet;
        mouse_x += dx;
        mouse_y -= dy;
    }
}

int mouse_get_x() { return mouse_x; }
int mouse_get_y() { return mouse_y; }
int mouse_left_pressed() { return buttons & 1; }
int mouse_right_pressed() { return buttons & 2; }
