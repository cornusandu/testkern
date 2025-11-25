set -e

./clear

nasm -f elf32 ./src/entrypoint.asm -o entrypoint.o
nasm -f elf32 ./src/bootseq/boot.asm -o boot.o
nasm -f elf32 ./src/constants.asm -o constants.o
nasm -f elf32 ./src/bootseq/stack.asm -o stack.o
nasm -f elf32 ./src/printk.asm -o printk.o

g++ -m32 -march=i386 -mtune=generic \
    -O2 \
    -ffreestanding -fno-stack-protector \
    -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables \
    -fno-unwind-tables -fno-pic -fno-PIE \
    -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wno-write-strings \
    -c src/*.cpp

comp_driver() {
    mkdir -p tmp_DRV_$2
    mkdir -p ./iso/boot/DRV/

    g++ -m32 -march=i386 -mtune=generic \
    -O2 \
    -ffreestanding -fno-stack-protector \
    -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables \
    -fno-unwind-tables -fno-pic -fno-PIE \
    -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wno-write-strings -c $1/fs.cpp -o tmp_DRV_$2/fscpp.o
    nasm $1/fs.asm -o tmp_DRV_$2/fsasm.o -f elf32
    ld $(find ./tmp_DRV_$2 -name "*.o") -T"$1/fs.ld" -o ./iso/boot/DRV/$2.drv -m elf_i386
    objcopy -O binary tmp_DRV_$2/$2.elf ./iso/boot/DRV/$2.drv
}

comp_driver ./drivers/fs fs

gcc -m32 -march=i386 \
    -ffreestanding -fno-stack-protector \
    -fno-asynchronous-unwind-tables -Wwrite-strings \
    -c src/*.c

mkdir -p ./tmp1

mv $(find . -name "*.o") ./tmp1

ld -m elf_i386 -T src/linker.ld \
    $(find ./tmp1 -name "*.o") \
    -o iso/boot/kernel.elf --entry=_start

grub-mkrescue -o kernel.iso iso/
