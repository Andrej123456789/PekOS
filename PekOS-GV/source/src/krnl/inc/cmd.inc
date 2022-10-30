    
    ;
    ; INCLUDE FILE FOR PTERM 2.4
    ;

    ;prompt: db '%', 0
    buffer times 64 db 0

    pterm_scr:
        call clear

        xor al, al

        mov ah, 09h
        mov cx, 2000d
        
        mov al, 20h
        mov bl, 0xF0            ; FOREGROUND ON BACKGROUND --- BLACK (TEXT) on WHITE (BACKGROUND)
        
        int 10h

        ret

    scroll:
        xor al, al

        mov ah, 09h
        mov cx, 2000d
        
        mov al, 20h
        mov bl, 0xF0            ; FOREGROUND ON BACKGROUND --- BLACK (TEXT) on WHITE (BACKGROUND)
        
        int 10h

        ret

    ;
    ; COMMANDS
    ;
    empty:      db '',       0
    cmd_ver:    db 'ver',    0        ; COMPLETED
    cmd_osf:    db 'inf', 0           ; COMPLETED
    cmd_clr:    db 'clr',    0        ; COMPLETED
    cmd_hlp:    db 'help',   0        ; COMPLETED
    cmd_ext:    db 'exit',   0        ; COMPLETED
    cmd_lst:    db 'list',   0        ; COMPLETED
    run_pnt:    db 'paint',  0        ; COMPLETED
    cmd_rbt:    db 'reboot', 0        ; COMPLETED
    cmd_sht:    db 'shutdw', 0        ; COMPLETED

    cmd_err:    db '  ? [Syntax Error] ?', nl, 0
