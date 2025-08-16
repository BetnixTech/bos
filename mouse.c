#include "mouse.h"
#include "ports.h"
#include <stdint.h>

#define MOUSE_DATA_PORT 0x60
#define MOUSE_STATUS_PORT 0x64

static int mouse_x = 40;
static int mouse_y = 12;
static int left_pressed = 0;
static int right_pressed = 0;

// Internal buffer
static uint8_t mouse_cycle = 0;
static int8_t mouse_bytes[3];

// Initialize mouse (PS/2)
void mouse_init() {
    outb(0x64, 0xA8); // enable auxiliary device - mouse
    outb(0x64, 0xD4); // next byte to mouse
    outb(0x60, 0xF4); // enable packet streaming
}

// Read mouse data and update coordinates & buttons
void mouse_update() {
    if(!(inb(MOUSE_STATUS_PORT) & 0x20)) return; // no data
    uint8_t data = inb(MOUSE_DATA_PORT);

    if(mouse_cycle == 0) {
        mouse_bytes[0] = data;
        mouse_cycle++;
    } else if(mouse_cycle == 1) {
        mouse_bytes[1] = data;
        mouse_cycle++;
    } else if(mouse_cycle == 2) {
        mouse_bytes[2] = data;
        mouse_cycle = 0;

        int x_move = (int8_t)mouse_bytes[1];
        int y_move = (int8_t)mouse_bytes[2];

        mouse_x += x_move;
        mouse_y -= y_move; // screen y inverted

        if(mouse_x < 0) mouse_x = 0;
        if(mouse_y < 0) mouse_y = 0;
        if(mouse_x > 79) mouse_x = 79;
        if(mouse_y > 24) mouse_y = 24;

        left_pressed = mouse_bytes[0] & 0x1;
        right_pressed = mouse_bytes[0] & 0x2;
    }
}

int mouse_get_x() { return mouse_x; }
int mouse_get_y() { return mouse_y; }
int mouse_left_pressed() { return left_pressed; }
int mouse_right_pressed() { return right_pressed; }
