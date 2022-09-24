
#ifndef _WRITE_H
#define _WRITE_H

#include "../CM/common.h"

enum color {
	BLACK	= 0,
	BLUE	= 1,
	GREEN	= 2,
	CYAN	= 3,
	RED	= 4,
	MAGENTA	= 5,
	BROWN	= 6,
	
	L_GRAY	= 7,
	D_GRAY	= 8,
	
	L_BLUE	= 9,
	L_GREEN	= 10,
	L_CYAN	= 11,
	L_RED	= 12,
	L_MAG	= 13,
	YELLOW	= 14,
	
	WHITE	= 15,

};

void putc(char c);
void clear(uint8_t bg, uint8_t fg);
void write(char *c);
void w_hex(uint32_t n);
void w_dec(uint32_t n);

#endif
