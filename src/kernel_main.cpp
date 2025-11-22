#include <cstdint>
#include "../include/constants.h" // for vga memory address and setup_color

extern "C" void kernel_main()
{
    // Clear screen
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    for (int i = 0; i < 80*25; i++)
        vga[i] = 0x0F20;  // white space on black

    const char* msg = "Hello, World! It finally works!";
    for (int i = 0; msg[i]; i++)
        vga[i] = 0x0F00 | msg[i];

    for (;;) asm volatile ("hlt");
}