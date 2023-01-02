
    ; cpui →  discover details of the processor
    
    cpui:
        pushfd
        pop     eax

        mov     ecx,    eax

        xor     eax,    1 << 21

        push    eax
        popfd

        pushfd
        pop     eax

        push    ecx
        popfd

        xor     eax,    ecx
        jz      NOID

        ret

    lmode:
        mov     eax,    0x80000001
        cpuid
        test    edx,    1 << 29
        jz      NOLM
        ret

    NOLM:
        hlt         ;   NO Long mode support

    NOID:
        hlt         ;   NO CPUID support
