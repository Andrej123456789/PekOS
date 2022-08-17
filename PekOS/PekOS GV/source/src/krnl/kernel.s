
    ;
    ;   MAIN KERNEL FILE
    ;


    %include "src/boot/krnlinfo.inc"

    ORG KERNEL_RUN_OFS

    [bits 16]
    bootscr:

        .start:
        .end:
        jmp kmain


    kmain:

        ; -- TOP BAR -- ;
        call scr_setup

        ; --  MOUSE -- ;
        call mouse

    ; --> INCLUDES <--;

    %include "src/krnl/print/write.s"
    %include "src/krnl/mouse.s"
    %include "src/krnl/screen.s"
    
    %include "src/krnl/inc/strcmp.inc"
    %include "src/krnl/inc/OSinf.inc"
    
    %include "src/krnl/apps/pTerm.s"
    %include "src/krnl/apps/Paint.s"
