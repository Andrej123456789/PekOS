
    ;   gdt.S → sets up the GDT.

    _gnulldesc:
        dd  0
        dd  0
    
    _gcodedesc:
        dw  0xFFFF      ;   Limit
        dw  0x0000      ;   Base    (Low)
        db  0x00        ;   Base    (Med.)
        db  10011010b   ;   Flags
        db  11001111b   ;   Flags + Upper Limit
        db  0x00        ;   Base    (High)
    
    _gdatadesc:
        dw  0xFFFF
        dw  0x0000
        db  0x00
        db  10010010b
        db  11001111b
        db  0x00

    _gend:
    _gdescriptor:
        _gsize:
            dw  _gend   -   _gnulldesc - 1
            dq  _gnulldesc

    codeseg     equ     _gcodedesc  -   _gnulldesc
    dataseg     equ     _gdatadesc  -   _gnulldesc
    
    [BITS       32]
    _gdt32:
        mov [_gcodedesc + 6], byte 10101111b
        mov [_gdatadesc + 6], byte 10101111b

        ret

    [BITS       16]
