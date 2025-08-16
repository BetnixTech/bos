#include "fs.h"
#include "console.h"
#include "bns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fs_run(const char* name) {
    char path[64];
    snprintf(path, sizeof(path), "disk/%s.bns", name); // Store .bns in "disk/" folder
    FILE* f = fopen(path, "rb");
    if (!f) {
        terminal_write("File not found: ");
        terminal_write(name);
        terminal_write("\n");
        return;
    }

    bns_header_t header;
    fread(&header, sizeof(bns_header_t), 1, f);
    if (header.magic != BNS_MAGIC) {
        terminal_write("Invalid BNS file\n");
        fclose(f);
        return;
    }

    // Allocate memory for code
    void (*prog)(void);
    fread(&prog, sizeof(void*), 1, f);  // Simplified for now: pointer stored in file
    fclose(f);

    terminal_write("Running ");
    terminal_write(name);
    terminal_write("...\n");

    // Run program
    prog();
}
