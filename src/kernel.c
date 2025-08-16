#include "console.h"
#include "fs.h"
#include "keyboard.c"
#include "mouse.c"
#include "task.h"
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
void draw_window(Window* win);
void add_window(const char* name, int x, int y, int w, int h);

void kmain() {
    terminal_clear();
    terminal_write("Booting Betnix OS...\n");

    gui_init();

    add_window("shell", 0,0,MAX_PROG_ROWS,MAX_PROG_COLS);
    add_window("editor",0,10,MAX_PROG_ROWS,MAX_PROG_COLS);
    active_window=0;
    windows[0].active=1;

    terminal_write("Boot complete!\n");

    // Add shell & editor tasks
    task_add(shell_run, "shell");
    task_add(editor_run, "editor");

    while(1) {
        gui_update();
        gui_handle_input();
        mouse_update();
        task_run();
        task_switch();
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
