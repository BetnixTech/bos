#include "console.h"
#include <stdint.h>

void sys_mem(void) {
    terminal_write("\nDetecting memory...\n");

    volatile uint32_t *ptr = (uint32_t*)0x100000; // Start at 1MB
    uint32_t mem_mb = 0;

    // Count memory in MB up to 16 MB (safe for QEMU demo)
    while ((uint32_t)ptr < 0x1000000) {
        *ptr = 0xA5A5A5A5;
        if (*ptr != 0xA5A5A5A5) break;
        mem_mb++;
        ptr += 1024 * 256; // step 1 MB (256k * 4 bytes = 1MB)
    }

    char buf[16];
    int len = 0;
    uint32_t tmp = mem_mb;

    // Convert number to string
    do {
        buf[len++] = '0' + (tmp % 10);
        tmp /= 10;
    } while (tmp);

    terminal_write("Approx RAM: ");
    for (int i = len-1; i >= 0; i--) terminal_write((char[]){buf[i],0});
    terminal_write(" MB\n");
}
