// kernel_early_exit

void __printk();

extern int vga_mem_offset;

// TODO: Fix this.
[[noreturn]] void kernel_early_exit() {
    asm volatile("movl $0, vga_mem_offset" ::: "memory");

    char *p = "The kernel has exited early with a non-zero exit code..\0\0";
    asm volatile(
        "mov %0, %%edi\n\t"
        "mov %1, %%esi\n\t"
        "call __printk\n\t"
        :
        :"r"(p),"r"(56)
        :"%edi","%esi","%eax","%ebx","memory"
    );

    asm volatile(
        "cli\n\t"
        "1:\n\t"
        "hlt\n\t"
        "jmp 1b\n\t"
    );

    __builtin_unreachable();
}
