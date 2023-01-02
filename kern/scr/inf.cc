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
    puts("|     Welcome to Wheat Beta 15!       |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 5));
    puts("| Wheat is a code-name project for    |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 6));
    puts("| PekOS version 9. Wheat is a x86_64  |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 7));
    puts("| operating system that targets BIOS  |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 8));
    puts("| PCs, it is written in a mix of C++  |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 9));
    puts("| and assembly. To close this window  |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 10));
    puts("| press the x / X, to move the cursor |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 11));
    puts("| use the arrow keys. To open the     |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 12));
    puts("| start menu press ESC key            |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 13));
    puts("| ................................... |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 14));
    puts("| If you spot any bugs / issues do    |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 15));
    puts("| report them!                        |\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(12, 16));
    puts("|_____________________________________|\n", BG_CYAN | FG_YELLOW);
    set_cursor_pos(pos_from_cord(0, 0));

}
