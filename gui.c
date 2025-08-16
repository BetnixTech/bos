#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include "fs.h"
#include "console.h"

// ------------------- Screen & Mouse -------------------
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define ICON_SIZE     40
#define ICON_SPACING  10
#define RESIZE_HANDLE 10
#define TITLE_HEIGHT  20
#define MIN_WIDTH     80
#define MIN_HEIGHT    60
#define MAX_PROG_ROWS 20
#define MAX_PROG_COLS 50

uint32_t* framebuffer = (uint32_t*)0xB8000;
int mouse_x = 100;
int mouse_y = 100;
bool mouse_pressed = false;

// ------------------- Drawing -------------------
void put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    framebuffer[y * SCREEN_WIDTH + x] = color;
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            put_pixel(x + i, y + j, color);
}

void draw_circle(int cx, int cy, int r, uint32_t color) {
    for (int y = -r; y <= r; y++)
        for (int x = -r; x <= r; x++)
            if (x*x + y*y <= r*r)
                put_pixel(cx + x, cy + y, color);
}

// ------------------- Desktop -------------------
void draw_desktop() {
    draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000088);
}

// ------------------- Icons -------------------
typedef struct {
    char name[64];
    int x, y;
} icon_t;

#define MAX_ICONS 64
icon_t icons[MAX_ICONS];
int icon_count = 0;

bool mouse_over(int x, int y, int w, int h) {
    return mouse_x >= x && mouse_x <= x + w &&
           mouse_y >= y && mouse_y <= y + h;
}

void draw_icon(icon_t* icon) {
    draw_rect(icon->x, icon->y, ICON_SIZE, ICON_SIZE, 0x888888);
    terminal_write(icon->name); // placeholder for label
}

int load_icons() {
    DIR* dir = opendir("disk");
    if (!dir) return 0;

    struct dirent* entry;
    int count = 0;
    int start_x = ICON_SPACING, start_y = ICON_SPACING;

    while ((entry = readdir(dir)) && count < MAX_ICONS) {
        if (strstr(entry->d_name, ".bns")) {
            strncpy(icons[count].name, entry->d_name, 63);
            icons[count].x = start_x;
            icons[count].y = start_y;

            start_x += ICON_SIZE + ICON_SPACING;
            if (start_x + ICON_SIZE > SCREEN_WIDTH) {
                start_x = ICON_SPACING;
                start_y += ICON_SIZE + ICON_SPACING;
            }
            count++;
        }
    }
    closedir(dir);
    return count;
}

void draw_icons_all() {
    for (int i = 0; i < icon_count; i++)
        draw_icon(&icons[i]);
}

// ------------------- Windows -------------------
typedef struct {
    char title[64];
    int x, y;
    int w, h;
    bool active;
    bool being_dragged;
    int drag_offset_x, drag_offset_y;
    bool resizing;
    int resize_start_w, resize_start_h;
    bool closing;

    // Program output buffer
    char content[MAX_PROG_ROWS][MAX_PROG_COLS];
    int rows, cols;
} window_t;

#define MAX_WINDOWS 16
window_t windows[MAX_WINDOWS];
int window_count = 0;

// Draw a window with content
void draw_window(window_t* win) {
    uint32_t bg_color = win->active ? 0xCCCCCC : 0xAAAAAA;
    uint32_t title_color = win->active ? 0x555555 : 0x333333;

    draw_rect(win->x, win->y, win->w, win->h, bg_color);             // body
    draw_rect(win->x, win->y, win->w, TITLE_HEIGHT, title_color);    // title
    draw_rect(win->x + win->w - RESIZE_HANDLE, win->y + win->h - RESIZE_HANDLE, RESIZE_HANDLE, RESIZE_HANDLE, 0xFF00FF); // resize handle
    draw_rect(win->x + win->w - TITLE_HEIGHT, win->y, TITLE_HEIGHT, TITLE_HEIGHT, 0xFF0000); // close button

    // Render program content
    int char_w = (win->w - 10)/MAX_PROG_COLS;
    int char_h = (win->h - TITLE_HEIGHT - 10)/MAX_PROG_ROWS;
    for (int r = 0; r < win->rows; r++) {
        for (int c = 0; c < win->cols; c++) {
            char ch = win->content[r][c];
            if (ch) {
                draw_rect(win->x + 5 + c*char_w, win->y + TITLE_HEIGHT + 5 + r*char_h, char_w-1, char_h-1, 0x000000); // black char block
            }
        }
    }
}

