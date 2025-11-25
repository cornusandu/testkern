SECTIONS {
    . = 0;
    .exports : { *(.exports) }
    .text :    { *(.text) }
    .data :    { *(.data) }
}
