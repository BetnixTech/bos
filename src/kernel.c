#include "console.h"
#include "fs.h"
#include "keyboard.c"
#include "mouse.h"
#include "task.h"
#include "timer.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define MAX_WINDOWS 10
#define MAX_PROG_ROWS 20
#define MAX_PROG_COLS 80
#define CMD_BUF_SIZE 128

typedef struct {
    char title[32];
    char buffer[MAX_PROG_ROWS][MAX_PROG_COLS];
    int width, height;
    int x, y;
    int active;
} Window;

Window windows[MAX_WINDOWS];
int window_count = 0;
int active_window = 0;
int cursor_visible = 1;
char kb_buffer[CMD_BUF_SIZE];
size_t kb_len = 0;

// Forward declarations
void gui_init();
void gui_update();
void gui_handle_input();
void gui_handle_mouse();
void draw_window(Window* win);
void add_window(const char* name, int x, int y, int w, int h);

extern void shell_run();
extern void editor_run();

void kmain() {
    terminal_clear();
    terminal_write("Booting Betnix OS...\n");

    gui_init();
    mouse_init();
    task_init();
    timer_init();

    // Enable interrupts (PIT timer)
    asm volatile("sti");

    // Add GUI windows
    add_window("shell", 0,0,MAX_PROG_COLS,MAX_PROG_ROWS);
    add_window("editor",0,10,MAX_PROG_COLS,MAX_PROG_ROWS);
    active_window = 0;
    windows[0].active = 1;

    terminal_write("Boot complete!\n");

    // Add tasks
    task_add(shell_run, "shell");
    task_add(editor_run, "editor");

    // Main loop
    while(1) {
        keyboard_update();
        mouse_update();
        gui_handle_input();
        gui_handle_mouse();
        gui_update();
        task_run(); // run first task manually
    }
}

// GUI functions
void gui_init() { terminal_clear(); }

void draw_window(Window* win) {
    terminal_write("\n");
    terminal_write(win->title);
    terminal_write("\n-------------------\n");
    for(int r=0;r<win->height;r++) {
        for(int c=0;c<win->width;c++) {
            char ch = win->buffer[r][c];
            if(!ch) ch=' ';
            char str[2]={ch,0};
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

    if(cursor_visible) {
        Window* win = &windows[active_window];
        int row = win->height-1;
        int col = kb_len;
        terminal_move_cursor(row,col);
        terminal_write("_");
    }
}

void gui_handle_input() {
    char c=keyboard_read();
    if(c) {
        Window* win=&windows[active_window];
        if(c=='\b') {
            if(kb_len>0) kb_len--;
            int row=win->height-1;
            for(int i=0;i<MAX_PROG_COLS;i++)
                win->buffer[row][i]=(i<kb_len)?kb_buffer[i]:' ';
        }
        else if(c=='\n') kb_len=0;
        else if(kb_len<MAX_PROG_COLS-1) {
            kb_buffer[kb_len++]=c;
            kb_buffer[kb_len]=0;
            int row=win->height-1;
            for(int i=0;i<kb_len;i++) win->buffer[row][i]=kb_buffer[i];
        }
    }
}

void gui_handle_mouse() {
    int mx = mouse_get_x();
    int my = mouse_get_y();

    // Check clicks to change active window
    if(mouse_left_pressed()) {
        for(int i=0;i<window_count;i++) {
            Window* win = &windows[i];
            if(mx >= win->x && mx < win->x+win->width &&
               my >= win->y && my < win->y+win->height) {
                   active_window = i;
                   win->active = 1;
               } else {
                   win->active = 0;
               }
        }
    }

    // Drag active window
    static int drag_offset_x = 0;
    static int drag_offset_y = 0;
    static int dragging = 0;

    Window* win = &windows[active_window];
    if(mouse_left_pressed() && mx >= win->x && mx < win->x+win->width &&
       my >= win->y && my < win->y+win->height) {
           if(!dragging) {
               drag_offset_x = mx - win->x;
               drag_offset_y = my - win->y;
               dragging = 1;
           }
           win->x = mx - drag_offset_x;
           win->y = my - drag_offset_y;
    } else {
        dragging = 0;
    }
}

void add_window(const char* name,int x,int y,int w,int h) {
    if(window_count>=MAX_WINDOWS) return;
    Window* win=&windows[window_count++];
    strncpy(win->title,name,31);
    win->width=w;
    win->height=h;
    win->x=x;
    win->y=y;
    win->active=0;
    fs_run_into_buffer(name,win->buffer,w,h);
}
