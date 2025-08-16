#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "console.h"
#include "keyboard.h"
#include "fs.h"

#define CMD_BUF_SIZE 128

static char cmd_buffer[CMD_BUF_SIZE];
static size_t cmd_len = 0;

void shell_prompt() {
    terminal_write("\nBetnix> ");
}

void shell_execute(const char* cmd) {
    if (cmd[0] == 0) return;

    if (!strcmp(cmd, "help")) {
        terminal_write("\nCommands:\n");
        terminal_write("  help   - Show this help message\n");
        terminal_write("  clear  - Clear the screen\n");
        terminal_write("  echo   - Echo back text\n");
        terminal_write("  ls     - List files on disk\n");
        terminal_write("  run    - Run a program\n");
    }
    else if (!strcmp(cmd, "clear")) {
        terminal_clear();
    }
    else if (strncmp(cmd, "echo ", 5) == 0) {
        terminal_write("\n");
        terminal_write(cmd + 5);
    }
    else if (!strcmp(cmd, "ls")) {
        fs_list();
    }
    else if (strncmp(cmd, "run ", 4) == 0) {
        const char* name = cmd + 4;
        fs_run(name);
    }
    else {
        terminal_write("\nUnknown command: ");
        terminal_write(cmd);
    }
}

void shell_run() {
    shell_prompt();

    while (1) {
        char c = keyboard_read();
        if (c) {
            if (c == '\n') {
                cmd_buffer[cmd_len] = 0;
                shell_execute(cmd_buffer);
                cmd_len = 0;
                shell_prompt();
            } else if (c == '\b') {
                if (cmd_len > 0) cmd_len--;
            } else {
                if (cmd_len < CMD_BUF_SIZE - 1) {
                    cmd_buffer[cmd_len++] = c;
                    char str[2] = {c, 0};
                    terminal_write(str);
                }
            }
        }
    }
}
