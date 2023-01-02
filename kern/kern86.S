
    jmp     _kr16t32

    ;%include "kern/txt/kput16.S"
    %include "kern/cpu/a20.S"
    %include "kern/cpu/gdt.S"

    ;   _kr16t32 → switches to 32-bit mode, 16-bit kernel entry point.
    _kr16t32:
        ;mov     bx, kmsg
        ;call        kput

        call        _ena20
        cli
        lgdt        [_gdescriptor]
        mov         eax,    cr0
        or          eax,    1
        mov         cr0,    eax

        jmp         codeseg:_kr32t64

    [BITS           32]
    %include "kern/cpu/cpu.S"
    %include "kern/mem/pge.S"

    ;   _kr32t64 → 32-bit entry point for the kernel, switches to 64-bit mode
    _kr32t64:
        mov         ax,     dataseg
        mov         ds,     ax
        mov         ss,     ax
        mov         es,     ax
        mov         fs,     ax
        mov         gs,     ax
        
        mov         [0xb8000],  byte '&'

        call        cpui
        call        lmode
        call        setup_idp
        call        _gdt32

        jmp         codeseg:_kr64

    [BITS           64]
    [EXTERN         _start]
    %include    "kern/idt/idt.S"
    ;   _kt64 → 64-bit kernel entry point
    _kr64:
        mov         edi,    0xb8000
        mov         rax,    0x1f201f201f201f20
        mov         ecx,    500
        rep         stosq

        call        _start

        jmp         $
        GLOBAL      _kr64

    times   2048 - ($-$$) db 0
