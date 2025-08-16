#include <stdbool.h>
#include <stddef.h>

extern void terminal_write(const char* str);
extern void terminal_clear(void);
extern char keyboard_read(void);

// Max command length
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
        terminal_write("  reboot - (not yet implemented)\n");
    }
    else if (!strcmp(cmd, "clear")) {
        terminal_clear();
    }
    else if (strncmp(cmd, "echo ", 5) == 0) {
        terminal_write("\n");
        terminal_write(cmd + 5);
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
