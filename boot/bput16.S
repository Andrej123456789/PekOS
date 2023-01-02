
    ;   bput → bootloader print string func.

    bput:
        mov ah, 0x0e

        .loop:
            cmp [bx],   byte 0
            je  .done

            mov al,     [bx]
            int         0x10
            inc         bx

            jmp         .loop

        .done:
            ret

    ;   Strings
    bmsg:   db  '$', 13, 10, 0
    derr:   db  '[', 13,10,'    PekOS : failed to read the disk',13,10, ']',0
