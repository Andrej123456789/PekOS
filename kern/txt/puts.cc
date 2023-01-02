#pragma once
#define VGA_MEM (unsigned char*)0xb8000
#define VGA_W   80

#include "../io/io.cc"
#include "color.cc"
#include <stddef.h>
#include <stdint.h>

uint16_t cursor_pos;

void clear(uint64_t clearc = BG_BLUE | FG_WHITE)
{
    uint64_t value = 0;
    value += clearc << 8;
    value += clearc << 24;
    value += clearc << 40;
    value += clearc << 56;

    for (uint64_t* i = (uint64_t*)VGA_MEM; i < (uint64_t*)(VGA_MEM + 4000); i++)
    {
        *i = value;
    }
}

void set_cursor_pos(uint16_t pos)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    cursor_pos = pos;

}

uint16_t pos_from_cord(uint8_t x, uint8_t y)
{
    return y * VGA_W + x;
}

void putc(char c, uint8_t color = BG_BLUE | FG_WHITE)
{
    *(VGA_MEM + cursor_pos * 2) = c;
    *(VGA_MEM + cursor_pos * 2 + 1) = color;
    set_cursor_pos(cursor_pos + 1);
}

void puts(const char* str, uint8_t color = BG_BLUE | FG_WHITE)
{
    uint8_t *char_ptr = (uint8_t*)str;
    uint16_t index = cursor_pos;

    while (*char_ptr != 0)
    {
        switch (*char_ptr)
        {
            case 10:
                index += VGA_W;
                index -= index % VGA_W;
                break;

            case 13:
                index -= index % VGA_W;
                break;
            
            default:
                *(VGA_MEM + index * 2) = *char_ptr;
                *(VGA_MEM + index * 2 + 1) = color;
                index++;
        }
        char_ptr++;
    }

    set_cursor_pos(index);

}

char hextostrout[128];
template<typename T>
const char* hextostr(T value)
{
    T* val_ptr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = (sizeof(T)) * 2 - 1;
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)val_ptr + i);

        temp = ((*ptr & 0xF0) >> 4);
        hextostrout[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);

        temp = ((*ptr & 0x0F));
        hextostrout[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);

    }
    hextostrout[size+1] = 0;
    return hextostrout;
}
