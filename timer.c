#include "timer.h"
#include "task.h"
#include "ports.h"
#include <stdint.h>

#define PIT_CMD 0x43
#define PIT_CHANNEL0 0x40

void timer_init() {
    // 100 Hz
    uint16_t divisor = 1193180 / 100;
    outb(PIT_CMD, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void timer_handler() {
    task_switch(); // switch task on each timer tick
}
