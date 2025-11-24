#include "../include/printk.hpp"
#include "../include/constants.h"
#include "../include/strlen.hpp"
#include <cstdint>

void kstd::printk(char* msg, uint32_t len) {
    asm volatile(
        "mov %0, %%edi\n\t"
        "mov %1, %%esi\n\t"
        "call __printk\n\t"
        :
        :"r"(msg),"r"(len)
        :"%edi","%esi","%eax","%ebx"
    );
}

extern "C" uint32_t vga_mem_offset;

void kstd::clearscrk() {
    for (int i = 0; i < 80*25; i++) {
        ((uint16_t*)vga)[i] = 0x0F20;  // white space on black
    }
    vga_mem_offset = 0;
}

void kstd::aprintk(char* msg) {
    kstd::printk(msg, kstd::strlen(msg));
};
