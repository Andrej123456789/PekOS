
#include "common.h"

void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
	for(; len != 0; len--) *dest++ = *src++;
}

void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
	for (; len != 0; len--) *dest++ = val;
}

int strcmp(char *str1, char *str2)
{
	while (*str1 && *str2 && (*str1++ == *str2++));
	
	if (*str1 == '\0' && *str2 == '\0')
		return 0;
		
	if (*str1 == '\0')
		return -1;
	else return 1;
}

char *strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	
	*dest = '\0';
}

char *strcat(char *dest, const char *src)
{
	while (*dest)
		*dest = *dest++;
		
	while (*src)
		*dest++ = *src++;
	
	*dest = '\0';
	
	return dest;
}

int strlen(char *src)
{
	int i = 0;
	
	while (*src++)
		i++;
	
	return i;
}
