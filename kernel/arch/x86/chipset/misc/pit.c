#include <core/system.h>
#include <cpu/io.h>
#include <chipset/misc.h>

static struct ioaddr __pit_ioaddr;

#define PIT_CHANNEL0    0x00
#define PIT_CMD         0x03

struct pit_cmd_register {
    union {
        struct {
            uint32_t bcd    : 1;
            uint32_t mode   : 3;
            uint32_t access : 2;
            uint32_t channel: 2;
        } __packed;
        uint8_t raw;
    } __packed;
} __packed;

#define PIT_MODE_SQUARE_WAVE    0x3
#define PIT_ACCESS_LOHIBYTE     0x3

void x86_pit_setup(struct ioaddr *io)
{
    printk("8254 PIT: Initializing [%p (%s)]\n", io->addr, ioaddr_type_str(io));
    __pit_ioaddr = *io;
}

#define FBASE   1193182ULL  /* PIT Oscillator operates at 1.193182 MHz */
uint32_t x86_pit_period_set(uint32_t period_ns)
{
    uint32_t freq = 1000000000UL/period_ns;
    uint32_t div = FBASE/freq;  

    if (div == 0) div = 1;

    div = 1000; /* XXX */
    period_ns = 1000000000UL/FBASE/div;

    printk("8254 PIT: Setting period to %d ns\n", period_ns);

    struct pit_cmd_register cmd = {
        .bcd = 0,
        .mode = PIT_MODE_SQUARE_WAVE,
        .access = PIT_ACCESS_LOHIBYTE,
        .channel = 0,
    };

    io_out8(&__pit_ioaddr, PIT_CMD, cmd.raw);
    io_out8(&__pit_ioaddr, PIT_CHANNEL0, (div >> 0) & 0xFF);
    io_out8(&__pit_ioaddr, PIT_CHANNEL0, (div >> 8) & 0xFF);

    return period_ns;
}
