

#include <common.h>
#include <asm/arch/mt65xx.h>
#include <asm/arch/mt65xx_typedefs.h>
#include <cust_key.h>
#include <asm/arch/mt6575_key.h>
#include <asm/arch/boot_mode.h>
#include <asm/arch/mt6575_pmic6329.h>

extern mt6575_detect_powerkey(void);
extern mt6575_detect_homekey(void);

bool mt6575_detect_key(unsigned short key)	/* key: HW keycode */
{
	unsigned short idx, bit, din;

	if (key >= KPD_NUM_KEYS)
		return false;

	if (key % 9 == 8)
		key = 8;

#if 0 //KPD_PWRKEY_USE_EINT 
    if (key == 8)
    {                         /* Power key */
        idx = KPD_PWRKEY_EINT_GPIO / 16;
        bit = KPD_PWRKEY_EINT_GPIO % 16;

        din = DRV_Reg16 (GPIO_DIN_BASE + (idx << 4)) & (1U << bit);
        din >>= bit;
        if (din == KPD_PWRKEY_GPIO_DIN)
        {
            dbg_print ("power key is pressed\n");
            return true;
        }
        return false;
    }
#else // check by PMIC
    if (key == 8)
    {                         /* Power key */
        if (1 == mt6575_detect_powerkey())
        {
            //dbg_print ("power key is pressed\n");
            return true;
        }
        return false;
    }    
#endif

	if(get_chip_eco_ver() == CHIP_E2) /* for E1 board's VR301 issue */
	{
		if (key == MT65XX_PMIC_RST_KEY) 
		{
			if (1 == mt6575_detect_homekey())
			{
				return true;
			}
			// return false; /* to support common EVB */
		}
	}

	idx = key / 16;
	bit = key % 16;

	din = DRV_Reg16(KP_MEM1 + (idx << 2)) & (1U << bit);
	if (!din) {
		printf("key %d is pressed\n", key);
		return true;
	}
	return false;
}

bool mt6575_detect_pmic_just_rst()
{
	kal_uint8 just_rst;
	
	printf("detecting pmic just reset\n");
	if(get_chip_eco_ver() == CHIP_E2)
	{
		pmic_read_interface(0x15, &just_rst, 0x01, 0x07);
		if(just_rst)
		{
			printf("Just recover form a reset\n"); 
			pmic_bank1_config_interface(0x22, 0x01, 0x01, 0x07);
			return TRUE;
		}
	}
	return FALSE;
}
