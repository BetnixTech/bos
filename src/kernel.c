#include "console.h"
#include "fs.h"
#include "shell.h"

void kernel_main() {
    terminal_clear();
    terminal_write("Betnix OS Booted!\n");

    fs_init();
    fs_load_disk("disk.bin");  // Load programs from disk

    shell_run();
}
