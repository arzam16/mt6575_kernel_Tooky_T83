
#include <linux/pm.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/smp_scu.h>
#include <mach/mt6575_reg_base.h>
#include <mach/irqs.h>

extern struct sys_timer mt6575_timer;
extern void mt6575_fixup(struct machine_desc *desc, struct tag *tags, char **cmdline, struct meminfo *mi);
extern void mt6575_power_off(void);


void __init mt6575_init(void)
{
    pm_power_off = mt6575_power_off;

#if defined(CONFIG_CACHE_L2X0)
    writel(L2X0_DYNAMIC_CLK_GATING_EN, PL310_BASE + L2X0_POWER_CTRL);
    writel(readl(PL310_BASE + L2X0_PREFETCH_CTRL) | 0x40000000, PL310_BASE + L2X0_PREFETCH_CTRL); 
    l2x0_init((void __iomem *)PL310_BASE, 0x70000000, 0x8FFFFFFF);
#endif  /* CONFIG_CACHE_L2X0 */

#if defined(CONFIG_HAVE_ARM_SCU)
    scu_enable((void *)SCU_BASE);

    /* set INFRA_ACP to 0x00003333 for receiving transactions to ACP */
    writel(0x00003333, INFRA_SYS_CFG_BASE + 0x0F04);
#endif  /* CONFIG_HAVE_ARM_SCU */
}

static struct map_desc mt6575_io_desc[] __initdata = 
{
    {
        .virtual = AP_RGU_BASE,
        .pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(AP_RGU_BASE)),
        .length = SZ_16M,
        .type = MT_DEVICE
    },
    {
        .virtual = PERICFG_BASE,
        .pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(PERICFG_BASE)),
        .length = SZ_16M,
        .type = MT_DEVICE
    },
    {
        .virtual = MMSYS1_CONFIG_BASE,
        .pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(MMSYS1_CONFIG_BASE)),
        .length = SZ_16M,
        .type = MT_DEVICE
    },
    {
        .virtual = SYSRAM_BASE,
        .pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(SYSRAM_BASE)),
        .length = SZ_256K,
        .type = MT_DEVICE_WC
    },
    {
        .virtual = ABB_MDSYS_BASE,
        .pfn = __phys_to_pfn(IO_ABB_MDSYS_VIRT_TO_PHYS(ABB_MDSYS_BASE)),
        .length = SZ_4K,
        .type = MT_DEVICE
    },
    {
        .virtual = AUDIO_BASE,
        .pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(AUDIO_BASE)),
        .length = SZ_4K,
        .type = MT_DEVICE
    },
    {
        .virtual = MFG_AXI_BASE,
        .pfn = __phys_to_pfn(IO_VIRT_TO_PHYS(MFG_AXI_BASE)),
        .length = SZ_64K,
        .type = MT_DEVICE
    },
    {
        .virtual = VER_BASE,
        .pfn = __phys_to_pfn(IO_VER_VIRT_TO_PHYS(VER_BASE)),
        .length = SZ_4K,
        .type = MT_DEVICE
    },
    {
        .virtual = INTERNAL_SRAM_BASE,
        .pfn = __phys_to_pfn(0xF0000000),
        .length = SZ_64K,
        .type = MT_DEVICE_WC
    },
};

void __init mt6575_map_io(void)
{
    iotable_init(mt6575_io_desc, ARRAY_SIZE(mt6575_io_desc));
}

MACHINE_START(MT6575, "MT6575")
    .boot_params    = PHYS_OFFSET + 0x00000100,
    .map_io         = mt6575_map_io,
    .init_irq       = mt6575_init_irq,
    .timer          = &mt6575_timer,
    .init_machine   = mt6575_init,
    .fixup          = mt6575_fixup
MACHINE_END
