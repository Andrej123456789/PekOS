nasm -f bin stage1.asm -o stage1.bin

nasm -f bin stage2.asm -o stage2.bin

nasm -f bin main.asm -o main.bin

dd if=/dev/zero of=disk.iso bs=1024 count=1440
dd if=main.bin of=disk.iso conv=notrunc
qemu-system-x86_64 disk.iso