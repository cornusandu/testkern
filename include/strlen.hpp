#pragma once

#include <cstdint>

namespace kstd {
    inline uint32_t strlen(char* msg) {
        char* i = msg;
        while (true) {
            if (*i == 0) {
                return i - msg;
            }
            i++;
        }
    }
}
