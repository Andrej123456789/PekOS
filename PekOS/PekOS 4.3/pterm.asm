  mov ax, 0  ; set up segments
   mov ds, ax
   mov es, ax
   mov ss, ax     ; setup stack
   mov sp, 0x7C00 ; stack grows downwards from 0x7C00
 
 mainloop:

   mov si, prompt
   call print_string
 
   mov di, buffer
   call get_string
 
   mov si, buffer
   cmp byte [si], 0  ; blank line?
   je mainloop       ; yes, ignore it

   mov si, buffer
   mov di, cmd_about  ; "about" command
   call strcmp
   jc .about
 
   mov si, buffer
   mov di, cmd_help  ; "help" command
   call strcmp
   jc .help

   mov si, buffer
   mov di, cmd_clear  ; "clear" command
   call strcmp
   jc .clear

   mov si, buffer
   mov di, cmd_exit  ; "exit" command
   call strcmp
   jc .exit

   mov si, buffer
   mov di, cmd_shutdown  ; "shutdown" command
   call strcmp
   jc .shutdown

   mov si, buffer
   mov di, cmd_shutdown  ; "shutdown" command
   call strcmp
   jc .shutdown

   mov si,badcommand
   call print_string 
   jmp mainloop 


  .about:
   mov si, msg_about
   call print_string
 
   jmp mainloop
 
 .help:
   mov si, msg_help
   call print_string
 
   jmp mainloop

 .clear:
    call cls
    MOV AH, 06h    ; Scroll up function
    XOR AL, AL     ; Clear entire screen
    XOR CX, CX     ; Upper left corner CH=row, CL=column
    MOV DX, 184FH  ; lower right corner DH=row, DL=column
    MOV BH, 07h    ; InsertColorOnBlue
    INT 10H

    mov si, pterm_str
    call print_string

    jmp mainloop

  .exit:
    mov ax, 0
    int 19h

  .shutdown:
    mov ax, 0x1000
    mov ax, ss
    mov sp, 0xf000
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15


 prompt db '> ', 0
 cmd_clear db 'clear', 0
 cmd_help db 'help', 0
 cmd_about db 'about', 0
 cmd_exit db 'exit', 0
 cmd_shutdown db 'shutdown', 0
 msg_about db 'pTerm is a terminal which allows you to do terminal stuff', 0x0D, 0x0A, 0
 msg_help db 'pTerm commands> help, about, clear, exit, shutdown', 0x0D, 0x0A, 0
 badcommand db 'Bad command entered.', 0x0D, 0x0A, 0
 buffer times 64 db 0

 print_string:
   lodsb        ; grab a byte from SI
 
   or al, al  ; logical or AL by itself
   jz .done   ; if the result is zero, get out
 
   mov ah, 0x0E
   int 0x10      ; otherwise, print out the character!
 
   jmp print_string

 .done:
   ret
 
 get_string:
   xor cl, cl
 
 .loop:
   mov ah, 0
   int 0x16   ; wait for keypress
 
   cmp al, 0x08    ; backspace pressed?
   je .backspace   ; yes, handle it
 
   cmp al, 0x0D  ; enter pressed?
   je .done      ; yes, we're done
 
   cmp cl, 0x3F  ; 63 chars inputted?
   je .loop      ; yes, only let in backspace and enter
 
   mov ah, 0x0E
   int 0x10      ; print out character
 
   stosb  ; put character in buffer
   inc cl
   jmp .loop
 
 .backspace:
   cmp cl, 0	; beginning of string?
   je .loop	; yes, ignore the key
 
   dec di
   mov byte [di], 0	; delete character
   dec cl		; decrement counter as well
 
   mov ah, 0x0E
   mov al, 0x08
   int 10h		; backspace on the screen
 
   mov al, ' '
   int 10h		; blank character out
 
   mov al, 0x08
   int 10h		; backspace again
 
   jmp .loop	; go to the main loop
 
 .done:
   mov al, 0	; null terminator
   stosb
 
   mov ah, 0x0E
   mov al, 0x0D
   int 0x10
   mov al, 0x0A
   int 0x10		; newline
 
   ret
 
 strcmp:
 .loop:
   mov al, [si]   ; grab a byte from SI
   mov bl, [di]   ; grab a byte from DI
   cmp al, bl     ; are they equal?
   jne .notequal  ; nope, we're done.
 
   cmp al, 0  ; are both bytes (they were equal before) null?
   je .done   ; yes, we're done.
 
   inc di     ; increment DI
   inc si     ; increment SI
   jmp .loop  ; loop!
 
 .notequal:
   clc  ; not equal, clear the carry flag
   ret
 
 .done: 	
   stc  ; equal, set the carry flag
   ret
 