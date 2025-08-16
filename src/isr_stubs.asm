[BITS 32]
global isr0
global isr1
global irq0
global irq1
global isr80
extern isr_handler

%macro ISR_STUB 1
    global %1
%1:
    pusha
    push ds
    push es
    pushad
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    push %1
    call isr_handler
    add esp, 4
    popad
    pop es
    pop ds
    popa
    iret
%endmacro

ISR_STUB isr0
ISR_STUB isr1
ISR_STUB irq0
ISR_STUB irq1

; syscall int 0x80
global isr80
isr80:
    pusha
    call isr_handler
    popa
    iret
