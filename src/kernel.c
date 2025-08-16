extern void terminal_clear(void);
extern void terminal_write(const char* str);
extern void shell_run(void);

void kmain(void) {
    terminal_clear();
    terminal_write("Betnix OS Booted!\nType 'help' for a list of commands.\n");
    shell_run();
}
