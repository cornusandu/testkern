section .data
    global vga_mem_offset
    vga_mem_offset: dd 0

section .text
global __printk
extern vga

; edi = char*
; esi = uint32_t
__printk:
    mov eax, 0  ; loop counter

.loop:
    cmp eax, esi
    jge .done

    mov ebx, [vga]
    add ebx, [vga_mem_offset]

    mov dl, [edi + eax]   ; character
    mov [ebx], dl

    mov dl, 0x0F  ; attribute (white on black)
    mov [ebx + 1], dl

    add dword [vga_mem_offset], 2  ; += 2

    inc eax
    jmp .loop

.done:
    ret
