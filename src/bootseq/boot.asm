section .text
global _boot
_boot:
    extern setup_stack
    call setup_stack

    ; 1. Mask ALL legacy PIC interrupts (this is the missing line)
    mov al, 0x34          ; disable speaker + set channel 0 to mode 2
    out 0x43, al
    xor al, al
    out 0x40, al          ; timer divisor = 0 → timer stops
    out 0x40, al

    ; 2. Switch to text mode
    jmp skip_text  ; Temporarily disabled

    ;mov ax, 0x0003  ; Commented for testing
    ;int 0x10

    skip_text:

    extern kernel_main
    call kernel_main

    cmp eax, 0x1012
    jne .is_greater
    je .done
.is_greater:
    extern kernel_early_exit
    call kernel_early_exit
.done:
    cli
.hang:
    hlt
    jmp .hang