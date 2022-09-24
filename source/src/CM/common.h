
#ifndef _COMMON_H
#define _COMMON_H

#include	<stdint.h>
#include	<stddef.h>

/* JamesM's typedefs */
typedef unsigned    int   u32int;
typedef             int   s32int;
typedef unsigned    short u16int;
typedef             short s16int;
typedef unsigned    char  u8int;
typedef             char  s8int;

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);

#endif
