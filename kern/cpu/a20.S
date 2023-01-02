
    ;   _ena20 → enables A20 line
    _ena20:
        in      al,     0x92
        or      al,     2
        out     0x92,   al
        ret
