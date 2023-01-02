
    PRG_SPACE   equ     0x8000

    ;   _rdisk → read data from the disk
    _rdisk:
        mov     ah,     0x02
        mov     bx,     PRG_SPACE
        mov     al,     32
        mov     dl,     [BOOTD]
        mov     ch,     0x00
        mov     dh,     0x00
        mov     cl,     0x02

        int             0x13

        jc      _fail

        ret

    _fail:
        mov     ah,     0x09
        mov     cx,     2000
        mov     al,     20h
        mov     bl,     0x4F
        int             0x10
        
        mov     bx,     derr
        call            bput

        jmp             $

    BOOTD:  db  0
