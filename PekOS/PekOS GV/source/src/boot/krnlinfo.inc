KERNEL_ABS_ADDR   equ 0x07e00    ; Physical address of KERNEL

; Segment and Offset to use to transfer (FAR JMP) control to KERNEL
;     Segment:Offset = 0x0000:0x7e00
KERNEL_RUN_SEG   equ 0x0000
KERNEL_RUN_OFS   equ KERNEL_ABS_ADDR
