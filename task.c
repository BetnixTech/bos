#include <stddef.h>
#include "task.h"

#define MAX_TASKS 10
Task tasks[MAX_TASKS];
int task_count = 0;
int current_task = 0;

void task_add(void (*entry)(), const char* name) {
    if(task_count >= MAX_TASKS) return;
    Task* t = &tasks[task_count++];
    t->entry = entry;
    t->active = 1;
    t->pid = task_count;
    // copy name
    for(int i=0;i<31 && name[i];i++) t->name[i] = name[i];
    t->name[31] = 0;
}

void task_switch() {
    current_task = (current_task + 1) % task_count;
}

void task_run() {
    for(int i=0;i<task_count;i++)
        if(tasks[i].active && tasks[i].entry)
            tasks[i].entry();
}
