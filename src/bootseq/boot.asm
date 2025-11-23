section .text
global _boot
_boot:
    ; Set up stack directly (no function call needed)
    extern stack_bottom
    lea esp, [stack_bottom - 4]  ; Point just below stack_bottom
    and esp, 0xFFFFFFF0          ; 16-byte align
    xor ebp, ebp

    ; Mask all interrupts on both PICs
    mov al, 0xFF
    out 0x21, al
    out 0xA1, al

    ; Switch to text mode
    jmp skip_text

    ;mov ax, 0x0003
    ;int 0x10

    skip_text:

    ;cli  ; Already in entrypoint.asm

    extern kernel_main
    call kernel_main

    cmp eax, 0x1012
    jne .is_greater
    je .done
.is_greater:
    extern kernel_early_exit
    call kernel_early_exit
    jmp .hang
.done:
    jmp .hang
.hang:
    hlt
    jmp .hang