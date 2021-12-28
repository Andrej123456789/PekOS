%include "stage2info.inc"

STAGE2_LOAD_SEG  equ STAGE2_ABS_ADDR>>4



STAGE2_LBA_START equ 1
 
STAGE2_LBA_END   equ STAGE2_LBA_START + NUM_STAGE2_SECTORS

DISK_RETRIES     equ 3

bits 16
ORG 0x7c00

%include "bpb.inc"

boot_start:
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7c00
    cld

load_stage2:
    mov [bootDevice], dl
    mov di, STAGE2_LOAD_SEG
    mov si, STAGE2_LBA_START
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
    cmp si, STAGE2_LBA_END
    jl .read_sector_loop

.stage2_loaded:
    mov ax, STAGE2_RUN_SEG
    mov ds, ax
    mov es, ax

    ; FAR JMP to the Stage2 entry point at physical address 0x07e00
    jmp STAGE2_RUN_SEG:STAGE2_RUN_OFS

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
diskErrorMsg:    db "Unrecoverable disk error!", 0

TIMES 510-($-$$) db  0
dw 0xaa55

NUM_STAGE2_SECTORS equ (stage2_end-stage2_start+511) / 512
                                ; Number of 512 byte sectors stage2 uses.

stage2_start:
    incbin "stage2.bin"

stage2_end:
