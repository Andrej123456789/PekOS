
/*
 *	{ main.c } :	Main Kernel file for the
*/

#include	<stddef.h>
#include	<stdint.h>

#include	"CM/common.h"
#include	"multiboot.h"
#include	"WRT/write.h"
#include	"MM/paging.h"
#include	"MM/kheap.h"

/* Compilation checks  */
#if defined(__linux__)
	#error	"Error : You are not using a cross-compiler"
#endif

#if !defined(__i386__)
	#error	"Error: PekOS needs to be compiled with a i686-elf compiler"
#endif

void main(multiboot_t* mboot_ptr)
{	
	clear(BLACK, WHITE);


	gdt_init();
	idt_init();
	printf("	PekOS v8	|	Running Bt System Kernel II	\n");
	
    initialise_paging();

	timer_init(20);
	
	keyboard_install();
	printf("] ");

	asm volatile ("sti");

	for(;;);
	return 0xdeadbeef;
}
