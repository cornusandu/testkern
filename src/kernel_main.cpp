#include <cstdint>
#include "../include/constants.h" // for vga memory address and setup_color
#include "../include/printk.hpp" // for printk

extern "C" void kernel_main()
{
    kstd::clearscrk();

    kstd::aprintk("Test!");

    asm volatile(
        "mov $0x1012, %%eax\n\t"
        :
        :
        :"%eax"
    );

    return;
};