
#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../txt/puts.cc"
#include "../scr/sets.cc"
#include "../idt/idt.cc"
#include "../scr/menu.cc"

bool leftshift  =   false;
bool rightshift =   false;
uint8_t lastc;
extern void screen(uint8_t color = BG_BLUE | FG_WHITE);

void stdkeyb_handler(uint8_t scancode, uint8_t c)
{
        if (c != 0)
        {
            switch (leftshift | rightshift)
            {
                case true:
                    c = Set2::ScanCodeLookUp2[scancode];
                    putc(c);
                    break;
                case false:
                    putc(c);
                    break;
            }
        }

    else
    {
        switch (scancode)
        {
            case 0x8E:  // Backspace
                set_cursor_pos(cursor_pos - 1);
                putc(' ');
                set_cursor_pos(cursor_pos - 1);
                break;
            
            case 0x2A:  // Left Shift (Pressed)
                leftshift   =   true;
                break;

            case 0xAA:  // Left Shift (Released)
                leftshift   =   false;
                break;

            case 0x36:  // Right Shift (Pressed)
                rightshift  =   true;
                break;

            case 0xB6:  // Right Shift (Released)
                rightshift  =   false;
                break;
            
            case 0x9C:  //  Enter
                puts("\n");
                break;
        }
    }

    lastc = scancode;
}

void keyb_handler_0x0E(uint8_t scancode)
{
    switch (scancode)
    {
        case 0x50:
            set_cursor_pos(cursor_pos + VGA_W);
            break;
        
        case 0x48:
            set_cursor_pos(cursor_pos - VGA_W);
            break;
        
        case 0x4D:
            set_cursor_pos(cursor_pos + 1);
            break;

        case 0xCB:
            set_cursor_pos(cursor_pos - 1);
            break;

        default:
            break;
    }
}

void desktop_handler(uint8_t scancode, uint8_t c)
{
    switch (lastc)
    {
        case 0xE0:
            keyb_handler_0x0E(scancode);
            break;

        case 0x01:
            menu();
            break;

        case 0x02:
            screen();
            break;

        case 0x03:
            screen(BG_CYAN | FG_WHITE);
            break;

        case 0x04:
            screen(BG_RED | FG_WHITE);
            break;

        case 0x05:
            screen(BG_MAGENTA | FG_WHITE);
            break;

        case 0x06:
            screen(BG_BROWN | FG_WHITE);
            break;

    }
    lastc = scancode;
}


void texted_hanlder(uint8_t scancode, uint8_t c)
{
    switch (lastc)
    {
        case 0x01:
            MainKBHandler = desktop_handler;
            screen();
            break;

        default:
            stdkeyb_handler(scancode, c);
            break;
    }
    lastc = scancode;
}

void keyboard_handler(uint8_t scancode, uint8_t c)
{
    switch (lastc)
    {
        case 0xE0:
            keyb_handler_0x0E(scancode);
            break;
        
        case 0x01:
            menu();
            break;

        default:
            stdkeyb_handler(scancode, c);
    }
    lastc = scancode;
}
