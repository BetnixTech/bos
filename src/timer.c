#include "ports.h"
#include "task.h"

#define PIT_CMD 0x43
#define PIT_CH0 0x40

// Initialize PIT to fire IRQ0 at ~100Hz
void timer_init() {
    uint32_t freq = 100;            // 100 interrupts per second
    uint16_t divisor = 1193180 / freq;

    outb(PIT_CMD, 0x36);             // Command byte
    outb(PIT_CH0, divisor & 0xFF);   // Low byte
    outb(PIT_CH0, (divisor >> 8) & 0xFF); // High byte
}

// This is called on IRQ0
void timer_handler() {
    task_switch(); // preemptive multitasking
}
