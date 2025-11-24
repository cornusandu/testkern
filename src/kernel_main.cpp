#include <cstdint>
#include "../include/constants.h" // for vga memory address and setup_color
#include "../include/printk.hpp" // for printk

extern "C" void kernel_main()
{
    // Clear screen
    //volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    

    //const char* msg = "Hello, World! It finally works!";
    //for (int i = 0; msg[i]; i++)
    //    vga[i] = 0x0F00 | msg[i];

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