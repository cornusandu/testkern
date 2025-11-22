set -e

nasm -f elf32 ./src/entrypoint.asm -o entrypoint.o
nasm -f elf32 ./src/bootseq/boot.asm -o boot.o
nasm -f elf32 ./src/constants.asm -o constants.o
nasm -f elf32 ./src/bootseq/stack.asm -o stack.o

# C++ file — this is the only line that matters
g++ -m32 -march=i386 -mtune=i386 -ffreestanding -nostdlib -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-pic -fno-PIE -c src/kernel_main.cpp -o kernel_main.o

# C file
gcc -m32 -march=i386 -mtune=i386 -ffreestanding -nostdlib -fno-asynchronous-unwind-tables -c src/KEE.c -o KEE.o

ld -m elf_i386 -T src/linker.ld \
    entrypoint.o boot.o stack.o constants.o kernel_main.o KEE.o \
    -o iso/boot/kernel.elf --entry=_start

grub-mkrescue -o kernel.iso iso/
