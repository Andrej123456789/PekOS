
    ;
    ;   MAIN BOOTLOADER FILE
    ;
    ;   PEKOS BOOTLOADER V3.1
    ;

    %include "src/boot/krnlinfo.inc"
    %include "src/boot/bpb.inc"

    KERNEL_LOAD_SEG  equ KERNEL_ABS_ADDR>>4

    KERNEL_LBA_START equ 1
    
    KERNEL_LBA_END   equ KERNEL_LBA_START + NUM_KERNEL_SECTORS

    DISK_RETRIES     equ 3

    bits 16
    ORG 0x7c00

    boot_start:
        xor ax, ax
        mov ds, ax
        mov ss, ax
        mov sp, 0x7c00
        cld

    load_KERNEL:
        mov [bootDevice], dl
        mov di, KERNEL_LOAD_SEG
        mov si, KERNEL_LBA_START
        jmp .chk_for_last_lba

    .read_sector_loop:
        mov bp, DISK_RETRIES

        call lba_to_chs
        mov es, di
        xor bx, bx

    .retry:
        mov ax, 0x0201

        int 0x13
        jc .disk_error

    .success:
        add di, 512>>4
        inc si

    .chk_for_last_lba:
        cmp si, KERNEL_LBA_END
        jl .read_sector_loop

    .KERNEL_loaded:
        mov ax, KERNEL_RUN_SEG
        mov ds, ax
        mov es, ax

        ; FAR JMP to the KERNEL entry point at physical address 0x07e00
        jmp KERNEL_RUN_SEG:KERNEL_RUN_OFS

    .disk_error:
        xor ah, ah
        int 0x13
        dec bp
        jge .retry

    error_end:
        mov si, diskErrorMsg        ; Display disk error message
        call print_string
        cli

    .error_loop:
        hlt
        jmp .error_loop

    print_string:
        mov ah, 0x0e
        xor bx, bx
        jmp .getch

    .repeat:
        int 0x10                    ; print character

    .getch:
        lodsb                       ; Get character from string
        test al,al                  ; Have we reached end of string?
        jnz .repeat                 ;     if not process next character
    
    .end:
        ret
    
    lba_to_chs:
        push ax                     ; Preserve AX
        mov ax, si                  ; Copy LBA to AX
        xor dx, dx                  ; Upper 16-bit of 32-bit value set to 0 for DIV
        div word [sectorsPerTrack]  ; 32-bit by 16-bit DIV : LBA / SPT
        mov cl, dl                  ; CL = S = LBA mod SPT
        inc cl                      ; CL = S = (LBA mod SPT) + 1
        xor dx, dx                  ; Upper 16-bit of 32-bit value set to 0 for DIV
        div word [numHeads]         ; 32-bit by 16-bit DIV : (LBA / SPT) / HEADS
        mov dh, dl                  ; DH = H = (LBA / SPT) mod HEADS
        mov dl, [bootDevice]        ; boot device, not necessary to set but convenient
        mov ch, al                  ; CH = C(lower 8 bits) = (LBA / SPT) / HEADS
        shl ah, 6                   ; Store upper 2 bits of 10-bit Cylinder into
        or  cl, ah                  ;     upper 2 bits of Sector (CL)
        pop ax                      ; Restore scratch registers
        ret


    bootDevice:      db 0x00
    diskErrorMsg:    db "ERROR :: Unrecoverable disk error", 0

    TIMES 510-($-$$) db  0
    dw 0xaa55

    NUM_KERNEL_SECTORS equ (KERNEL_end-KERNEL_start+511) / 512
                                    ; Number of 512 byte sectors KERNEL uses.

    KERNEL_start:
        incbin "out/kernel.bin"

    KERNEL_end:
