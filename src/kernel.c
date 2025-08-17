// Phase 2 kernel: boot -> init subsystems -> run shell + GUI tasks.

#include <stdint.h>

// Core subsystems
#include "console.h"
#include "irq.h"       // pic_remap(), irq_install() or idt_init() depending on your layout
#include "paging.h"    // paging_init()
#include "kheap.h"     // kheap_init()
#include "syscall.h"   // syscall_init()
#include "timer.h"     // timer_init(), timer_handler() called by IRQ0
#include "task.h"      // task_init(), task_add(), scheduler/ctx switch

// Drivers
#include "keyboard.h"  // keyboard_init()
#include "mouse.h"     // mouse_init()
#include "ata.h"       // ata_init()  (optional now; safe to call)

// Userland-ish services
#include "fs.h"        // fs_init()    (ramfs stub ok)
#include "shell.h"     // shell_run()  (task entry)
#include "gui.h"       // gui_run()    (task entry)

// If you don’t have these headers yet, create minimal ones that declare the functions used.
// (You already made paging.h, kheap.h, irq.h, gui.h earlier.)

static void banner(void) {
    terminal_clear();
    terminal_write("Betnix OS — Phase 2 kernel\n");
    terminal_write("Initializing subsystems...\n");
}

void kmain(void) {
    banner();

    // 1) Remap PIC and load IDT/ISRs so hardware IRQs don’t collide with CPU exceptions
    //    If your project uses idt_init()/isr_install(), call that here instead of irq_install().
    pic_remap(0x20, 0x28);     // IRQs -> 0x20..0x2F
    irq_install();             // set IDT entries & default ISRs/IRQs (your isr/irq glue should provide this)

    // 2) Enable paging (identity-map low memory so existing pointers still work)
    paging_init();

    // 3) Kernel heap (choose a region you don’t otherwise use; 16MB..17MB is common in QEMU)
    kheap_init((void*)0x01000000, 1 * 1024 * 1024); // 1 MiB heap at 16 MiB

    // 4) Optional early services
    fs_init();                 // safe stub if you don’t have a real FS yet
    ata_init();                // safe to call; can be a no-op if not wired

    // 5) Syscalls (int 0x80) — requires an IDT gate in irq/isr setup
    syscall_init();

    // 6) Drivers that raise interrupts
    keyboard_init();           // uses IRQ1
    mouse_init();              // uses IRQ12 (ensure your IRQ stubs cover it)

    // 7) Tasking + timer (preemptive): timer IRQ0 will call scheduler each tick
    task_init();
    timer_init();              // PIT @ ~100 Hz

    // 8) Spawn initial tasks (cooperative entries; preemptive switch happens via IRQ0)
    task_add(shell_run);       // classic text shell task
    task_add(gui_run);         // GUI main loop task (mouse/keyboard routed there too)

    // 9) Enable interrupts globally
    __asm__ __volatile__("sti");

    terminal_write("Init complete. Launching tasks...\n");

    // Idle loop — the scheduler will run tasks on each timer tick
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
