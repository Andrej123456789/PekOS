
#include "timer.h"
#define PRINT_TICK 0

uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
    tick++;

    if (PRINT_TICK == 1)
        printf(".");

	if (PRINT_TICK == 2)
    	printf("[~] Tick: %d \n", tick);

}

void timer_init(uint32_t frequency)
{
    register_interrupt_handler(IRQ0, &timer_callback);

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    outb(0x40, l);
    outb(0x40, h);

}