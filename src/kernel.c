// src/kernel.c
#include "console.h"
#include "fs.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// Maximum windows
#define MAX_WINDOWS 10

// Window structure
typedef struct {
    char title[32];
    char buffer[MAX_PROG_ROWS][MAX_PROG_COLS];
    int width, height;
    int x, y;
    int active;
} Window;

// Global window list
Window windows[MAX_WINDOWS];
int window_count = 0;
int active_window = 0;

// Cursor
int mouse_x = 10, mouse_y = 5;
int cursor_visible = 1;

// Keyboard buffer
char kb_buffer[CMD_BUF_SIZE];
size_t kb_len = 0;

// Forward declarations
void gui_init();
void gui_update();
void gui_handle_input();
char keyboard_read();
void mouse_update();

// Add a program window
void add_window(const char* name, int x, int y, int w, int h) {
    if(window_count >= MAX_WINDOWS) return;
    Window* win = &windows[window_count++];
    strncpy(win->title, name, 31);
    win->width = w;
    win->height = h;
    win->x = x;
    win->y = y;
    win->active = 0;
    fs_run_into_buffer(name, win->buffer, w, h);
}

// Kernel entry
void kmain() {
    terminal_clear();
    terminal_write("Booting Betnix OS...\n");

    gui_init();

    // Add example windows
    add_window("shell", 0, 0, MAX_PROG_ROWS, MAX_PROG_COLS);
    add_window("editor", 0, 10, MAX_PROG_ROWS, MAX_PROG_COLS);

    // Make first window active
    active_window = 0;
    windows[0].active = 1;

    // Main kernel loop
    while(1) {
        gui_update();        // Draw windows + cursor
        gui_handle_input();  // Route keyboard + mouse
        mouse_update();      // Update cursor position
    }
}

// --- GUI functions ---
void gui_init() {
    // Clear screen
    terminal_clear();
}

void draw_window(Window* win) {
    terminal_write("\n"); // start new line
    terminal_write(win->title);
    terminal_write("\n-------------------\n");
    for(int r=0;r<win->height;r++) {
        for(int c=0;c<win->width;c++) {
            char ch = win->buffer[r][c];
            if(!ch) ch=' ';
            char str[2] = {ch, 0};
            terminal_write(str);
        }
        terminal_write("\n");
    }
    terminal_write("-------------------\n");
}

void gui_update() {
    terminal_clear();
    for(int i=0;i<window_count;i++)
        draw_window(&windows[i]);

    // Draw cursor
    if(cursor_visible) {
        terminal_write("O"); // simple circular cursor
    }
}

void gui_handle_input() {
    char c = keyboard_read();
    if(c) {
        Window* win = &windows[active_window];
        if(kb_len < MAX_PROG_COLS-1) {
            kb_buffer[kb_len++] = c;
            kb_buffer[kb_len] = 0;
            // Echo typed character into program buffer last line
            int row = win->height-1;
            int col = 0;
            for(int i=0;i<kb_len;i++) win->buffer[row][i] = kb_buffer[i];
        }
        if(c=='\n') kb_len=0; // reset line
    }
}

void mouse_update() {
    // Placeholder: just moves cursor diagonally for now
    mouse_x = (mouse_x+1)%80;
    mouse_y = (mouse_y+1)%25;
}

// --- Keyboard input ---
char keyboard_read() {
    // Placeholder: return 0 until real keyboard driver implemented
    return 0;
}
