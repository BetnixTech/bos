#include "task.h"
#include <stdint.h>
#include <string.h>

#define MAX_TASKS 10
#define STACK_SIZE 1024  // 4KB stack per task

Task tasks[MAX_TASKS];
int task_count = 0;
int current_task = -1;

// Each task gets its own stack
uint32_t task_stacks[MAX_TASKS][STACK_SIZE];

// Add a new task
void task_add(void (*entry)(), const char* name) {
    if(task_count >= MAX_TASKS) return;

    Task* t = &tasks[task_count];
    t->entry = entry;
    t->active = 1;
    t->pid = task_count;
    strncpy(t->name, name, 31);
    t->name[31] = 0;

    // Set stack pointers to top of stack
    t->esp = (uint32_t)&task_stacks[task_count][STACK_SIZE - 1];
    t->ebp = t->esp;

    task_count++;
}

// Initialize task system
void task_init() {
    current_task = -1;
}

// Switch to the next task
extern void task_switch_asm(); // defined in assembly
void task_switch() {
    if(task_count == 0) return;

    int next_task = (current_task + 1) % task_count;
    current_task = next_task;
    task_switch_asm();
}

// Manually yield CPU to next task
void task_yield() {
    task_switch();
}

// Run first task manually (before PIT timer starts)
void task_run() {
    if(current_task == -1) current_task = 0;
    if(tasks[current_task].active && tasks[current_task].entry)
        tasks[current_task].entry();
}
