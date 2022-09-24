
#ifndef _GDT_H
#define _GDT_H

#include "../CM/common.h"

void gdt_init();

typedef struct
{
	uint16_t limit_low;     // The lower 16 bits of the limit.
	uint16_t base_low;      // The lower 16 bits of the base.
	uint8_t base_middle;    // The next 8 bits of the base.
	uint8_t access;         // Access flags, determines what ring this segment can be used in.
	uint8_t granularity;
	uint8_t base_high;      // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
	uint16_t limit;       // The upper 16 bits of all selector limits.
	uint32_t base;        // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdt_ptr_t;

extern void gdt_flush(uint32_t);
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// The GDT itself.
gdt_entry_t gdt_entries[6];
// Pointer structure to give to the CPU.
gdt_ptr_t gdt_ptr;

#endif
