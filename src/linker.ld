ENTRY(_start)

PHDRS {
    rwx PT_LOAD FLAGS(7);  /* Read + Write + Execute */
    ro PT_LOAD FLAGS(5);  /* Read + Execute */
}

SECTIONS
{
    . = 0x100000;

    .multiboot : { *(.multiboot) } :rwx
    .text      : { *(.text*) } :rwx
    .bss       : { *(.bss*) *(COMMON) } :rwx
    .data      : { *(.data*) } :rwx
    .stack     : { *(.stack*) } :rwx

    .rodata    : { *(.rodata*) } :ro
}