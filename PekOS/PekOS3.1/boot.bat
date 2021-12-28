nasm -f bin stage2.asm -o stage2.bin

nasm -f bin main.asm -o main.bin

qemu-system-x86_64 -fda disk.img

pause
