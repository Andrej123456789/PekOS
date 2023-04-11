#pragma once

#include <stddef.h>
#include <stdint.h>

#define     PIC1_COMMAND        0x20
#define     PIC1_DATA           0x21
#define     PIC2_COMMAND        0xA0
#define     PIC2_DATA           0XA1

#define     ICW1_INIT           0x10
#define     ICW1_ICW4           0x01
#define     ICW4_8086           0x01

#define     PIC_EOI             0x20

void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{ 
    uint8_t val;
    asm volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void remap_pic()
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    //outb(PIC1_DATA, 0x20);
    //outb(PIC2_DATA, 0x28);

    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 8);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
