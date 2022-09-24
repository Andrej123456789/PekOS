
#include "write.h"

uint16_t *video_mem = (uint16_t*)0xB8000;

uint8_t cursor_x =  0;
uint8_t cursor_y =  0;

static void mv_cursor()
{
	uint16_t cursorLoc = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLoc >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
 	outb(0x3D5, cursorLoc);      // Send the low cursor byte
}

static void scroll(uint8_t bg, uint8_t fg)
{
	uint8_t attribute_Byte = (bg /*black*/ << 4) | (fg /*white*/ & 0x0F);
	uint16_t blank = 0x20 /* space */ | (attribute_Byte << 8);

	if (cursor_y >= 25)
	{
		int i;
		for (i = 0*80; i < 24*80; i++)
			video_mem[i] = video_mem[i+80];
			
		for (i = 24*80; i < 25*80; i++)
			video_mem[i] = blank;
			
		cursor_y = 24;
	}

}

void putc(char c)
{
	uint8_t bg = BLACK;
	uint8_t fg = WHITE;
	uint8_t attribute_Byte = (bg << 4) | (fg & 0x0F);
	
	uint16_t attribute = attribute_Byte << 8;
	
	if (c == 0x08 && cursor_x)
		cursor_x--;
		
	else if (c == 0x09)
		cursor_x = (cursor_x+8) & ~(8-1);
		
	else if (c == '\r')
		cursor_x = 0;
	
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	
	else if (c >= ' ')
	{
		video_mem[cursor_y*80 + cursor_x] = c | attribute;
		cursor_x++;
	}
	
	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y++;
	}
	
	scroll(BLACK, WHITE);
	mv_cursor();
}

void clear(uint8_t bg, uint8_t fg)
{
	uint8_t attribute_Byte = (bg << 4) | (fg & 0x0F);
	uint16_t blank = 0x20 | (attribute_Byte << 8);
	
	int i;
	for (i = 0; i < 80*25; i++)
		video_mem[i] = blank;
	
	cursor_x = 0;
	cursor_y = 0;
	
	mv_cursor();
}

void write(char *c)
{
	while (*c)
		putc(*c++);
}

void w_hex(uint32_t n)
{

	int tmp;
	char noZeroes = 1;
	
	write("0x");
	
	int i;
	for (i = 28; i >= 0; i -= 4)
	{
		tmp = (n >> i) & 0xF;
		
		if (tmp == 0 && noZeroes != 0)
		continue;
		
		noZeroes = 0;
		
		if (tmp >= 0xA)
			putc(tmp-0xA+'a');
		
		else
			putc(tmp+'0');
	}

}

void w_dec(uint32_t n)
{
	if (n == 0)
	{
		putc('0');
		return;
	}
	
	uint32_t acc = n;
	char c[32];
	int i = 0;
	
	while (acc > 0)
	{
		c[i] = '0' + acc%10;
		acc /= 10;
		i++;
	}
	c[i] = 0;
	
	char c2[32];
	c2[i--] = 0;
	int j = 0;
	
	while(i >= 0)
		c2[i--] = c[j++];
	
	write(c2);
}

extern void panic(const char *message, const char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

	printf("\nPANIC { %s at %s : %d }\n", message, file, line);
	printf("\nPlease restart your system...");

	asm volatile("hlt"); // Halt the system

	/*
    write("PANIC(");
    write(message);
    write(") at ");
    write(file);
    write(":");
    w_dec(line);
    write("\n");
    // Halt by going into an infinite loop.
    //for(;;);
	*/

}

extern void panic_assert(const char *file, u32int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

	printf("ASSERTUIN - FAILED { %s at %s : %d }\n", desc, file, line);

    // Halt by going into an infinite loop.
    for(;;);
}
