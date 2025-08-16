#include "task.h"
#include <stdint.h>

#define MAX_TASKS 10
Task tasks[MAX_TASKS];
int task_count = 0;
int current_task = -1;

// Assume each task has its own stack
uint32_t task_stacks[MAX_TASKS][1024]; // 4KB stack each

void task_add(void (*entry)(), const char* name) {
    if(task_count >= MAX_TASKS) return;
    Task* t = &tasks[task_count];
    t->entry = entry;
    t->active = 1;
    t->pid = task_count;
    for(int i=0;i<31 && name[i];i++) t->name[i] = name[i];
    t->name[31] = 0;

    // Set stack pointers to top of stack
    t->esp = (uint32_t)&task_stacks[task_count][1024];
    t->ebp = t->esp;

    task_count++;
}

extern void task_switch_asm(); // assembly context switch
void task_switch() {
    if(task_count==0) return;
    int next_task = (current_task + 1) % task_count;
    current_task = next_task;
    task_switch_asm();
}

void task_init() {
    current_task = -1;
}

void task_yield() {
    task_switch();
}

// Run first task manually
void task_run() {
    if(current_task == -1) current_task = 0;
    if(tasks[current_task].active && tasks[current_task].entry)
        tasks[current_task].entry();
}
