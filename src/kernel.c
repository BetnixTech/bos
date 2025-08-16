extern void terminal_clear(void);
extern void terminal_write(const char* str);

void kmain(void) {
    terminal_clear();
    terminal_write("Betnix OS Booted!\n");
    terminal_write("Welcome to your new OS.\n");
}
