// =========================================================================== 
// PekOS V7.X kernel
// ===========================================================================

#include "C/stdint.h"
#include "C/stdlib.h"
#include "C/string.h"
#include "C/time.h"
#include "global/global_addresses.h"
#include "gfx/2d_gfx.h"
#include "screen/clear_screen.h"
#include "print/print_types.h"
#include "screen/cursor.h"
#include "print/print_registers.h"
#include "memory/physical_memory_manager.h"
#include "memory/virtual_memory_manager.h"
#include "memory/malloc.h"
#include "disk/file_ops.h"
#include "print/print_fileTable.h"
#include "type_conversions/hex_to_ascii.h"
#include "interrupts/idt.h"
#include "interrupts/exceptions.h"
#include "interrupts/pic.h"
#include "interrupts/syscalls.h"
#include "ports/io.h"
#include "keyboard/keyboard.h"
#include "sound/pc_speaker.h"

void print_physical_memory_info(void);  // Print information from the physical memory map (SMAP)

// Physical memory map entry from Bios Int 15h EAX E820h
typedef struct SMAP_entry {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} __attribute__ ((packed)) SMAP_entry_t;

uint16_t kernel_cursor_x = 0;   // Text cursor X position
uint16_t kernel_cursor_y = 0;   // Text cursor Y position

__attribute__ ((section ("kernel_entry"))) void kernel_main(void)
{
    uint8_t tokens[50];             // tokens array, equivalent-ish to tokens[5][10]
    uint8_t *tokens_ptr = tokens;
    uint16_t tokens_length[5];      // length of each token in tokens array (0-10)
    uint16_t *tokens_length_ptr = tokens_length;
    uint8_t token_count;            // How many tokens did the user enter?
    uint8_t token_file_name1[11] = {0};   // Filename 1 for commands
    uint8_t token_file_name2[11] = {0};   // Filename 2 for commands

    // Users
    uint8_t *usr_root   = "ted\0";         // name of our beloved bear Ted (PekOS mascot) -- ***MASCOT WORK IN PROGRESS***
    uint8_t *usr_guest  = "Guest\0";      // Guest username
    char usr_usr[256]   = {0};         // User input for a username

    uint8_t *user;

    uint8_t *usr_domain  = "@PekOS\0";

    // Bool
    static bool is_root = false;
    static bool is_setup_done = false;

    // chars & strings

    char cmdString[256] = {0};         // User input string  
    char *cmdString_ptr = cmdString;

    uint8_t input_char   = 0;       // User input character
    uint8_t input_length;           // Length of user input
    uint16_t idx         = 0;
    uint8_t *cmdDir      = "ls\0";         // Directory command; list all files/pgms on disk
    uint8_t *cmdReboot   = "reboot\0";      // 'warm' reboot option
    uint8_t *cmdPrtreg   = "reg\0";      // Print register values
    uint8_t *cmdGfxtst   = "gfxtst\0";      // Graphics mode test
    uint8_t *cmdHlt      = "hlt\0";         // E(n)d current program by halting cpu
    uint8_t *cmdCls	     = "clr\0";         // Clear screen by scrolling
    uint8_t *cmdShutdown = "shutdown\0";    // Close QEMU emulator
    uint8_t *cmdEditor   = "editor\0";	    // Launch editor program
    uint8_t *cmdEcho     = "echo\0";        // prints out the user input
    uint8_t *cmdDelFile  = "rm\0";		    // Delete a file from disk
    uint8_t *cmdRenFile  = "rn\0";         // Rename a file in the file table
    uint8_t *cmdPrtmemmap = "memmap\0";  // Print physical memory map info
    uint8_t *cmdChgColors = "colors\0";  // Change current fg/bg colors
    uint8_t *cmdChgFont   = "fonts\0";    // Change current font
    uint8_t *cmdSleep     = "sleep\0";      // Sleep for a # of seconds
    uint8_t *cmdMSleep    = "msleep\0";     // Sleep for a # of milliseconds
    uint8_t *cmdShowDateTime = "date\0";  // Show CMOS RTC date/time values
    uint8_t *cmdSoundTest = "sound\0";  // Test pc speaker square wave sound
    uint8_t *cmdOSVer     = "ver\0";    // print out OS info
    uint8_t *cmdHelp      = "help\0";   // help message / command
    uint8_t *cmdwho       = "whois\0";  // display name of the user
    uint8_t *cmdroot      = "root\0";   // enter root mode
    uint8_t fileExt[4];
    uint8_t *fileBin = "bin \0";
    uint8_t *fileTxt = "txt \0";
    uint8_t fileSize = 0;
    uint8_t *file_ptr;
    uint8_t *windowsMsg     = "\r\n" "Oops! Something went wrong :(" "\r\n\0";
    uint8_t *notFoundString = "\r\n" "[!!] Program/file not found!, Try again? (Y)" "\r\n\0";
    uint8_t *sectNotFound   = "\r\n" "[!!] Sector not found!, Try again? (Y)" "\r\n\0";
    uint8_t *menuString     = "\r\n------------------------------------------\r\n"
                              "   PekOS 7.2 ''Service Pack 1'' Booted   \r\n"
                              "------------------------------------------\n\r\n\0";
    uint8_t *failure        = "\r\n" "[!!] Command/Program not found, Try again" "\r\n\0";
    uint8_t *prompt         = "] \0";
    uint8_t *pgmNotLoaded   = "\r\n" "[!!] Program found but not loaded, Try Again" "\r\n\0";
    uint8_t *fontNotFound   = "\r\n" "[!!] Font not found!" "\r\n\0";

    uint32_t needed_blocks, needed_pages;
    uint32_t *allocated_address;
    uint8_t font_width  = *(uint8_t *)FONT_WIDTH;
    uint8_t font_height = *(uint8_t *)FONT_HEIGHT;

    // --------------------------------------------------------------------
    // Initial hardware, interrupts, etc. setup
    // --------------------------------------------------------------------
    // Get & set current kernel page directory
    current_page_directory = (page_directory *)*(uint32_t *)CURRENT_PAGE_DIR_ADDRESS;

    // Set physical memory manager variables
    memory_map  = (uint32_t *)MEMMAP_AREA;
    max_blocks  = *(uint32_t *)PHYS_MEM_MAX_BLOCKS;
    used_blocks = *(uint32_t *)PHYS_MEM_USED_BLOCKS;

    // Set up interrupts
    init_idt_32();  

    // Set up exception handlers (ISRs 0-31)
    set_idt_descriptor_32(0, div_by_0_handler, TRAP_GATE_FLAGS); // Divide by 0 error #DE, ISR 0
    set_idt_descriptor_32(14, page_fault_handler, TRAP_GATE_FLAGS); // Page fault #PF errors, ISR 14
    
    // Set up software interrupts
    set_idt_descriptor_32(0x80, syscall_dispatcher, INT_GATE_USER_FLAGS);  // System call handler/dispatcher

    // Mask off all hardware interrupts (disable the PIC)
    disable_pic();

    // Remap PIC interrupts to after exceptions (PIC1 starts at 0x20)
    remap_pic();

    // Add ISRs for PIC hardware interrupts
    set_idt_descriptor_32(0x20, timer_irq0_handler, INT_GATE_FLAGS);  
    set_idt_descriptor_32(0x21, keyboard_irq1_handler, INT_GATE_FLAGS);
    set_idt_descriptor_32(0x28, cmos_rtc_irq8_handler, INT_GATE_FLAGS);
    
    // Clear out PS/2 keyboard buffer: check status register and read from data port
    //   until clear
    // This may fix not getting keys on boot
    while (inb(0x64) & 1) inb(0x60);    // Status register 0x64 bit 0 is set, output buffer is full

    // Enable PIC IRQ interrupts after setting their descriptors
    clear_irq_mask(0); // Enable timer (will tick every ~18.2/s)
    clear_irq_mask(1); // Enable keyboard IRQ1, keyboard interrupts
    clear_irq_mask(2); // Enable PIC2 line
    clear_irq_mask(8); // Enable CMOS RTC IRQ8
    
    // Enable CMOS RTC
    show_datetime = false;  // Don't show date/time on boot 
    enable_rtc();

    // Set default PIT Timer IRQ0 rate - ~1000hz
    // 1193182 MHZ / 1193 = ~1000
    set_pit_channel_mode_frequency(0, 2, 1193);

    // start up sound
    enable_pc_speaker();
    #include "../include/sound/start.mus"
    disable_pc_speaker();

    // After setting up hardware interrupts & PIC, set IF to enable 
    //   non-exception and not NMI hardware interrupts
    __asm__ __volatile__("sti");

    // Set intial colors
    while (!user_gfx_info->fg_color) {
        if (gfx_mode->bits_per_pixel > 8) {
            user_gfx_info->fg_color = convert_color(0x0000FF00);
            user_gfx_info->bg_color = convert_color(0x00000000);
        } else {
            // Assuming VGA palette
            user_gfx_info->fg_color = convert_color(0x02);
            user_gfx_info->bg_color = convert_color(0x00);
        }
    }

    // Clear the screen
    clear_screen(user_gfx_info->bg_color);

    // Print OS boot message
    print_string(&kernel_cursor_x, &kernel_cursor_y, "");
    print_string(&kernel_cursor_x, &kernel_cursor_y, menuString);

    // --------------------------------------------------------------------
    // Get user input, print to screen & run command/program  
    // --------------------------------------------------------------------
    while (1) {
        // Reset tokens data, arrays, and variables for next input line
        memset(tokens, 0, 50);
        memset(tokens_length, 0, 10);
        token_count = 0;
        memset(token_file_name1, 0 , 10);
        memset(token_file_name2, 0 , 10);
        memset(cmdString, 0 , 255);

        memset(usr_usr, 0, 255);

        // Print prompt

        if (is_root == true)
            print_string(&kernel_cursor_x, &kernel_cursor_y, usr_root);

        else if (is_setup_done == true)
            print_string(&kernel_cursor_x, &kernel_cursor_y, usr_usr);

        else if (is_setup_done == false  && is_root == false)
            print_string(&kernel_cursor_x, &kernel_cursor_y, usr_guest);
            print_string(&kernel_cursor_x, &kernel_cursor_y, usr_domain);
        

        print_string(&kernel_cursor_x, &kernel_cursor_y, prompt);
        move_cursor(kernel_cursor_x, kernel_cursor_y);
        
        input_length = 0;   // reset byte counter of input

        // Check the user
        if (is_root == false)
            user = usr_guest;

        else if (is_root == true)
            user = usr_root;

        // Key loop - get input from user
        while (1) {
            input_char = get_key();     // Get ascii char from scancode from keyboard data port 60h

            if (input_char == 0x0D) {   // enter key?
                remove_cursor(kernel_cursor_x, kernel_cursor_y);
                break;                  // go on to tokenize user input line
            }

            // TODO: Move all input back 1 char/byte after backspace, if applicable
            if (input_char == 0x08) {       // backspace?
                if (input_length > 0) {                // Did user input anything?
                    input_length--;                     // yes, go back one char in cmdString
                    cmdString[input_length] = '\0';    // Blank out character    					
                }

                if (kernel_cursor_x > 2) {     // At start of line (at prompt)?
                    // TODO: change to use remove_cursor, move back 1 space, print a space, move back 1 space
                    // Move cursor back 1 space
                    kernel_cursor_x--;

                    // Print 2 spaces at cursor
                    print_char(&kernel_cursor_x, &kernel_cursor_y, 0x20);
                    print_char(&kernel_cursor_x, &kernel_cursor_y, 0x20);

                    // Move cursor back 2 spaces
                    kernel_cursor_x -= 2;
                    move_cursor(kernel_cursor_x, kernel_cursor_y);
                }

                continue;   // Get next character
            }

            cmdString[input_length] = input_char;   // Store input char to string
            input_length++;                         // Increment byte counter of input

            // Print input character to screen
            print_char(&kernel_cursor_x, &kernel_cursor_y, input_char);
            move_cursor(kernel_cursor_x, kernel_cursor_y);
        }

        if (input_length == 0) {
            // No input or command not found! boo D:
            print_string(&kernel_cursor_x, &kernel_cursor_y, failure);
            move_cursor(kernel_cursor_x, kernel_cursor_y);

            continue;
        }

        cmdString[input_length] = '\0';     // else null terminate cmdString from si

        // Tokenize input string "cmdString" into separate tokens
        cmdString_ptr     = cmdString;      // Reset pointers...
        tokens_ptr        = tokens;
        tokens_length_ptr = tokens_length;

        // Get token loop
        while (*cmdString_ptr != '\0') { 

            // Skip whitespace between tokens
            while (*cmdString_ptr == ' ') cmdString_ptr++;

            // If alphanumeric or underscore, add to current token
            while ((*cmdString_ptr >= '0' && *cmdString_ptr <= '9') || 
                  (*cmdString_ptr >= 'A' && *cmdString_ptr <= 'Z')  ||
                  (*cmdString_ptr == '_')                           ||
                  (*cmdString_ptr >= 'a' && *cmdString_ptr <= 'z')) {

                *tokens_ptr++ = *cmdString_ptr++;
                (*tokens_length_ptr)++;     // increment length of current token
            } 

            // Go to next token
            token_count++;                              // Next token
            tokens_ptr = tokens + (token_count * 10);   // Next position in tokens array
            tokens_length_ptr++;                        // Next position in tokens_length array
        }

        // Check commands 
        // Get first token (command to run) & second token (if applicable e.g. file name)
        if (strncmp(tokens, cmdDir, strlen(cmdDir)) == 0) {
            // --------------------------------------------------------------------
            // File/Program browser & loader   
            // --------------------------------------------------------------------
            print_fileTable(&kernel_cursor_x, &kernel_cursor_y);
            continue;
        }

        if (strncmp(tokens, cmdwho, strlen(cmdwho)) == 0)
        {
            
            if (is_root == true)
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nted\r\n");
            
            else
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nGuest\r\n");

            continue;
        }

        if (strncmp(tokens, cmdroot, strlen(cmdroot)) == 0)
        {
            is_root = true;
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
            continue;
        }

        if (strncmp(tokens, cmdOSVer, strlen(cmdOSVer)) == 0)
        {
            // --------------------------------------------------------------------
            // Print out the information about the OS
            // --------------------------------------------------------------------

            uint32_t num_entries = *(uint32_t *)0x8500;         // Number of SMAP entries
            SMAP_entry_t *SMAP_entry = (SMAP_entry_t *)0x8504;  // Memory map entries start point

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nUser > ");
            print_string(&kernel_cursor_x, &kernel_cursor_y, user);
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nOS Info >>\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "PekOS Version    :: 7.2 ''Service Pack 1''\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "Kernel Version   :: 7.X\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "PekOS Bootloader :: V7\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nMemory Info >>\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "Total memory in bytes: ");

            SMAP_entry--;   // Get last SMAP entry
            print_hex(&kernel_cursor_x, &kernel_cursor_y, SMAP_entry->base_address + SMAP_entry->length - 1);
        
            // Print out memory manager block info:
            //   total memory in 4KB blocks, total # of used blocks, total # of free blocks
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nTotal 4KB blocks: ");
            print_dec(&kernel_cursor_x, &kernel_cursor_y, max_blocks);

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nUsed or reserved blocks: ");
            print_dec(&kernel_cursor_x, &kernel_cursor_y, used_blocks);

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nFree or available blocks: ");
            print_dec(&kernel_cursor_x, &kernel_cursor_y, max_blocks - used_blocks);

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n \r\n");

            continue;

        }

        if (strncmp(tokens, cmdHelp, strlen(cmdHelp)) == 0)
        {
            // --------------------------------------------------------------------
            // Help command 
            // --------------------------------------------------------------------

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nHelp >>\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   ls       -     print out file table\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   reboot   -     reboot the system\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   reg      -     print out the registers\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   gfxtst   -     graphics test\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   hlt      -     halt the system\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   clr      -     clear the screen\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   shutdown -     shuts down the system\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   rm       -     removes a file\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   rn       -     rename a file\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   memmap   -     print out the memory map\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   colors   -     color settings\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   fonts    -     font settings\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   sleep    -     halt the system for X seconds\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   msleep   -     halt the system for X miliseconds\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   date     -     display time & date\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   sound    -     play a sound file (*.mus)\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   ver      -     OS info & ver\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   whois    -     display name of the user\r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "   root     -     enter the root mode\r\n");
            continue;

        }

        if (strncmp(tokens, cmdReboot, strlen(cmdReboot)) == 0) {
            // --------------------------------------------------------------------
            // Reboot: Reboot the PC
            // --------------------------------------------------------------------
            outb(0x64, 0xFE);  // Send "Reset CPU" command to PS/2 keyboard controller port
        }

        if (strncmp(tokens, cmdPrtreg, strlen(cmdPrtreg)) == 0) {
            // --------------------------------------------------------------------
            // Print Register Values
            // --------------------------------------------------------------------
            print_registers(&kernel_cursor_x, &kernel_cursor_y); 

            continue;   
        }

        if (strncmp(tokens, cmdGfxtst, strlen(cmdGfxtst)) == 0) {
            // --------------------------------------------------------------------
            // Graphics Mode Test(s)
            // --------------------------------------------------------------------
            clear_screen(convert_color(LIGHT_GRAY));

            Point p0, p1, p2;
            Point vertex_array[6];

            // Draw pixel test
            for (uint8_t i = 0; i < 100; i ++) {
                for (uint8_t j = 0; j < 100; j++) {
                    p0.X = 200 + j;
                    p0.Y = 200 + i;
                    draw_pixel(p0.X, p0.Y, convert_color(RED));
                }
            } 

            // Draw line tests
            // Horizontal line
            p0.X = 1920/2 - 100;
            p0.Y = 1080/2 - 100;
            p1.X = 1920/2;
            p1.Y = 1080/2 - 100;
            draw_line(p0, p1, convert_color(BLACK));

            // Vertical line
            p0.X = 1920/2 + 100;
            p0.Y = 1080/2 - 100;
            p1.X = 1920/2 + 100;
            p1.Y = 1080/2 + 100;
            draw_line(p0, p1, convert_color(PURPLE));

            // Vertical line
            p0.X = 1920/2 + 200;
            p0.Y = 1080/2 - 200;
            p1.X = 1920/2 + 200;
            p1.Y = 1080/2 + 200;
            draw_line(p0, p1, convert_color(PURPLE));
            
            // Diagonal line up-right
            p0.X = 1920/2 + 150;
            p0.Y = 1080/2;
            p1.X = 1920/2 + 300;
            p1.Y = 1080/2 - 100;
            draw_line(p0, p1, convert_color(DARK_GRAY));
            
            // Diagonal line down-right
            p0.X = 1920/2 + 350;
            p0.Y = 1080/2 - 100;
            p1.X = 1920/2 + 500;
            p1.Y = 1080/2 + 100;
            draw_line(p0, p1, convert_color(BLUE));

            // Draw triangle test - right angle
            p0.X = 1920/2 - 600;
            p0.Y = 1080/2 + 100;
            p1.X = 1920/2 - 600;
            p1.Y = 1080/2 + 200;
            p2.X = 1920/2 - 450;
            p2.Y = 1080/2 + 200;
            draw_triangle(p0, p1, p2, convert_color(0x00FF7233));  // Sort of "burnt orange"

            // Draw rectangle test
            p0.X = 1920/2 - 400;
            p0.Y = 1080/2 + 100;
            p1.X = 1920/2 - 100;
            p1.Y = 1080/2 + 200;
            draw_rect(p0, p1, convert_color(0x0033CEFF));  // Kind of teal maybe

             // Draw rectangle test
            p0.X = 1920/2 - 10;
            p0.Y = 1080/2 + 10;
            p1.X = 1920/2 - 10;
            p1.Y = 1080/2 + 10;
            draw_rect(p0, p1, convert_color(0x0033CEFF));  // Kind of teal maybe

            // Draw polygon test - hexagon
            vertex_array[0].X = 1920/2;
            vertex_array[0].Y = 1080/2 + 100;
            vertex_array[1].X = 1920/2 + 100;
            vertex_array[1].Y = 1080/2 + 100;
            vertex_array[2].X = 1920/2 + 200;
            vertex_array[2].Y = 1080/2 + 150;
            vertex_array[3].X = 1920/2 + 100;
            vertex_array[3].Y = 1080/2 + 200;
            vertex_array[4].X = 1920/2;
            vertex_array[4].Y = 1080/2 + 200;
            vertex_array[5].X = 1920/2 - 100;
            vertex_array[5].Y = 1080/2 + 150;
            draw_polygon(vertex_array, 6, convert_color(RED));

            // Draw circle test
            p0.X = 1920/2 + 350;
            p0.Y = 1080/2 + 200;
            draw_circle(p0, 50, convert_color(BLUE));
            
            // Draw ellipse test
            p0.X = 1920/2 - 600;
            p0.Y = 1080/2 + 350;
            draw_ellipse(p0, 100, 50, convert_color(GREEN - 0x00005500));

            // Fill triangle test
            p0.X = 1920/2 - 400;
            p0.Y = 1080/2 + 300;
            p1.X = 1920/2 - 500;
            p1.Y = 1080/2 + 350;
            p2.X = 1920/2 - 420;
            p2.Y = 1080/2 + 250;
            fill_triangle_solid(p0, p1, p2, convert_color(0x006315FD));  // Some sort of dark purple

            // Fill rectangle test
            p0.X = 1920/2 - 350;
            p0.Y = 1080/2 + 300;
            p1.X = 1920/2 - 150;
            p1.Y = 1080/2 + 350;
            fill_rect_solid(p0, p1, convert_color(0x0015FDCD));  // Mintish greenish

            // Fill polygon test - hexagon
            vertex_array[0].X = 1920/2 - 50;
            vertex_array[0].Y = 1080/2 + 250;
            vertex_array[1].X = 1920/2 + 50;
            vertex_array[1].Y = 1080/2 + 250;
            vertex_array[2].X = 1920/2 + 100;
            vertex_array[2].Y = 1080/2 + 300;
            vertex_array[3].X = 1920/2 + 50;
            vertex_array[3].Y = 1080/2 + 350;
            vertex_array[4].X = 1920/2 - 50;
            vertex_array[4].Y = 1080/2 + 350;
            vertex_array[5].X = 1920/2 - 100;
            vertex_array[5].Y = 1080/2 + 300;
            fill_polygon_solid(vertex_array, 6, convert_color(0x00FD9B15));  // Light orangey

            // Fill circle test
            p0.X = 1920/2 + 200;
            p0.Y = 1080/2 + 270;
            fill_circle_solid(p0, 50, convert_color(0x00FF0FE6));  // Magentish fuschish

            // Fill ellipse test
            p0.X = 1920/2 + 400;
            p0.Y = 1080/2 + 350;
            fill_ellipse_solid(p0, 100, 50, convert_color(0x00FFEE0F));  // I Love GOOOOOOoooolllddd

            input_char = get_key(); 
            clear_screen(user_gfx_info->bg_color);  
            kernel_cursor_x = 0;
            kernel_cursor_y = 0;

            continue;
        }

        if (strncmp(tokens, cmdHlt, strlen(cmdHlt)) == 0) {
            // --------------------------------------------------------------------
            // End Program  
            // --------------------------------------------------------------------
            // __asm ("cli");   // Clear interrupts
            __asm ("hlt");   // Halt cpu
        }

        if (strncmp(tokens, cmdCls, strlen(cmdCls)) == 0) {
            // --------------------------------------------------------------------
            // Clear Screen
            // --------------------------------------------------------------------
            clear_screen(user_gfx_info->bg_color); 

            // Update cursor values for new position
            kernel_cursor_x = 0;
            kernel_cursor_y = 0;

            continue;
        }

        if (strncmp(tokens, cmdShutdown, strlen(cmdShutdown)) == 0) {
            // --------------------------------------------------------------------
            // Shutdown (QEMU)
            // --------------------------------------------------------------------
            __asm ("outw %%ax, %%dx" : : "a"(0x2000), "d"(0x604) );

            // TODO: Get shutdown command for bochs, user can uncomment which one they want to use
        }

        if (strncmp(tokens, cmdDelFile, strlen(cmdDelFile)) == 0) {
            // --------------------------------------------------------------------
            // Delete a file from the disk
            // --------------------------------------------------------------------
            //	Input 1 - File name to delete
            //	      2 - Length of file name
            // File name is 2nd token in array, each token is 10 char max
            tokens_ptr = tokens + 10;
            for (idx = 0; idx < tokens_length[1]; idx++)
                token_file_name1[idx] = *tokens_ptr++; 

            if (delete_file(token_file_name1, tokens_length[1]) != 0) {
                //	;; TODO: Add error message or code here
            }

            // Print newline when done
            print_char(&kernel_cursor_x, &kernel_cursor_y, 0x0A); 
            print_char(&kernel_cursor_x, &kernel_cursor_y, 0x0D); 

            continue;
        }

        if (strncmp(tokens, cmdRenFile, strlen(cmdRenFile)) == 0) {
            // --------------------------------------------------------------------
            // Rename a file in the file table
            // --------------------------------------------------------------------
            // 1 - File to rename
            // 2 - Length of name to rename
            // 3 - New file name // 4 - New file name length
            // File name is 2nd token in array, each token is 10 char max
            for (idx = 0; idx < tokens_length[1]; idx++)
                token_file_name1[idx] = tokens[10+idx]; 

            // New file name is 3rd token in array, each token is 10 char max
            for (idx = 0; idx < tokens_length[2]; idx++)
                token_file_name2[idx] = tokens[20+idx];

            if (rename_file(token_file_name1, tokens_length[1], token_file_name2, tokens_length[2]) != 0) {
                //	;; TODO: Add error message or code here
            }

            // Print newline when done
            print_char(&kernel_cursor_x, &kernel_cursor_y, 0x0A); 
            print_char(&kernel_cursor_x, &kernel_cursor_y, 0x0D); 

            continue;
        }

        // Print memory map command
        if (strncmp(tokens, cmdPrtmemmap, strlen(cmdPrtmemmap)) == 0) {
            // Print out physical memory map info
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n-------------------\r\nPhysical Memory Map"
                                                             "\r\n-------------------\r\n\r\n");
            print_physical_memory_info();
            continue;
        }

        // Change colors command
        if (strncmp(tokens, cmdChgColors, strlen(cmdChgColors)) == 0) {
            uint32_t fg_color = 0;
            uint32_t bg_color = 0;

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Current colors (32bpp ARGB):");
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Foregound: ");
            print_hex(&kernel_cursor_x, &kernel_cursor_y, user_gfx_info->fg_color);
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Backgound: ");
            print_hex(&kernel_cursor_x, &kernel_cursor_y, user_gfx_info->bg_color);

            // Foreground color
            if (gfx_mode->bits_per_pixel > 8)
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "New Foregound (0xRRGGBB): 0x");
            else
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "New Foregound (0xNN): 0x");

            move_cursor(kernel_cursor_x, kernel_cursor_y);

            while ((input_char = get_key()) != 0x0D) {
                if (input_char >= 'a' && input_char <= 'f') input_char -= 0x20; // Convert lowercase to Uppercase

                print_char(&kernel_cursor_x, &kernel_cursor_y, input_char);
                move_cursor(kernel_cursor_x, kernel_cursor_y);

                fg_color *= 16;
                if      (input_char >= '0' && input_char <= '9') fg_color += input_char - '0';          // Convert hex ascii 0-9 to integer
                else if (input_char >= 'A' && input_char <= 'F') fg_color += (input_char - 'A') + 10;   // Convert hex ascii 10-15 to integer
            }

            remove_cursor(kernel_cursor_x, kernel_cursor_y);

            // Background color
            if (gfx_mode->bits_per_pixel > 8)
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "New Backgound (0xRRGGBB): 0x");
            else
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "New Backgound (0xNN): 0x");

            move_cursor(kernel_cursor_x, kernel_cursor_y);

            while ((input_char = get_key()) != 0x0D) {
                if (input_char >= 'a' && input_char <= 'f') input_char -= 0x20; // Convert lowercase to Uppercase

                print_char(&kernel_cursor_x, &kernel_cursor_y, input_char);
                move_cursor(kernel_cursor_x, kernel_cursor_y);

                bg_color *= 16;

                if      (input_char >= '0' && input_char <= '9') bg_color += input_char - '0';          // Convert hex ascii 0-9 to integer
                else if (input_char >= 'A' && input_char <= 'F') bg_color += (input_char - 'A') + 10;   // Convert hex ascii 10-15 to integer
            }

            remove_cursor(kernel_cursor_x, kernel_cursor_y);

            user_gfx_info->fg_color = convert_color(fg_color);  // Convert colors first before setting new values
            user_gfx_info->bg_color = convert_color(bg_color);

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");

            continue;
        }

        // Change font command; Usage: chgFont <name of font>
        if (strncmp(tokens, cmdChgFont, strlen(cmdChgFont)) == 0) {
            // First check if font exists - name is 2nd token
            file_ptr = check_filename(tokens+10, tokens_length[1]);
            if (*file_ptr == 0) {  
                print_string(&kernel_cursor_x, &kernel_cursor_y, fontNotFound);  // File not found in filetable, error
                move_cursor(kernel_cursor_x, kernel_cursor_y);

                continue;
            }

            // Check if file has .fnt extension
            if (strncmp(file_ptr+10, "fnt", 3) != 0) {
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n[!!] file is not a font\r\n"); 
                move_cursor(kernel_cursor_x, kernel_cursor_y);

                continue;
            }

            // Save file name
            strncpy(token_file_name1, file_ptr, 10);
            token_file_name1[10] = '\0';

            // File is a valid font, try to load it to memory
            if (!load_file(tokens+10, tokens_length[1], (uint32_t)FONT_ADDRESS, fileExt)) {
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n[!!] file could not be loaded\r\n"); 
                move_cursor(kernel_cursor_x, kernel_cursor_y);

                continue;
            }

            // New font should be loaded and in use now
            font_width  = *(uint8_t *)FONT_WIDTH;
            font_height = *(uint8_t *)FONT_HEIGHT;
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nFont loaded: \r\n");
            print_string(&kernel_cursor_x, &kernel_cursor_y, token_file_name1);
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nWidth: ");
            print_dec(&kernel_cursor_x, &kernel_cursor_y, font_width);
            print_string(&kernel_cursor_x, &kernel_cursor_y, " Height: ");
            print_dec(&kernel_cursor_x, &kernel_cursor_y, font_height);
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");

            move_cursor(kernel_cursor_x, kernel_cursor_y);

            continue;
        }

        // Sleep command - sleep for given number of seconds
        if (strncmp(tokens, cmdSleep, strlen(cmdSleep)) == 0) {
            sleep_seconds(atoi(tokens+10)); 

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
            continue;
        }

        // MSleep command - sleep for given number of milliseconds
        if (strncmp(tokens, cmdMSleep, strlen(cmdMSleep)) == 0) {
            sleep_milliseconds(atoi(tokens+10));

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
            continue;
        }

        // Show CMOS RTC date/time values
        if (strncmp(tokens, cmdShowDateTime, strlen(cmdShowDateTime)) == 0) {
            show_datetime = !show_datetime;  

            if (!show_datetime) {
                // Blank out date/time
                uint16_t x = 50, y = 50;
                print_string(&x, &y, "                   "); // Overwrite date/time with spaces
            }

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
            continue;
        }

        // Test Sound
        if (strncmp(tokens, cmdSoundTest, strlen(cmdSoundTest)) == 0) {
            enable_pc_speaker();

            #include "../include/sound/sound.mus"

            disable_pc_speaker();

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
            continue;
        }

        // If command not input, search file table entries for user input file
        file_ptr = check_filename(cmdString, tokens_length[0]);
        if (*file_ptr == 0) {  
            print_string(&kernel_cursor_x, &kernel_cursor_y, failure);  // File not found in filetable, error
            move_cursor(kernel_cursor_x, kernel_cursor_y);

            continue;
        }

        // file_ptr is pointing to filetable entry, get number of pages needed to load the file
        //   num_pages = (file size in sectors * # of bytes in a sector) / size of a page in bytes
        needed_pages = (file_ptr[15] * 512) / PAGE_SIZE;  // Convert file size in bytes to pages
        if ((file_ptr[15] * 512) % PAGE_SIZE > 0) needed_pages++;   // Allocate extra page for partial page of memory

        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Allocating ");
        print_dec(&kernel_cursor_x, &kernel_cursor_y, needed_pages);
        print_string(&kernel_cursor_x, &kernel_cursor_y, " page(s)\r\n");
        
        // Load files/programs to this starting address
        const uint32_t entry_point = 0x400000;  // Put entry point right after identity mapped 4MB page table

        // Allocate & map pages
        for (uint32_t i = 0; i < needed_pages; i++) {
            pt_entry page = 0;
            uint32_t phys_addr = (uint32_t)allocate_page(&page);

            if (!map_page((void *)phys_addr, (void *)(entry_point + i*PAGE_SIZE)))
                print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n[!!] Couldn't map pages\r\n[??] Might be out of memory?\r\n");
        }

        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Allocated to virtual address ");
        print_hex(&kernel_cursor_x, &kernel_cursor_y, entry_point);
        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
        
        // Call load_file function to load program/file to starting memory address
        // Input 1: File name (address)
        //       2: File name length
        //       3: Memory offset to load file to
        //       4: File extension variable
        // Return value - 0 = Success, !0 = error
        if (!load_file(cmdString, tokens_length[0], entry_point, fileExt)) {
            // Error, program did not load correctly
            print_string(&kernel_cursor_x, &kernel_cursor_y, pgmNotLoaded);
            move_cursor(kernel_cursor_x, kernel_cursor_y);

            continue;
        }

        // Check file extension in file table entry, if 'bin'/binary, far jump & run
        if (strncmp(fileExt, fileBin, 3) == 0) {
            // Reset malloc variables before calling program
            malloc_list_head    = 0;    // Start of linked list
            malloc_virt_address = entry_point + (needed_pages * PAGE_SIZE);
            malloc_phys_address = 0;
            total_malloc_pages  = 0;

            // Void function pointer to jump to and execute code at specific address in C
            ((void (*)(void))entry_point)();     // Execute program, this can return

            // If used malloc(), free remaining memory to prevent memory leaks
            for (uint32_t i = 0, virt = malloc_virt_address; i < total_malloc_pages; i++, virt += PAGE_SIZE) {
                pt_entry *page = get_page(virt);

                if (PAGE_PHYS_ADDRESS(page) && TEST_ATTRIBUTE(page, PTE_PRESENT)) {
                    free_page(page);
                    unmap_page((uint32_t *)virt);
                    flush_tlb_entry(virt);  // Invalidate page as it is no longer present
                }
            }

            // Reset malloc variables after calling program
            malloc_list_head    = 0;    // Start of linked list
            malloc_virt_address = 0;
            malloc_phys_address = 0;
            total_malloc_pages  = 0;
            
            // TODO: In the future, if using a backbuffer, restore screen data from that buffer here instead
            //  of clearing
            
            // Clear the screen before going back
            clear_screen(user_gfx_info->bg_color);  

            // Reset cursor position
            kernel_cursor_x = 0;
            kernel_cursor_y = 0;

            // Free previously allocated pages when done
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Freeing ");
            print_dec(&kernel_cursor_x, &kernel_cursor_y, needed_pages);
            print_string(&kernel_cursor_x, &kernel_cursor_y, " page(s)\r\n");
            
            for (uint32_t i = 0, virt = entry_point; i < needed_pages; i++, virt += PAGE_SIZE) {
                pt_entry *page = get_page(virt);

                if (PAGE_PHYS_ADDRESS(page) && TEST_ATTRIBUTE(page, PTE_PRESENT)) {
                    free_page(page);
                    unmap_page((uint32_t *)virt);
                    flush_tlb_entry(virt);  // Invalidate page as it is no longer present or mapped
                }
            }

            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Freed at address ");
            print_hex(&kernel_cursor_x, &kernel_cursor_y, entry_point);
            print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");

            continue;   // Loop back to prompt for next input
        }

        // Else print text file to screen
        // TODO: Put this behind a "shell" command like 'typ'/'type' or other
        file_ptr = (uint8_t *)entry_point;   // File location to print from

        // Print newline first
        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
        
        // Get size of filesize in bytes (512 byte per sector)
        // TODO: Change this later - currently assuming text files are only 1 sector
        //	<fileSize> * 512 
        
        // print_file_char:
        for (idx = 0; idx < 512; idx++) {
            // TODO: Handle newlines (byte 0x0A in txt file data)
            if (*file_ptr <= 0x0F)          // Convert to hex
                *file_ptr = hex_to_ascii(*file_ptr);

            // Print file character to screen
            print_char(&kernel_cursor_x, &kernel_cursor_y, *file_ptr);

            file_ptr++;
        }

        // Print newline after printing file contents
        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");

        // Free pages when done
        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Freeing ");
        print_dec(&kernel_cursor_x, &kernel_cursor_y, needed_pages);
        print_string(&kernel_cursor_x, &kernel_cursor_y, " page(s)\r\n");
        
        for (uint32_t i = 0, virt = entry_point; i < needed_pages; i++, virt += PAGE_SIZE) {
            pt_entry *page = get_page(virt);

            if (PAGE_PHYS_ADDRESS(page) && TEST_ATTRIBUTE(page, PTE_PRESENT)) {
                free_page(page);
                unmap_page((uint32_t *)virt);
                flush_tlb_entry(virt);  // Invalidate page as it is no longer present or mapped
            }
        }

        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Freed at address ");
        print_hex(&kernel_cursor_x, &kernel_cursor_y, entry_point);
        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
    }
}

