
    %include "src/krnl/apps/calc.lib"

    _calc:
        call    _calcs
        _calcm:
            mov si, _msg1
            call    print

            mov ah, 0x00
            int     0x16

            mov ah, 0x0e
            int     0x10

            mov si, _msg2
            call    print

            mov ah, 0x00
            int     0x16

            mov ah, 0x0e
            int     0x10

            mov si, _reslt
            call    print
	
	jmp	_kmain
