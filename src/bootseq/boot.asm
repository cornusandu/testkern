section .text
global _boot
align 16
_boot:
    ; Set up stack
    extern stack_bottom
    lea esp, [stack_bottom - 4]  ; Point just below stack_bottom
    and esp, 0xFFFFFFF0          ; 16-byte align

    xor ebp, ebp

    ; Arguments passed from _start to _boot are:
    ; [ESP+0] -> Return address to _start
    ; [ESP+4] -> EAX (Multiboot magic number)
    ; [ESP+8] -> EBX (Multiboot info structure physical address)

    ; Load Multiboot arguments into registers for later use
    mov edx, [esp + 4]   ; Load magic
    mov ecx, [esp + 8]   ; Load mbi_addr

    ; Call GDT setup (before paging for initial segment reload)
    extern setup_gdt
    call setup_gdt

    ; --- REMOVED: Paging setup calls ---
    ; extern identity_map_region_4mb
    ; push 0x2
    ; push 0x400000
    ; push 0x0
    ; call identity_map_region_4mb
    ; add esp, 12

    ; --- REMOVED: CR3 load and paging enable ---
    ; mov eax, pd
    ; mov cr3, eax
    ; mov eax, cr4
    ; or  eax, 0x10
    ; mov cr4, eax
    ; mov eax, cr0
    ; or  eax, 0x80000000
    ; mov cr0, eax

    ; Mask all interrupts on both PICs
    mov al, 0xFF
    out 0x21, al
    out 0xA1, al

    ; Switch to text mode (can also be done after paging, but before kernel_main)
    jmp skip_text

    ;mov ax, 0x0003
    ;int 0x10

    skip_text:

    ;cli  ; Already in entrypoint.asm

    ; Now call kernel_main with the Multiboot arguments (magic, mbi_addr)
    push ecx             ; Push mbi_addr for kernel_main (last arg for cdecl)
    push edx             ; Push magic for kernel_main (first arg for cdecl)
    extern kernel_main
    call kernel_main
    add esp, 8           ; Clean arguments for kernel_main

    cmp eax, 0x1012  ; Current standard exit code. Can be replaced later
    jne .is_greater
    jmp .done
.is_greater:
    extern kernel_early_exit
    call kernel_early_exit
    jmp .hang
.done:
    jmp .hang
.hang:
    hlt
    jmp .hang
    