// Print information from the physical memory map (SMAP)
void print_physical_memory_info(void)  
{
    uint32_t num_entries = *(uint32_t *)0x8500;         // Number of SMAP entries
    SMAP_entry_t *SMAP_entry = (SMAP_entry_t *)0x8504;  // Memory map entries start point

    for (uint32_t i = 0; i < num_entries; i++) {
        print_string(&kernel_cursor_x, &kernel_cursor_y, "Region ");
        print_hex(&kernel_cursor_x, &kernel_cursor_y, i);

        print_string(&kernel_cursor_x, &kernel_cursor_y, ": base: ");
        print_hex(&kernel_cursor_x, &kernel_cursor_y, SMAP_entry->base_address);

        print_string(&kernel_cursor_x, &kernel_cursor_y, " length: ");
        print_hex(&kernel_cursor_x, &kernel_cursor_y, SMAP_entry->length);

        print_string(&kernel_cursor_x, &kernel_cursor_y, " type: ");
        print_hex(&kernel_cursor_x, &kernel_cursor_y, SMAP_entry->type);

        switch(SMAP_entry->type) {
            case 1:
                print_string(&kernel_cursor_x, &kernel_cursor_y, " (Available)");
                break;
            case 2: 
                print_string(&kernel_cursor_x, &kernel_cursor_y, " (Reserved)");
                break;
            case 3: 
                print_string(&kernel_cursor_x, &kernel_cursor_y, " (ACPI Reclaim)");
                break;
            case 4: 
                print_string(&kernel_cursor_x, &kernel_cursor_y, " (ACPI NVS Memory)");
                break;
            default: 
                print_string(&kernel_cursor_x, &kernel_cursor_y, " (Reserved)");
                break;
        }

        print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
        SMAP_entry++;   // Go to next entry
    }

    // Print total amount of memory
    print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n");
    print_string(&kernel_cursor_x, &kernel_cursor_y, "Total memory in bytes: ");

    SMAP_entry--;   // Get last SMAP entry
    print_hex(&kernel_cursor_x, &kernel_cursor_y, SMAP_entry->base_address + SMAP_entry->length - 1);
  
    // Print out memory manager block info:
    //   total memory in 4KB blocks, total # of used blocks, total # of free blocks
    print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nTotal 4KB blocks: ");
    print_dec(&kernel_cursor_x, &kernel_cursor_y, max_blocks);

    print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\nUsed or reserved blocks: ");
    print_dec(&kernel_cursor_x, &kernel_cursor_y, used_blocks);

    print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n" "Free or available blocks: ");
    print_dec(&kernel_cursor_x, &kernel_cursor_y, max_blocks - used_blocks);

    print_string(&kernel_cursor_x, &kernel_cursor_y, "\r\n\r\n");
}