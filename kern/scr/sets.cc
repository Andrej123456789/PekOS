
#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../txt/puts.cc"
#include "../io/keyb.cc"

void screen(uint8_t color = BG_BLUE | FG_WHITE)
{
    clear(color);
    
    set_cursor_pos(pos_from_cord(0,24));
    for (int i = 0; i < 80; i++) {
        puts(" ", BG_LGRAY | FG_WHITE);
    }

    /*set_cursor_pos(pos_from_cord(54,23));
    puts(" Pek OS \"Wheat\" | Beta 15 " , BG_BRED |  FG_WHITE);*/

    set_cursor_pos(pos_from_cord(59,24));
    puts("| 1 | 2 | 3 | 4 | 5 |", BG_BLACK | FG_YELLOW);

    set_cursor_pos(pos_from_cord(0,24));
    puts("  Start  ", BG_GREEN | FG_WHITE);

    set_cursor_pos(pos_from_cord(0, 0));
}
