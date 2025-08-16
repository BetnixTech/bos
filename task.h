#ifndef TASK_H
#define TASK_H

typedef struct {
    void (*entry)();
    int active;
    int pid;
    char name[32];
} Task;

void task_add(void (*entry)(), const char* name);
void task_switch();
void task_run();

#endif
