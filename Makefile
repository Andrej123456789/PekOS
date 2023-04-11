
ARCH=

#	Assembler
AS=nasm
AF=-f bin

#	C(++) compiler
CC=$(ARCH)-g++
CF=-ffreestanding  -mno-red-zone -m64 -fpermissive

#	Linker
LD=$(ARCH)-ld

#	Directorys & Out. files
O=bin
OF=$(O)/*
B=build

BT=boot
K=kern

#	Emulator
EMU=qemu-system-x86_64
DEV=-fda
EF=-m 256M

all: bootl kernl link fimg run

bootl:
	$(AS) $(AF) $(BT)/boot16.S -o $(B)/boot16.bin

kernl:
	$(AS) -f elf64 $(K)/kern86.S -o $(O)/kern86.o
	$(AS) -f elf64 $(K)/mem/bins.S -o $(O)/bins.o

	$(CC) $(CF) -c $(K)/kern64.cc -o $(O)/kern64.o

link:
	$(LD) -T "link64.ld"

fimg:
	cat $(B)/boot16.bin $(B)/kernel.bin > $(B)/OS.bin
	dd if=/dev/zero of=$(B)/disk.img bs=512 count=2880
	dd if=$(B)/OS.bin of=$(B)/disk.img conv=notrunc

run:
	$(EMU) $(EF) $(DEV) $(B)/disk.img

clean:
	rm $(OF)
	rm $(B)/*
