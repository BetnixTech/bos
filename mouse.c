#include "ports.h"
#include <stdint.h>

int mouse_x = 10, mouse_y = 5;
int mouse_btn_left = 0;

// Dummy mouse: simulate movement & click
void mouse_update() {
    mouse_x = (mouse_x + 1) % 80;
    mouse_y = (mouse_y + 1) % 25;
    mouse_btn_left = 0; // 1 if clicked, for now always 0
}

int mouse_get_x() { return mouse_x; }
int mouse_get_y() { return mouse_y; }
int mouse_left_pressed() { return mouse_btn_left; }
