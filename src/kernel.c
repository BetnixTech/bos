#include "console.h"

void kmain(void) {
    terminal_clear();
    terminal_write("Betnix OS: hello from kernel!\n");
    terminal_write("If you see this, GRUB loaded our kernel and we jumped to kmain.\n");
    for(;;) { __asm__ __volatile__("hlt"); }
}
