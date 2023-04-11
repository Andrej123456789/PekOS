
#include <stddef.h>
#include <stdint.h>
#include "txt/puts.cc"
#include "scr/sets.cc"
#include "idt/idt.cc"
#include "io/keyb.cc"
#include "scr/inf.cc"
#include "scr/menu.cc"

extern const char Test[];

unsigned long long rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

void init_pit(uint32_t frequency)
{
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);

    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void wait_pit(uint32_t ticks)
{
    uint64_t start_time = rdtsc();
    uint64_t end_time = start_time + ticks;

    while (rdtsc() < end_time);
}

void timer_test()
{
    init_pit(100);
    wait_pit(1000000000);

    puts("Timer done!\n");
}

extern "C" void _start()
{
    clear();
    screen(BG_BLUE | FG_WHITE);

    init_idt();
    draw_inf();

    // int c = 4 / 0;
    // asm("int $0x0e");

    return;
}
