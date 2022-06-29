SECTIONS
{
    /* Higher half kernel - will run from 3GB+ addresses */
    .text 0xC0000000 :
    {
        *(kernel_entry); 
        *(.text*);
    }
    .data :
    {
        *(.data*);
    }
    .rodata :
    {
        *(.rodata*);
    }
    .bss :
    {
        *(.bss*);
    }
}
