
#include <stddef.h>
#include <stdint.h>
#include "txt/puts.cc"
#include "scr/sets.cc"
#include "idt/idt.cc"
#include "io/keyb.cc"
#include "scr/inf.cc"
#include "scr/menu.cc"

extern const char Test[];

extern "C" void _start()
{

    clear();
    screen(BG_BLUE | FG_WHITE);

    init_idt();

    draw_inf();

    //MainKBHandler = keyboard_handler;

    return;
}
