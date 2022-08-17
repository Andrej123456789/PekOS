
    ; ---------------- ;
    ;   SCREEN SETUP   ;
    ; ---------------- ;

    scr_setup:

        ; SCREEN
        call clear
        xor al, al

        mov ah, 09h
        mov cx, 6000d
        mov al, 20h
        mov bl, 0x1F             ; FOREGROUND ON BACKGROUND --- BLACK (TEXT) on WHITE (BACKGROUND)
        int 10h

        ; ACT_BAR
        mov si, ACT_BAR
        call writef

        ret

    