
    ;
    ; STRCMP function
    ; @https://wiki.osdev.org/Real_mode_assembly_bare_bones
    ;
    
    strcmp:
        .loop:
            mov al, [si]
            mov bl, [di]
            cmp al, bl
            jne .notequ

            cmp al, 0
            je .done

            inc di
            inc si
            
            jmp .loop

        .notequ:
            clc
            ret

        .done:
            stc
            ret