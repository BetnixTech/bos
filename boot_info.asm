
section .data
    msg db 'ASM Boot Info Example',0

section .text
    global _start
_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 20
    syscall

    mov rax, 60
    xor rdi, rdi
    syscall
