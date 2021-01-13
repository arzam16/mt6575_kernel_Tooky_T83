
#include <linux/module.h>

/*This can be used ONLY by the M4U driver!*/
void inner_dcache_flush_all(void)
{
    unsigned int way = 0x3; /* 4 ways */
    unsigned int set = 0xFF;    /* 32KB cache size */
    unsigned int Rt = 0;

    asm volatile(
        "1:\n"
        "mov %1, #3\n"
        "2:\n"
        "mov %0, #0\n"
        "orr %0, %0, %1, lsl #30\n"
        "orr %0, %0, %2, lsl #5\n"
        "mcr p15, 0, %0, c7, c14, #2\n"
        "subs %1, %1, #1\n"
        "bge 2b\n"
        "subs %2, %2, #1\n"
        "bge 1b\n"
        "dsb\n"
        "isb\n"
        : "+r" (Rt), "+r" (way), "+r" (set)
        : 
        : "cc"
    );
}

EXPORT_SYMBOL(inner_dcache_flush_all);
