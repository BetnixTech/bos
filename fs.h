#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stddef.h>

#define FS_MAX_FILENAME 32
#define FS_MAX_FILES 16
#define FS_DISK_FILE "disk.bin"

// File entry structure
typedef struct {
    char name[FS_MAX_FILENAME];
    uint32_t offset;   // Offset in disk file
    uint32_t size;     // Size of program
} fs_file_entry_t;

void fs_init(void);
int fs_load_disk(const char* disk_path);
fs_file_entry_t* fs_find(const char* name);
void fs_list(void);
void fs_run(const char* name);

#endif
