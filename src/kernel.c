#include <stdint.h>
#include "ports.h"
#include "paging.h"
#include "kheap.h"
#include "irq.h"
#include "timer.h"
#include "fs.h"
#include "shell.h"
#include "gui.h"
#include "task.h"
#include "keyboard.h"
#include "mouse.h"

extern void isr_install(); // sets IDT and handlers
extern void sti();         // enable interrupts

void kmain() {
    terminal_clear();

    // Initialize low-level subsystems
    pic_remap(0x20, 0x28);
    isr_install();
    paging_init();
    kheap_init((void*)0x01000000, 1024*1024); // 1MB heap
    ata_init();

    task_init();        // initialize tasking
    keyboard_init();
    mouse_init();
    timer_init();       // PIT timer, calls task_switch()

    sti();              // enable interrupts

    // Add initial tasks
    task_add(shell_run);   // shell task
    task_add(gui_run);     // GUI loop task

    while(1) {
        asm volatile("hlt"); // idle CPU until interrupt
    }
}
