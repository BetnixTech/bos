#include "fs.h"
#include "console.h"
#include <stdio.h>
#include <string.h>

static fs_file_entry_t files[FS_MAX_FILES];
static int file_count = 0;
static FILE* disk = NULL;

void fs_init(void) {
    file_count = 0;
    disk = NULL;
}

int fs_load_disk(const char* disk_path) {
    disk = fopen(disk_path, "rb");
    if (!disk) {
        terminal_write("Failed to open disk file\n");
        return -1;
    }

    fread(&file_count, sizeof(int), 1, disk);
    if (file_count > FS_MAX_FILES) file_count = FS_MAX_FILES;
    fread(files, sizeof(fs_file_entry_t), file_count, disk);

    return 0;
}

fs_file_entry_t* fs_find(const char* name) {
    for (int i = 0; i < file_count; i++) {
        if (strncmp(files[i].name, name, FS_MAX_FILENAME) == 0)
            return &files[i];
    }
    return NULL;
}

void fs_list(void) {
    terminal_write("\nFiles on disk:\n");
    for (int i = 0; i < file_count; i++) {
        terminal_write("  ");
        terminal_write(files[i].name);
        terminal_write("\n");
    }
}

void fs_run(const char* name) {
    if (!disk) {
        terminal_write("Disk not loaded\n");
        return;
    }

    fs_file_entry_t* file = fs_find(name);
    if (!file) {
        terminal_write("File not found: ");
        terminal_write(name);
        terminal_write("\n");
        return;
    }

    // Read program into memory (simplified: function pointer)
    void (*prog)(void);
    fseek(disk, file->offset, SEEK_SET);
    fread(&prog, sizeof(void*), 1, disk);
    terminal_write("Running ");
    terminal_write(name);
    terminal_write("...\n");
    prog();
}
