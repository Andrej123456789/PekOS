
    ; ------------------------------ ;
    ; CURSOR CONTROLED BY ARROW KEYS ;
    ; ------------------------------ ;

    mouse:

        mov ah, 01h
        mov cx, 07h
        int 10h

        mov dl, 5d      ; COL (0-80)
        mov dh, 5d      ; ROW (0-25)

        jmp _mouse

        _mouse:

            mov ah, 02h
            mov dl, bl 
            mov dh, cl
            int 10h

            mov ah, 00h
            int 16h

            cmp ah, 0x48            ; UP ARROW KEY
            je _up

            cmp ah, 0x50            ; DOWN ARROW KEY
            je _down

            cmp ah, 0x4B            ; LEFT ARROW KEY
            je _left

            cmp ah, 0x4D            ; RIGHT ARROW KEY
            je _right

            cmp al, 20h
            je _click

            ; action-bar menu
            ; keyboard ver

            cmp al, 49d         ; 1
            je _reboot          ; Reboots the OS

            cmp al, 50d         ; 2
            je _exit            ; Shuts down the OS (QEMU)

            cmp al, 51d         ; 3
            je _pTerm           ; Runs pTerm 2.4s

            cmp al, 52d         ; 4
            je _paint           ; Runs Paint 1.0
            
            cmp al, 27d         ; ESC
            je _exit

            jmp _mouse

        _up:
            cmp cl, 0h
            je _mouse

            sub cl, 1
            jmp _mouse

        _down:
            cmp cl, 24d
            je _mouse

            add cl, 1
            jmp _mouse

        _left:
            cmp bl, 0h
            je _mouse

            sub bl, 1
            jmp _mouse

        _right:
            cmp bl, 79d
            je _mouse

            add bl, 1
            jmp _mouse

        _click:

            ;mov ah, 0x0e
            ;mov al, 219d     ; PRINT OUT A CHAR
            ;int 10h

            ; Reboot

            ; Paint

            ; Notes

            ; pTerm

            jmp _mouse

        _reboot:
            call bootscr

        _exit:
            mov ax, 0x1000
            mov ax, ss
            mov sp, 0xf000
            mov ax, 0x5307
            mov bx, 0x0001
            mov cx, 0x0003
            int 0x15

        _paint:
            call paint_m

        _pTerm:
            call pTerm