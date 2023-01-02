#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../txt/puts.cc"
#include "../io/keyb.cc"

extern uint8_t lastc;

void infowind(uint8_t scancode, uint8_t c)
{
    switch (lastc)
    {
        case 0xE0:
            keyb_handler_0x0E(scancode);
            break;
        
        case 0x2D:
            screen();
            MainKBHandler = desktop_handler;
            break;

        case 0x22:
            screen();
            MainKBHandler = desktop_handler;
            break;
    }
    lastc = scancode;
}

void draw_inf()
{
    MainKBHandler = infowind;
    set_cursor_pos(pos_from_cord(12, 3));
    puts("|-----------------------------------|X|\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 4));
    puts("|     Welcome to Pek OS v9.0.1!       |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 5));
    puts("| PekOS 9.0.1 is a x86_64 OS written  |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 6));
    puts("| in a mix of C++ & assembly. PekOS   |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 7));
    puts("| 9.0.1 targets the BIOS PC platform. |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 8));
    puts("| ................................... |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 9));
    puts("|   ARROW KEYS  =   move the cursor   |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 10));
    puts("|   ESC         =   start menu        |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 11));
    puts("|   x / X       =   close this window |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 12));
    puts("|   1 - 5       =   change desktop    |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 13));
    puts("|_____________________________________|\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(0, 0));

}
