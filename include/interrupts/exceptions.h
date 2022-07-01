/*
 * exceptions.h: Functions/ISRs for exceptions (interrupts 0-31 in the IDT)
 */
#pragma once

#include "../interrupts/idt.h"
#include "../ports/io.h"
#include "../screen/clear_screen.h"
#include "../print/print_types.h"
#include "../gfx/2d_gfx.h"

__attribute__ ((interrupt)) void div_by_0_handler(int_frame_32_t *frame)
{
    uint16_t x = 0, y = 0;
    uint32_t color = user_gfx_info->fg_color;   // Save current text color

    user_gfx_info->fg_color = convert_color(0x00FF0000);          // Red
    print_string(&x, &y, "DIVIDE BY 0 ERROR - EXCEPTION HANDLED");

    user_gfx_info->fg_color = color;

    // Attempt to move on after the offending DIV/IDIV instruction by 
    //   incrementing instruction pointer
    frame->eip++;
}

__attribute__ ((interrupt)) void page_fault_handler(int_frame_32_t *frame, const uint32_t error_code)
{
    uint16_t x = 0, y = 0;
    uint32_t color = user_gfx_info->fg_color;   // Save current text color
    uint32_t bad_address = 0;

    user_gfx_info->bg_color = convert_color(0x000000FF);           // blue
    user_gfx_info->fg_color = convert_color(0xFFFFFFFF);          // White
    
    // Clear the screen
    clear_screen(user_gfx_info->bg_color);

    print_string(&x, &y, ":(\r\nPekOS has ran into a page fault exception and has crashed.\r\n");

    print_string(&x, &y, "      Page fault exception (#PF)\r\n      ERROR CODE: ");
    print_hex(&x, &y, error_code);

    // CR2 contains bad address that caused page fault
    __asm__ __volatile__("movl %%CR2, %0" : "=r"(bad_address) );

    print_string(&x, &y, "\r\n      ADDRESS: ");
    print_hex(&x, &y, bad_address);

    user_gfx_info->fg_color = color;

    // TODO: Replace with actual implementations for mapping in and setting present, read/write, etc.
    //   pages
    //__asm__ __volatile__("cli;hlt");

    print_string(&x, &y, "\r\nIt is recommended to restart your system...\r\n");
    __asm__ __volatile__("cli;hlt");

}




