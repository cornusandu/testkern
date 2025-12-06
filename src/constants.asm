section .rodata
    global monochrome_vga
    global vga
    align 4
    monochrome_vga: dd 0xB0000
    vga: dd 0xB8000

section .text
    global setup_color
setup_color:
    ret