// Update window interactions
void update_windows() {
    for (int i = window_count-1; i >= 0; i--) {
        window_t* win = &windows[i];

        // Drag
        if (mouse_pressed && mouse_over(win->x, win->y, win->w - TITLE_HEIGHT, TITLE_HEIGHT)) {
            win->being_dragged = true;
            win->drag_offset_x = mouse_x - win->x;
            win->drag_offset_y = mouse_y - win->y;
            win->active = true;

            // Bring to top
            window_t temp = windows[i];
            for (int j = i; j < window_count-1; j++) windows[j] = windows[j+1];
            windows[window_count-1] = temp;
            break;
        }

        if (win->being_dragged) {
            if (!mouse_pressed) win->being_dragged = false;
            else {
                win->x = mouse_x - win->drag_offset_x;
                win->y = mouse_y - win->drag_offset_y;
            }
        }

        // Resize
        if (mouse_pressed && mouse_over(win->x + win->w - RESIZE_HANDLE, win->y + win->h - RESIZE_HANDLE, RESIZE_HANDLE, RESIZE_HANDLE)) {
            win->resizing = true;
            win->resize_start_w = win->w;
            win->resize_start_h = win->h;
            break;
        }

        if (win->resizing) {
            if (!mouse_pressed) win->resizing = false;
            else {
                int new_w = mouse_x - win->x;
                int new_h = mouse_y - win->y;
                if (new_w > MIN_WIDTH) win->w = new_w;
                if (new_h > MIN_HEIGHT) win->h = new_h;
            }
        }

        // Close button
        if (mouse_pressed && mouse_over(win->x + win->w - TITLE_HEIGHT, win->y, TITLE_HEIGHT, TITLE_HEIGHT)) {
            win->closing = true;
        }
    }

    // Remove closed windows
    for (int i = 0; i < window_count; i++) {
        if (windows[i].closing) {
            for (int j = i; j < window_count-1; j++)
                windows[j] = windows[j+1];
            window_count--;
            i--;
        }
    }
}

// Open a program window with buffer
void open_program_window(const char* name) {
    if (window_count >= MAX_WINDOWS) return;

    window_t* win = &windows[window_count++];
    strncpy(win->title, name, 63);
    win->x = 50 + window_count*20;
    win->y = 50 + window_count*20;
    win->w = 300;
    win->h = 200;
    win->active = true;
    win->being_dragged = false;
    win->resizing = false;
    win->closing = false;

    // Initialize program buffer
    win->rows = MAX_PROG_ROWS;
    win->cols = MAX_PROG_COLS;
    memset(win->content, 0, sizeof(win->content));

    // Run program into buffer
    fs_run_into_buffer(name, win->content, MAX_PROG_ROWS, MAX_PROG_COLS);
}

// ------------------- Mouse -------------------
void draw_cursor() {
    draw_circle(mouse_x, mouse_y, 5, 0xFFFFFF);
}

// ------------------- Main GUI Loop -------------------
void gui_run() {
    icon_count = load_icons();

    while (1) {
        draw_desktop();
        draw_icons_all();

        update_windows();
        for (int i = 0; i < window_count; i++)
            draw_window(&windows[i]);

        draw_cursor();

        // Icon click opens program
        if (mouse_pressed) {
            for (int i = 0; i < icon_count; i++) {
                if (mouse_over(icons[i].x, icons[i].y, ICON_SIZE, ICON_SIZE)) {
                    char name[64];
                    strncpy(name, icons[i].name, 63);
                    name[strlen(name)-4] = 0;
                    open_program_window(name);
                    mouse_pressed = false;
                    break;
                }
            }
        }

        // TODO: update mouse_x/mouse_y from driver
    }
}
