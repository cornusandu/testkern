// kernel_early_exit

void __printk();

// TODO: Fix this.
[[noreturn]] void kernel_early_exit() {
    char *p = "The kernel has exited early with a non-zero exit code.";
    asm volatile(
        "mov %0, %%edi\n\t"
        "mov %1, %%esi\n\t"
        "call __printk\n\t"
        :
        :"r"(p),"r"(54)
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
