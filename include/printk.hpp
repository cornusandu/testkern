#pragma once

#include <cstdint>

extern "C" void __printk();

namespace kstd {
    void aprintk(char* msg);
    void printk(char* msg, uint32_t len);
    void clearscrk();
}
