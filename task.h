#ifndef TASK_H
#define TASK_H

typedef struct {
    void (*entry)();
    int active;
    int pid;
    char name[32];
    uint32_t esp; // stack pointer
    uint32_t ebp; // base pointer
} Task;

void task_add(void (*entry)(), const char* name);
void task_init();
void task_switch();
void task_run();
void task_yield(); // manual yield if needed

#endif
