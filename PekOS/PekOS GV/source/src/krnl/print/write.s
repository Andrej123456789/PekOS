
    ; ************** ;
    ;   PRINT FUNC   ;
    ; ************** ;

    %define nl 0x0D, 0x0A
    %include "src/krnl/print/text.inc"

    writef:                     ; PRINT A STRING

        lodsb        ; grab a byte from SI

        or al, al  ; logical or AL by itself
        jz .done   ; if the result is zero, get out

        mov ah, 0x0E
        int 0x10      ; otherwise, print out the character!

        jmp writef

    .done:
        ret

    clear:             ; CLEAR THE SCREEN
        pusha

        mov ah, 0x00
        mov al, 0x03    ; text mode 80x25 16 colours
        int 10h

        popa
        ret
