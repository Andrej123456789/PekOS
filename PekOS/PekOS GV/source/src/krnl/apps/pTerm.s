

    ;
    ; @AUTHOR :: OS Dev Wiki
    ; @LINK :: https://wiki.osdev.org/Real_mode_assembly_bare_bones
    ;

    mov ax, 0  ; set up segments
    mov ds, ax
    mov es, ax
    mov ss, ax

    %include "src/krnl/inc/cmd.inc"

    pTerm:
        call pterm_scr

        mov si, scr_msg
        call writef

        pTerm_m:

            call scroll

            ;mov si, prompt
            ;call writef

            mov di, buffer
            call getinput

            mov si, buffer
            cmp byte [si], 0
            je pTerm_m

            mov si, buffer
            mov di, empty
            call strcmp
            jc .empty

            mov si, buffer
            mov di, cmd_ver
            call strcmp
            jc .ver

            mov si, buffer
            mov di, cmd_clr
            call strcmp
            jc .clr

            mov si, buffer
            mov di, cmd_lst
            call strcmp
            jc .list

            mov si, buffer
            mov di, cmd_hlp
            call strcmp
            jc .help

            mov si, buffer
            mov di, cmd_ext
            call strcmp
            jc .exit

            mov si, buffer
            mov di, cmd_rbt
            call strcmp
            jc .reboot

            mov si, buffer
            mov di, cmd_sht
            call strcmp
            jc .shutdw

            mov si, buffer
            mov di, cmd_osf
            call strcmp
            jc .infa

            mov si, buffer
            mov di, run_pnt
            call strcmp
            jc .paint

            mov si, cmd_err
            call writef
            jmp pTerm_m

        ; ============ ;
        ;   COMMANDS   ;
        ; ============ ;
    
        .empty:
            jmp pTerm_m

        .shutdw:
            jmp _exit

        .reboot:
            jmp bootscr

        .exit:
            jmp kmain

        .infa:
            mov si, help_infa
            call writef

            jmp pTerm_m

        .ver:
            mov si, pTerm_ver
            call writef
            
            mov si, BLANK
            call writef

            jmp pTerm_m

        .help:
            mov si, help_str
            call writef

            jmp pTerm_m

        .paint:
            call paint_m

        .clr:
            jmp pTerm

        .list:
            mov si, list_str
            call writef

            jmp pTerm_m

        ;
        ; STUFF
        ;

        getinput:
            xor cl, cl
        
        .loop:
            call scroll

            mov ah, 0
            int 16h
            

            cmp al, 0x08
            je .backspace

            cmp al, 0x0D
            je .done

            cmp cl, 0x40
            je .loop

            mov ah, 0x0e
            int 10h

            stosb
            inc cl
            call scroll
            jmp .loop

        .backspace:
            cmp cl, 0
            je .loop

            dec di
            mov byte [di], 0
            dec cl

            mov ah, 0x0e
            mov al, 0x08
            int 10h

            mov al, ' '
            int 10h

            mov al, 0x08
            int 10h

            jmp .loop

        .done:
            mov al, 0
            stosb

            mov ah, 0x0e
            mov al, 0x0D
            int 10h

            mov al, 0x0A
            int 10h
            
            ret 
