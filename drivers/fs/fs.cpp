#include <stdint.h>
#include "fs.hpp"

#define ATA_IO_BASE     0x1F0
#define ATA_REG_DATA    0x00
#define ATA_REG_ERROR   0x01
#define ATA_REG_SECCNT  0x02
#define ATA_REG_LBA0    0x03
#define ATA_REG_LBA1    0x04
#define ATA_REG_LBA2    0x05
#define ATA_REG_HDDEV   0x06
#define ATA_REG_COMMAND 0x07
#define ATA_REG_STATUS  0x07

static inline uint8_t inb(uint16_t port) {
    uint8_t r;
    asm volatile("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t r;
    asm volatile("inw %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

extern "C" {

int read_block(uint32_t lba, void* buffer) {
    // Wait until drive is not busy
    while (inb(ATA_IO_BASE + ATA_REG_STATUS) & 0x80) { }

    outb(ATA_IO_BASE + ATA_REG_HDDEV, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_IO_BASE + ATA_REG_SECCNT, 1);
    outb(ATA_IO_BASE + ATA_REG_LBA0,  lba & 0xFF);
    outb(ATA_IO_BASE + ATA_REG_LBA1, (lba >> 8)  & 0xFF);
    outb(ATA_IO_BASE + ATA_REG_LBA2, (lba >> 16) & 0xFF);
    outb(ATA_IO_BASE + ATA_REG_COMMAND, 0x20); // READ SECTORS

    // Wait for data ready
    while (!(inb(ATA_IO_BASE + ATA_REG_STATUS) & 0x08)) { }

    uint16_t* ptr = (uint16_t*)buffer;
    for (int i = 0; i < 256; i++) {
        *ptr++ = inw(ATA_IO_BASE + ATA_REG_DATA);
    }

    return 0;
}

int read_blocks(uint32_t lba, uint32_t count, void* buffer) {
    uint8_t* ptr = (uint8_t*)buffer;
    for (uint32_t i = 0; i < count; i++) {
        if (read_block(lba + i, ptr + i * BLOCK_SIZE) != 0)
            return 1;
    }
    return 0;
}

}

