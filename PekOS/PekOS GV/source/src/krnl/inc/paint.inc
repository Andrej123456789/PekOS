
    ;
    ;   INCLUDE FILE FOR PAINT
    ;

    canvas:
        call clear
        xor al, al

        mov ah, 09h
        mov cx, 6000d
        mov al, 20h
        mov bl, 0xF0             ; FOREGROUND ON BACKGROUND --- BLACK (TEXT) on WHITE (BACKGROUND)
        int 10h

        jmp paint_tbar

        ret

    paint_tbar:
        xor al, al

        mov ah, 09h
        mov cx, 80d
        mov al, 20h
        mov bl, 0x0F
        int 10h
        
        mov si, p_tbar_txt
        call writef

        jmp paint_cursor

        ret

    paint_cursor:
        mov ah, 01h
        mov cx, 07h
        int 10h

        mov dl, 2d
        mov dh, 1h

        jmp _p_cursor

        _p_cursor:
            mov ah, 02h
            mov dl, bl 
            mov dh, cl
            int 10h

            mov ah, 00h
            int 16h

            cmp ah, 0x48            ; UP ARROW KEY
            je p_up

            cmp ah, 0x50            ; DOWN ARROW KEY
            je p_down

            cmp ah, 0x4B            ; LEFT ARROW KEY
            je p_left

            cmp ah, 0x4D            ; RIGHT ARROW KEY
            je p_right

            cmp al, 113d            ; Q
            je p_place              ; place a block

            cmp al, 101d            ; E
            je p_erase              ; erase a block

            ; TRANSPER
            cmp al, 48d             ; no shade
            je p_place

            cmp al, 49d             ; Dark shade
            je p_trans_1
            
            cmp al, 50d             ; Medium shade
            je p_trans_2

            cmp al, 51d             ; Light shade
            je p_trans_3

            ; Other Keys
            cmp al, 105d              ; Info
            je p_info               ; Launch the Info pop-up

            cmp al, 27d             ; ESC
            je kmain                ; exit to desktop
        
        p_up:
            cmp cl, 0h
            je _p_cursor

            sub cl, 1
            jmp _p_cursor

        p_down:
            cmp cl, 24d
            je _p_cursor

            add cl, 1
            jmp _p_cursor

        p_left:
            cmp bl, 0h
            je _p_cursor

            sub bl, 1
            jmp _p_cursor

        p_right:
            cmp bl, 79d
            je _p_cursor

            add bl, 1
            jmp _p_cursor

        p_place:
            mov ah, 0x0e
            mov al, 219d     ; PRINT OUT A CHAR
            int 10h

            jmp _p_cursor

        p_erase:
            mov ah, 0x0e
            mov al, 32d
            int 10h

            jmp _p_cursor

        ; TRANSPER
        p_trans_1:
            mov ah, 0x0e
            mov al, 178d
            int 10h

            jmp _p_cursor

        p_trans_2:
            mov ah, 0x0e
            mov al, 177d
            int 10h

            jmp _p_cursor

        p_trans_3:
            mov ah, 0x0e
            mov al, 176d
            int 10h

            jmp _p_cursor

        ; POP-UPS
        p_info:
            mov bl, 2d
            mov cl, 1h

            mov si, p_info_txt
            call writef

            mov ah, 0h
            int 16h
            je canvas