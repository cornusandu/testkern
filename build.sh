set -e

nasm -f elf32 ./src/entrypoint.asm -o entrypoint.o
nasm -f elf32 ./src/bootseq/boot.asm -o boot.o
nasm -f elf32 ./src/constants.asm -o constants.o
nasm -f elf32 ./src/bootseq/stack.asm -o stack.o

g++ -m32 -march=i386 -mtune=generic \
    -O2 \
    -ffreestanding -fno-stack-protector \
    -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables \
    -fno-unwind-tables -fno-pic -fno-PIE \
    -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
    -c src/kernel_main.cpp -o kernel_main.o

gcc -m32 -march=i386 \
    -ffreestanding -fno-stack-protector \
    -fno-asynchronous-unwind-tables \
    -c src/KEE.c -o KEE.o

ld -m elf_i386 -T src/linker.ld \
    entrypoint.o boot.o stack.o constants.o kernel_main.o KEE.o \
    -o iso/boot/kernel.elf --entry=_start

grub-mkrescue -o kernel.iso iso/
