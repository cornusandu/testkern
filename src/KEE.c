// kernel_early_exit

[[noreturn]] void kernel_early_exit() {
    asm volatile(
        "cli\n\t"
        "1:\n\t"
        "hlt\n\t"
        "jmp 1b\n\t"
    );

    __builtin_unreachable();
}
