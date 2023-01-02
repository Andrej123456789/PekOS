
#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../txt/puts.cc"
#include "../io/keyb.cc"
#include "../scr/sets.cc"

extern void texted_hanlder(uint8_t scancode, uint8_t c);

void texted()
{
    clear();
    MainKBHandler = texted_hanlder;
    set_cursor_pos(pos_from_cord(0,0));
    for (int i = 0; i < 80; i++) {
        puts(" ", BG_BLACK | FG_WHITE);
    }

    set_cursor_pos(pos_from_cord(0,0));
    puts(" |> Text EDitor : Version 1 <| ESC = quit \n", BG_BLACK | FG_WHITE);

    for (int l = 1; l < 79; l++) {
        set_cursor_pos(pos_from_cord(0,l));
        puts("^", BG_BLUE | FG_LBLUE);
    }

    set_cursor_pos(pos_from_cord(0,1));
}
