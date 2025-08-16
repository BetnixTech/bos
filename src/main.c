#include "console.h"
#include "gui.c"

void kmain() {
    terminal_clear();
    terminal_write("Booting Betnix OS...\n");
    gui_run();
}
