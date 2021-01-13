
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <asm/mach/irq.h>
#include <asm/hardware/gic.h>
#include <linux/smp.h>
#include <asm/hardware/cache-l2x0.h>
#include "mach/mt6575_reg_base.h"
#include "mach/mt6575_irq.h"
#include "mach/mt6575_fiq.h"

#define GIC_ICDISR      (GIC_DIST_BASE + 0x80)
#define GIC_ICDSGIR     (GIC_DIST_BASE + 0xF00)

struct fiq_glue_handler **list_fiq_handler;

static int allowed_fiqs[] = 
{
    MT6575_UART4_IRQ_ID,
    MT6575_WDT_IRQ_ID,
};

static void cache_sync (void)
{
    *((volatile unsigned int*)(PL310_BASE + L2X0_CACHE_SYNC)) = 0;
}

static void mt6575_dist_set_fiq(int irq)
{
    int val;

    val = readl(GIC_ICDISR + 4 * (irq / 32));
    val &= ~(1 << (irq % 32));
    writel(val, GIC_ICDISR + 4 * (irq / 32));
}

int request_fiq(int fiq, struct fiq_glue_handler *h)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(allowed_fiqs); i++)
    {
        if (allowed_fiqs[i] == fiq)
        {
            list_fiq_handler[i] = h;
            mt6575_dist_set_fiq(fiq);
            enable_fiq(fiq);
            return 0;
        }
    }
    return -1;
}

int free_fiq(int fiq)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(allowed_fiqs); i++)
    {
        if (allowed_fiqs[i] == fiq)
        {
            list_fiq_handler[i] = 0;
            return 0;
        }
    }
    return -1;
}

static void fiq_isr(struct fiq_glue_handler *h, void *regs, void *svc_sp)
{
//    unsigned int this_cpu = THREAD_INFO(svc_sp)->cpu;
    struct fiq_glue_handler *handler;
    unsigned int id;
    int i;

    id = *((volatile unsigned int*)(GIC_CPU_BASE + GIC_CPU_INTACK));
    
    for (i = 0; i < ARRAY_SIZE(allowed_fiqs); i++)
    {
        if (allowed_fiqs[i] == id && (handler = list_fiq_handler[i]) != 0)
        {
            (handler->fiq)(handler, regs, svc_sp);
            break;
        }
    }

    *((volatile unsigned int*)(GIC_CPU_BASE + GIC_CPU_EOI)) = id;
    dsb ();
    cache_sync ();
}

struct fiq_glue_handler fiq_handler =
{
    .fiq = fiq_isr,
    .resume = 0,
};

int __init fiq_init(void)
{
    int ret;

    ret = fiq_glue_register_handler(&fiq_handler);

    if (ret)
    {
        pr_err("%s: could not install fiq handler\n", __func__);
    }
    else
    {
        list_fiq_handler = kmalloc(sizeof (void*) * ARRAY_SIZE(allowed_fiqs), GFP_KERNEL);

        if (!list_fiq_handler)
        {
            pr_err("%s: could not allocated list of fiq isr\n", __func__);
            ret = -ENOMEM;
        }
    }

    return ret;
}

arch_initcall(fiq_init);

EXPORT_SYMBOL(request_fiq);
EXPORT_SYMBOL(free_fiq);
