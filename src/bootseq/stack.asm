section .stack
global stack_top
align 64
stack_top:
    times 16384 db 0       ; 16 KiB
global stack_bottom
stack_bottom: