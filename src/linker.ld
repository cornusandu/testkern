ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .multiboot : { *(.multiboot) }
    .text      : { *(.text*) }
    .rodata    : { *(.rodata*) }
    .data      : { *(.data*) }

    /* THIS IS THE ONLY LINE THAT WAS MISSING */
    .stack     : { *(.stack) }

    .bss       : { *(.bss*) *(COMMON) }
}