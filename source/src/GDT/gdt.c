
#include "gdt.h"

void gdt_init()
{
	gdt_ptr.limit = sizeof (gdt_entry_t) * 6 - 1;
	gdt_ptr.base = (uint32_t) &gdt_entries;

	gdt_set_gate (0, 0, 0, 0, 0);             // Null segment.
	gdt_set_gate (1, 0, 0xFFFFF, 0x9A, 0xCF); // Code segment.
	gdt_set_gate (2, 0, 0xFFFFF, 0x92, 0xCF); // Data segment.
	gdt_set_gate (3, 0, 0xFFFFF, 0xFA, 0xCF); // User mode code segment.

	// Inform the CPU about our GDT.
	gdt_flush ((uint32_t) &gdt_ptr);

}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}
