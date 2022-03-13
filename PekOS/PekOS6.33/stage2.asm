%include "stage2info.inc"
ORG STAGE2_RUN_OFS



stage2start:

    start:

        call cls
        XOR AL, AL     ; Clear entire screen
        mov ah, 09h
        mov cx, 9999h
        mov al, 20h
        mov bl, 1fh
        int 10h

        mov ah, 09h
        mov cx, 80d
        mov al, 20h
        mov bl, 8fh
        int 10h

        ;MOV AH, 06h    ; Scroll up function
        ;XOR CX, CX     ; Upper left corner CH=row, CL=column
        ;MOV DX, 184FH  ; lower right corner DH=row, DL=column
        ;INT 10H

        mov si, osvermsg
        call print_string

        mov si, text_string ; Put string position into SI
        call print_string   ; string-printing routine

    push bx
    push cx
    push dx
    mov ah,0h
    int 16h

        cmp al, '1'
        je shutdown
        cmp al, '2'
        je about
        cmp al, '3'
        je pTerm
        cmp al, '4'
        je README

        jmp $            ; Jump here - infinite loop!

        call .message
        %include "stage2text.asm"
        

    shutdown:
    mov ax, 0x1000
    mov ax, ss
    mov sp, 0xf000
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15

    about:
    call cls
    MOV AH, 06h    ; Scroll up function
    XOR AL, AL     ; Clear entire screen
    XOR CX, CX     ; Upper left corner CH=row, CL=column
    MOV DX, 184FH  ; lower right corner DH=row, DL=column
    MOV BH, 0fh    ; InsertColor
    INT 10H
    mov si, about_string
    call print_string
    push bx
    push cx
    push dx
    mov ah,0h
    int 16h
    je start

    README:
    call cls
    MOV AH, 06h
    XOR AL, AL
    XOR CX, CX
    MOV DX, 184FH
    MOV BH, 0fh
    INT 10h
    mov si, Firsttimeusertxt
    call print_string
    push bx
    push cx
    push dx
    mov ah,0h
    int 16h
    je start

    pTerm:
    call cls
    MOV AH, 06h    ; Scroll up function
    XOR AL, AL     ; Clear entire screen
    XOR CX, CX     ; Upper left corner CH=row, CL=column
    MOV DX, 184FH  ; lower right corner DH=row, DL=column
    MOV BH, 0fh    ; InsertColor
    INT 10H
    mov si, pterm_str ; Put string position into SI
    call print_string   ; Call our string-printing routine
    call mainloop

    %include "pterm.asm"

    cls:
    pusha
    mov ah, 0x00
    mov al, 0x03  ; text mode 80x25 16 colours
    int 0x10
    popa
    ret

    

        

    ;rint_string:           ; Routine: output string in SI to screen
        ;mov ah, 0Eh     ; int 10h 'print char' function

    .repeat:
        lodsb           ; Get character from string
        cmp al, 0
        je .done        ; If char is zero, end of string
        int 10h         ; Otherwise, print it
        jmp .repeat

    .done:
        ret
