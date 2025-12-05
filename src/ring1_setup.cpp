#include <cstdint>
#include "../include/printk.hpp" // For kstd::aprintk in error cases

// --- GDT and Ring 1 Setup ---

// GDT Descriptor Structure
struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// GDT Pointer Structure
struct GDTR {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Global GDT table.
#define NUM_GDT_ENTRIES 5 // Null, Kernel Code, Kernel Data, Ring 1 Code, Ring 1 Data
GDTEntry gdt[NUM_GDT_ENTRIES];
GDTR gdtr;

// Set up a GDT entry
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt[index].access = access;
}

// Access byte flags (DPL = Descriptor Privilege Level)
#define GDT_ACCESS_PRESENT      0x80 // Segment present (P bit)
#define GDT_ACCESS_RING0        0x00 // DPL=0
#define GDT_ACCESS_RING1        0x20 // DPL=1
#define GDT_ACCESS_RING2        0x40 // DPL=2
#define GDT_ACCESS_RING3        0x60 // DPL=3
#define GDT_ACCESS_CODE_SEG     0x18 // Code segment: Executable (E=1), Not Conforming (C=0), Read-only (R/W=0, or Read/Execute R/W=1), Accessed (A=0)
                                     // This is Type field (bits 0-3): 1010 for Read/Execute code (Type 0xA) or 1000 for Read-only code (Type 0x8)
                                     // For executable, 1 bit must be set in bit 3 (Executable bit).
                                     // For actual value: Type byte 0x9A (Read/Exec, A=0, E=1, R/W=1) for code.
                                     // Type byte 0x92 (R/W, A=0) for data.
#define GDT_ACCESS_DATA_SEG     0x10 // Data segment: Read/Write (E=0, C=0, R/W=1)
#define GDT_ACCESS_ACCESSED     0x01 // Accessed bit (A bit) - CPU sets this when segment is accessed
#define GDT_TYPE_CODE_RX        0x0A // Executable, Read/Execute
#define GDT_TYPE_DATA_RW        0x02 // Data, Read/Write

// Granularity flags
#define GDT_GRAN_4K             0x80 // Granularity bit (G=1)
#define GDT_GRAN_32BIT          0x40 // Default operand size (D/B=1 for 32-bit)

// Function to initialize GDT with basic entries
extern "C" void setup_gdt() {
    // 0: Null segment
    set_gdt_entry(0, 0, 0, 0, 0);

    // 1: Kernel Code Segment (Ring 0) - Selector 0x08
    set_gdt_entry(1, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEG | GDT_TYPE_CODE_RX | GDT_ACCESS_ACCESSED,
                  GDT_GRAN_4K | GDT_GRAN_32BIT);

    // 2: Kernel Data Segment (Ring 0) - Selector 0x10
    set_gdt_entry(2, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEG | GDT_TYPE_DATA_RW | GDT_ACCESS_ACCESSED,
                  GDT_GRAN_4K | GDT_GRAN_32BIT);

    // 3: Ring 1 Code Segment (DPL=1) - Selector 0x18
    set_gdt_entry(3, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_RING1 | GDT_ACCESS_CODE_SEG | GDT_TYPE_CODE_RX | GDT_ACCESS_ACCESSED,
                  GDT_GRAN_4K | GDT_GRAN_32BIT);

    // 4: Ring 1 Data Segment (DPL=1) - Selector 0x20
    set_gdt_entry(4, 0, 0xFFFFFFFF,
                  GDT_ACCESS_PRESENT | GDT_ACCESS_RING1 | GDT_ACCESS_DATA_SEG | GDT_TYPE_DATA_RW | GDT_ACCESS_ACCESSED,
                  GDT_GRAN_4K | GDT_GRAN_32BIT);

    gdtr.limit = (sizeof(GDTEntry) * NUM_GDT_ENTRIES) - 1;
    gdtr.base = reinterpret_cast<uint32_t>(&gdt);

    // Load GDTR
    asm volatile("lgdt (%0)" : : "r" (&gdtr));

    // Reload segment registers to use the new GDT.
    // For CS, a far jump is required. We'll stick to ring 0 for now.
    // This reloads data segments.
    asm volatile (
        "mov $0x10, %ax     \n" // Selector for Kernel Data (index 2 * 8 = 0x10)
        "mov %ax, %ds       \n"
        "mov %ax, %es       \n"
        "mov %ax, %fs       \n"
        "mov %ax, %gs       \n"
        "mov %ax, %ss       \n"
        // To reload CS, a far jump to the new code segment is needed.
        // E.g., "ljmp $0x08, $1f \n" "1: \n"
        // For this boot sequence, we assume we remain in Ring 0 code segment.
    );
}