
#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../txt/puts.cc"
#include "../io/keyb.cc"
#include "../scr/sets.cc"
#include "../app/texted.cc"

#define KEYB_RETURN  0

extern uint8_t lastc;
extern "C" void _kr64();
extern void desktop_handler(uint8_t scancode, uint8_t c);

void menukeyboard(uint8_t scancode, uint8_t c)
{
    switch (lastc)
    {
        case 0x02:
            __asm ("outw %%ax, %%dx" : : "a"(0x2000), "d"(0x604));
            break;

        case 0x03:
            texted();
            break;

        #if KEYB_RETURN >= 1
        default:
            screen();
            MainKBHandler = desktop_handler;
            break;
        #endif
    }

    lastc = scancode;
}

void menu()
{
    MainKBHandler = menukeyboard;
    set_cursor_pos(pos_from_cord(0, 19));
    puts("  ______________________  \n", BG_BWHITE | FG_BLACK);
    /*puts("  4) pTerminal     [3.0]  \n", BG_BWHITE | FG_BLACK);  //  0x05
    puts("  3) Paint         [3.0]  \n", BG_BWHITE | FG_BLACK);  //  0x04*/
    puts("  2) TextED        [1.0]  \n", BG_BWHITE | FG_BLACK);  //  0x03
    puts("  ----------------------  \n", BG_BWHITE | FG_BLACK); 
    puts("  1) Shutdown             \n", BG_BWHITE | FG_BLACK);  //  0x02
    puts("  ......................  \n", BG_BWHITE | FG_BLACK);
}
