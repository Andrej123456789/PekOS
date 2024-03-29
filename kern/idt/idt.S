
    [extern     _idt]
    idt_desc:
        dw  4095
        dq  _idt

    %macro pushall 0
        push rax
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
    %endmacro


    %macro popall 0
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rax
    %endmacro

    [extern     isr0_handler]
    isr0:
        pushall
        call isr0_handler
        popall

        iretq
        GLOBAL isr0

    [extern     isr1_handler]
    isr1:
        pushall
        call    isr1_handler
        popall

        iretq
        GLOBAL  isr1

    [extern     isr8_handler]
    isr8:
        pushall
        call isr8_handler
        popall

        iretq
        GLOBAL isr8

    [extern     isr12_handler]
    isr12:
        pushall
        call    isr12_handler
        popall

        iretq
        GLOBAL  isr12

    [extern     isr13_handler]
    isr13:
        pushall
        call isr13_handler
        popall

        iretq
        GLOBAL isr13

    [extern     isr14_handler]
    isr14:
        pushall
        call    isr14_handler
        popall

        iretq
        GLOBAL  isr14

    loadidt:
        lidt    [idt_desc]
        sti
        ret
        GLOBAL  loadidt
