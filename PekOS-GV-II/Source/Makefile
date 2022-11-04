
# =============================
# 	[./Makefile]
# Main makefile for compilation
# =============================

#	Assembler
AS=nasm
AF=-f bin
#	Directorys
SRC=src

BLD=bld
BIN=bin

#	Emulation
EMU=qemu-system
DEV=-fda
ARC=-i386

#	Targets

all: disk run

disk: boot krnl
	dd if=/dev/zero of=$(BLD)/disk.img bs=512 count=2880
	mkfs.fat -F 12 -n "PEKOS GV II" $(BLD)/disk.img
	dd if=$(BIN)/bootl.bin of=$(BLD)/disk.img conv=notrunc
	mcopy -i $(BLD)/disk.img $(BIN)/system.bin "::system.bin"

boot:
	$(AS) $(AF) $(SRC)/boot/bootl.S -o $(BIN)/bootl.bin

krnl:
	$(AS) $(AF) $(SRC)/krnl/kernl.s -o $(BIN)/system.bin

run:
	$(EMU)$(ARC) $(DEV) $(BLD)/disk.img

clean:
	rm -rf $(BIN)/*
	rm -rf $(BLD)/*

