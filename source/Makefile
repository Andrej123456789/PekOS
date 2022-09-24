
#|																\
	MAIN MAKEFILE FOR „BtOS System II”							\
	@AUTHOR		: Stjepan Bilić Matišić	(StjepanBM1)			\
	@ABOUT		: Main Makefile used in compilation				\
				  of Bt System II.								\
|#

#|		COMPILER & COMPILER OPTIONs		|#
ARCH = i686-elf

AS = $(ARCH)-as
AC = nasm -f elf -o

CC = $(ARCH)-gcc
CF = -std=gnu99 -fno-builtin -ffreestanding -O2 -Wall -Wextra

LD = $(CC) -T $(BLD)/link.ld -o $(BLD)/kernel
LF = -ffreestanding -fno-builtin -O2 -nostdlib $(BIN)/!boot $(OFILES) -lgcc

#|			DIRECTORYs & Files			|#

OFILES=bin/*.o

BIN = bin
BLD = build

SRC = src
BOT = boot
DEV = krnl

#|			EMULATION SETTINGS			|#
EMU=qemu-system
ARC=-i386
DEV0=-cdrom
DEV1=-kernel

#|				TARGETS					|#
all: boot krnl link grub run

boot:
	$(AS) $(SRC)/!boot.s -o $(BIN)/!boot
#	$(AC) $(BIN)/!boot $(SRC)/!boot.s

krnl:
#	Most used files
	$(CC) -c $(SRC)/main.c -o $(BIN)/main.o  $(CF)
	$(CC) -c $(SRC)/CM/common.c -o $(BIN)/common.o $(CF)
	$(CC) -c $(SRC)/WRT/write.c -o $(BIN)/write.o $(CF)

#	GDT
	$(AC) $(BIN)/gdt_s.o $(SRC)/GDT/gdt.s
	$(CC) -c $(SRC)/GDT/gdt.c -o $(BIN)/gdt.o $(CF)

#	IDT, ISR, IRQ, TIMER, KEYBOARD
	$(AC) $(BIN)/ints_s.o $(SRC)/INTs/ints.s
	$(CC) -c $(SRC)/INTs/ints.c -o $(BIN)/ints.o $(CF)
	$(CC) -c $(SRC)/INTs/keyb.c -o $(BIN)/keyb.o $(CF)
	$(CC) -c $(SRC)/INTs/timer.c -o $(BIN)/timer.o $(CF)

#	Mem. stuff
	$(CC) -c $(SRC)/MM/kheap.c -o $(BIN)/kheap.o $(CF)
	$(CC) -c $(SRC)/MM/paging.c -o $(BIN)/paging.o $(CF)
	$(CC) -c $(SRC)/MM/array.c -o $(BIN)/array.o $(CF)

#	Print Functions
	$(CC) -c $(SRC)/WRT/printf.c -o $(BIN)/printf.o $(CF)
	$(CC) -c $(SRC)/WRT/vsprintf.c -o $(BIN)/vsprintf.o $(CF)

#	User stuff

#	Main kernel file
	$(CC) -c $(SRC)/main.c -o $(BIN)/main.o  $(CF)

link:
	$(LD) $(LF)

grub:
	@printf "\033[0;31m!== ISO file is not protected under RGL ==!\033[0m \n"

	@mkdir -p isodir/boot/grub/images
	@cp $(BLD)/kernel isodir/boot/kernel
	@cp $(BLD)/grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(BLD)/PekOS-8.iso isodir
	rm -rf isodir

run:
	$(EMU)$(ARC) $(DEV0) $(BLD)/PekOS-8.iso

runk:
	$(EMU)$(ARC) $(DEV1) $(BLD)/kernel
	
clean:
	rm -rf $(BIN)/*.o
	rm -rf $(BIN)/!boot
	rm -rf $(BLD)/kernel
	rm -rf $(BLD)/*.iso
