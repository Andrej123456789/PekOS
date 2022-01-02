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

    jmp STAGE2_RUN_SEG:STAGE2_RUN_OFS

.disk_error:
    xor ah, ah
    int 0x13
    dec bp
    jge .retry

error_end:
    mov si, diskErrorMsg
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
    int 0x10
.getch:
    lodsb
    test al,al
    jnz .repeat
.end:
    ret
lba_to_chs:
    push ax
    mov ax, si
    xor dx, dx
    div word [sectorsPerTrack]
    mov cl, dl
    inc cl
    xor dx, dx
    div word [numHeads]
    mov dh, dl
    mov dl, [bootDevice]
    mov ch, al
    shl ah, 6
    or  cl, ah
    pop ax
    ret


bootDevice:      db 0x00
diskErrorMsg:    db "Unrecoverable disk error!", 0

TIMES 510-($-$$) db  0
dw 0xaa55

NUM_STAGE2_SECTORS equ (stage2_end-stage2_start+511) / 512


stage2_start:
    incbin "stage2.bin"

stage2_end:
