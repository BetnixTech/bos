; src/task_switch.asm
; Implements context switching between tasks (NASM syntax)

global task_switch_asm

section .text
; void task_switch_asm(uint32_t** old_sp, uint32_t* new_sp)
; Saves old stack pointer, loads new one, and returns into new task
task_switch_asm:
    ; Save ESP of current task into *old_sp
    mov eax, [esp+4]      ; eax = &old_sp
    mov [eax], esp        ; *old_sp = current ESP

    ; Load new ESP
    mov eax, [esp+8]      ; eax = new_sp
    mov esp, eax

    ; Restore task registers
    popad                 ; restore general registers
    popfd                 ; restore EFLAGS
    ret
