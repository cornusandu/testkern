global _start

extern mount_toyfs
extern toyfs_find
extern toyfs_read
extern read_block
extern read_blocks

section .exports
    db "mount_toyfs", 0
    dd mount_toyfs
    db "toyfs_find", 0
    dd toyfs_find
    db "toyfs_read", 0
    dd toyfs_read
    db "read_block", 0
    dd read_block
    db "read_blocks", 0
    dd read_blocks
    db 0
