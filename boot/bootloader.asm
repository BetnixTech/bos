; Simple x86 bootloader (512 bytes)
[org 0x7C00]
jmp start

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load kernel (assumes kernel.bin in second sector)
    mov si, 0x1000
    call load_kernel

    sti
    jmp 0x1000

load_kernel:
    ret ; placeholder
times 510-($-$$) db 0
dw 0xAA55
