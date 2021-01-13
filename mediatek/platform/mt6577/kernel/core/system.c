#include <linux/kernel.h>
#include <linux/string.h>
#include <mach/mt_clock_manager.h>
#include <mach/mtk_rtc.h>

extern void wdt_arch_reset(char);

#ifdef CONFIG_LOCAL_WDT
enum wk_wdt_type {
	WK_WDT_LOC_TYPE,
	WK_WDT_EXT_TYPE,
	WK_WDT_LOC_TYPE_NOLOCK,
	WK_WDT_EXT_TYPE_NOLOCK,
};
extern void mtk_wdt_restart(enum wk_wdt_type type);
#else
extern void mtk_wdt_restart(void);
#endif
void arch_reset(char mode, const char *cmd)
{
#ifdef MT6575
  if (cmd && !strcmp(cmd, "charger")) {
      	wdt_arch_reset(0);
    }
	else
	{
		wdt_arch_reset(1);
	}
#endif
#ifndef CONFIG_MT6577_FPGA
    char reboot = 0;
    printk("arch_reset: cmd = %s\n", cmd ? : "NULL");

    if (cmd && !strcmp(cmd, "charger")) {
        /* do nothing */
    } else if (cmd && !strcmp(cmd, "recovery")) {
        rtc_mark_recovery();
    } else {
    	reboot = 1;
    }
    wdt_arch_reset(reboot);

#endif
}

#ifdef CONFIG_MT6577_FPGA
void arch_idle(void)
{}
#endif
