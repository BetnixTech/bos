.global task_switch_asm
task_switch_asm:
    pusha                  # save all registers
    mov  current_task, %eax
    mov  tasks(,%eax,16), %ebx
    mov  %esp, 4(%ebx)     # save esp
    mov  %ebp, 8(%ebx)     # save ebp

    # find next task
    mov current_task, %eax
    inc %eax
    cmp task_count, %eax
    jl 1f
    xor %eax,%eax
1:
    mov %eax, current_task
    mov tasks(,%eax,16), %ebx
    mov 4(%ebx), %esp     # restore esp
    mov 8(%ebx), %ebp     # restore ebp
    popa
    ret
