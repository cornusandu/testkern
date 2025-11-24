section .stack
align 16
global stack_top
stack_top:
    times 16384 db 0       ; 16 KiB
global stack_bottom
stack_bottom: