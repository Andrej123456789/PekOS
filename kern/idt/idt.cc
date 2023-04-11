#pragma once

#include <stddef.h>
#include "../io/io.cc"
#include "../io/Scancode1.cc"
#include "../txt/puts.cc"
#include <stdint.h>

struct idt64
{
    uint16_t    offset_l;
    uint16_t    selector;
    uint8_t     ist;
    uint8_t     types_attrb;
    uint16_t    offset_mid;
    uint32_t    offset_hig;
    uint32_t    zero;
};
extern idt64 _idt[256];
extern uint64_t isr0;
extern uint64_t isr1;
extern uint64_t isr12;
extern uint64_t isr14;
extern "C" void loadidt();

void init_idt()
{
    /* Divide by zero */
    _idt[0].zero        =   0;
    _idt[0].offset_l    =   (uint16_t)(((uint64_t)&isr0 & 0x000000000000ffff));
    _idt[0].offset_mid  =   (uint16_t)(((uint64_t)&isr0 & 0x00000000ffff0000) >> 16);
    _idt[0].offset_hig  =   (uint16_t)(((uint64_t)&isr0 & 0x00000000ffff0000) >> 32);
    _idt[0].ist         =   0;
    _idt[0].selector    =   0x08;
    _idt[0].types_attrb =   0x8e;

    /* Keyboard */
    _idt[1].zero        =   0;
    _idt[1].offset_l    =   (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
    _idt[1].offset_mid  =   (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16);
    _idt[1].offset_hig  =   (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 32);
    _idt[1].ist         =   0;
    _idt[1].selector    =   0x08;
    _idt[1].types_attrb =   0x8e;

    /* Mouse - broken */
    _idt[12].zero       =   0;
    _idt[12].offset_l   =   (uint16_t)(((uint64_t)&isr12 & 0x000000000000ffff));
    _idt[12].offset_mid =   (uint16_t)(((uint64_t)&isr12 & 0x00000000ffff0000) >> 16);
    _idt[12].offset_hig =   (uint16_t)(((uint64_t)&isr12 & 0x00000000ffff0000) >> 32);
    _idt[12].ist        =   0;
    _idt[12].selector   =   0x08;
    _idt[12].types_attrb=   0x8e;

    /* Page fault */
    _idt[14].zero       =   0;
    _idt[14].offset_l   =   (uint16_t)(((uint64_t)&isr14 & 0x000000000000ffff));
    _idt[14].offset_mid =   (uint16_t)(((uint64_t)&isr14 & 0x00000000ffff0000) >> 16);
    _idt[14].offset_hig =   (uint16_t)(((uint64_t)&isr14 & 0x00000000ffff0000) >> 32);
    _idt[14].ist        =   0;
    _idt[14].selector   =   0x08;
    _idt[14].types_attrb=   0x8e;

    remap_pic();

    outb(0x21, 0xfd);
	outb(0xa1, 0xff);

    loadidt();
}

void (*MainKBHandler)(uint8_t scancode, uint8_t c);

extern "C" void isr0_handler()
{
    puts("You cannot divide by zero!\n");
    asm("cli; hlt"); /* halt the CPU */
}

extern "C" void isr1_handler()
{
    uint8_t scancode = inb(0x60);
    uint8_t c = 0;

    if (scancode < 0x3A)
    {
        c = Set1::ScanCodeLookUp[scancode];
    }

    if (MainKBHandler != 0)
    {
        MainKBHandler(scancode, c);
    }

    outb(0x20, 0x20);
	outb(0xa0, 0x20);
}

extern "C" void isr12_handler()
{
    putc('a');
}

extern "C" void isr14_handler()
{
    puts("Page fault!\n");
    asm("cli; hlt"); /* halt the CPU */
}
