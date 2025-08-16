#include <stdint.h>

#define MAX_TASKS 8
#define STACK_SIZE 4096

typedef struct task {
    uint32_t esp;
    void (*func)(void);
    uint8_t stack[STACK_SIZE];
} task_t;

static task_t tasks[MAX_TASKS];
static int task_count = 0;
static int current_task = 0;

void task_add(void (*func)(void)) {
    if(task_count >= MAX_TASKS) return;
    task_t* t = &tasks[task_count++];
    t->func = func;
    t->esp = (uint32_t)&t->stack[STACK_SIZE-1];
}

void task_switch() {
    int prev = current_task;
    current_task = (current_task+1)%task_count;
    // simple context switch
    asm volatile (
        "mov %%esp, %0\n"
        "mov %1, %%esp\n"
        : "=m"(tasks[prev].esp)
        : "m"(tasks[current_task].esp)
    );
}
