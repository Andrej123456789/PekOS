#include "keyb.h"

static char command[5];
static int i = 0;

char    os[10]     = "PekOS";
char    kernel[20] = "Bt-Sys-II";
char    sh[10]     = "pTerm";

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);

    if (scancode & 0x80) { }

    else if (scancode == 0x0E || scancode == 0x8E) { printf("\b \b"); }

    else if (scancode == 0x1C || scancode == 0x9C)
    {
        printf("\n");
        handle_command();
    }

    else
    {
        command[i] = BtSCI[scancode];
        i++;
        putc(BtSCI[scancode]);
    }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
    register_interrupt_handler(IRQ1, keyboard_handler);
}

void handle_command()
{
    /*
    * How this shell works
    * Checks command array.
    * Why we have temp4?
    * Well, if user enters `jabuka` which is not command but it is larger than current command (`test`) command
    * array is `testka` and program cannot recognize but using for loop at start of function we are making new char array
    * which is 4 chars long. If you want do similar for commands which have 2 or 3 letters, create new array temp + number and change
    * if statment condition, be careful C arrays starts from 0! 
    */

    char temp4[5];

    for (int i = 0; i < strlen(command); i++)
    {
        if (i != 4)
        {
            temp4[i] = command[i];
        }
    }

    if (strcmp(command, "test") == 0 || strcmp(temp4, "test") == 0) { printf("`1234567890'+qwertzuiop[]\n"); }
    /* Sometimes it is `cpu`, sometimes it is `cpui` */
    else if (strcmp(command, "cpui") == 0 || strcmp(temp4, "cpui") == 0) { detect_cpu(); }
    
    else if (strcmp(command, "ver") == 0 || strcmp(temp4, "ver") == 0) { 
        printf("OS      :   %s\n", os);
        printf("OS Ver  :   %d\n", 8);
        printf("Kernel  :   %s\n", kernel);
        printf("Shell   :   %s\n", sh);
        printf("Sh. v   :   2.5.0\n");
    }

    else if (strcmp(command, "clear") == 0 || strcmp(temp4, "clear") == 0) { clear(BLACK, WHITE); }
    
    else if (strcmp(command, "\0") == 0 || strcmp(temp4, "\0") == 0) { }

    else if (strcmp(command, "reboot") == 0 || strcmp(temp4, "reboot") == 0) { outb(0x64, 0xFE); }

    else if (strcmp(command, "shutdown") == 0 || strcmp(temp4, "shutdown") == 0) {
        __asm ("outw %%ax, %%dx" : : "a"(0x2000), "d"(0x604) );
    }

    else
    {
        printf("? Command '%s' not found ?\n", command);
    }

    printf("] ");

    command[0] = '\0';
    i = 0;
}