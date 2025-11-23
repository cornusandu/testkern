section .text
global _boot
_boot:
    extern setup_stack
    call setup_stack

    ; Mask all interrupts on both PICs
    mov al, 0xFF
    out 0x21, al    ; Master PIC data port
    out 0xA1, al    ; Slave PIC data port

    ; Switch to text mode
    jmp skip_text  ; Temporarily disabled

    ;mov ax, 0x0003  ; Commented for testing
    ;int 0x10

    skip_text:

    cli

    extern kernel_main
    call kernel_main

    cmp eax, 0x1012
    jne .is_greater
    je .done
.is_greater:
    extern kernel_early_exit
    call kernel_early_exit
.done:
    jmp .hang
.hang:
    hlt
    jmp .hang