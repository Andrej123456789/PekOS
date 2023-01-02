
    [ORG    0x7c00]
    [BITS       16]

        ;   _startb → bootloader entry point
        _startb:
            mov     dl, [BOOTD]

            mov     bp, 0x7c00
            mov     sp, bp

            mov ah, 0x00
            mov al, 0x03    ; text mode 80x25 16 colours
            int 10h

            mov     bx, bmsg
            call        bput

            call    _rdisk

            jmp     PRG_SPACE


        ;   File includes
        %include "boot/bput16.S"
        %include "boot/disk16.S"

    times   510 - ($-$$) db 0
    dw      0xaa55
