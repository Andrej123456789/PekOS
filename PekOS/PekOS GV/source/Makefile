
# COMPILER #
AS=nasm
AF=-fbin

# DIRS #
SRC=src
BOT=boot
KRN=krnl

OUT=out
BLD=build

# TARGETS #

all: krnl boot img run

boot:
	$(AS) $(AF) $(SRC)/$(BOT)/boot.s -o $(OUT)/boot.bin

krnl:
	$(AS) $(AF) $(SRC)/$(KRN)/kernel.s -o $(OUT)/kernel.bin

img:
	dd if=/dev/zero of=$(BLD)/PekOS-GV.img bs=1024 count=1440
	dd if=$(OUT)/boot.bin of=$(BLD)/PekOS-GV.img conv=notrunc
	
run:
	qemu-system-i386 -fda $(BLD)/PekOS-GV.img

clean:
	rm -rf $(OUT)/*
	rm -rf $(BLD)/*
