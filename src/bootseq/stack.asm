section .data
align 16
global stack_top
stack_top:
    times 16384 db 0       ; 16 KiB of actual zeros in the .data section
stack_bottom:

section .text
global setup_stack
setup_stack:
    mov esp, stack_bottom
    and esp, 0xFFFFFFF0   ; 16-byte align
    xor ebp, ebp
    ret