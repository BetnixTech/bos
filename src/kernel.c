extern void terminal_clear(void);
extern void terminal_write(const char* str);
extern char keyboard_read(void);

void kmain(void) {
    terminal_clear();
    terminal_write("Betnix OS Booted!\nType something:\n");

    while (1) {
        char c = keyboard_read();
        if (c) {
            char str[2] = {c, 0};
            terminal_write(str);
        }
    }
}
