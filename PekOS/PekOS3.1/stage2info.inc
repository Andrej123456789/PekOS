STAGE2_ABS_ADDR   equ 0x07e00    ; Physical address of stage2

; Segment and Offset to use to transfer (FAR JMP) control to Stage2
;     Segment:Offset = 0x0000:0x7e00
STAGE2_RUN_SEG   equ 0x0000
STAGE2_RUN_OFS   equ STAGE2_ABS_ADDR