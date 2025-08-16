#include "console.h" // assumes terminal_write is declared here

void sys_about(void) {
    terminal_write("\nBetnix OS - Bootable Demo");
    terminal_write("\nVersion: 0.1-alpha");
    terminal_write("\nAuthor: BetnixTech\n");
}
