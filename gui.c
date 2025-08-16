#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "console.h"
#include "fs.h"

// Screen settings
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

// Mouse
int mouse_x = 100;
int mouse_y = 100;
bool mouse_pressed = false;

// Simple framebuffer pointer (example)
uint32_t* framebuffer = (uint32_t*)0xB8000;

// Put a pixel on the screen
void put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    framebuffer[y * SCREEN_WIDTH + x] = color;
}

// Draw a filled rectangle
void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            put_pixel(x + i, y + j, color);
}

// Draw a filled circle
void draw_circle(int cx, int cy, int r, uint32_t color) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                put_pixel(cx + x, cy + y, color);
            }
        }
    }
}

// Draw window with title
void draw_window(int x, int y, int w, int h, const char* title) {
    draw_rect(x, y, w, h, 0xAAAAAA);      // Window background
    draw_rect(x, y, w, 20, 0x555555);     // Title bar
    terminal_write(title);                 // For now, title text in terminal
}

// Draw desktop background
void draw_desktop() {
    draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000088); // dark blue
}

// Draw mouse cursor
void draw_cursor() {
    draw_circle(mouse_x, mouse_y, 5, 0xFFFFFF); // white circle cursor
}

// Draw a simple icon
void draw_icon(int x, int y, const char* label) {
    draw_rect(x, y, 40, 40, 0x888888); // icon background
    terminal_write(label);              // For now, label printed in terminal
}

// Check if mouse is over a rectangle
bool mouse_over(int x, int y, int w, int h) {
    return mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h;
}

// GUI main loop
void gui_run() {
    draw_desktop();
    
    // Example icon
    int icon_x = 50, icon_y = 50;
    draw_icon(icon_x, icon_y, "example");

    while (1) {
        // Update mouse here (from keyboard/mouse driver in real OS)
        // For now, assume mouse_x/mouse_y are updated externally
        
        draw_desktop();
        draw_icon(icon_x, icon_y, "example");
        draw_window(100, 100, 200, 150, "Demo Window");
        draw_cursor();

        // Check click on icon
        if (mouse_pressed && mouse_over(icon_x, icon_y, 40, 40)) {
            fs_run("example"); // Run example.bns program
            mouse_pressed = false;
        }
    }
}
