#include <mach/mt_clock_manager.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/spinlock.h>
#include <linux/musb/mtk_musb.h>
#include "usb20.h"

#define FRA (48)
#define PARA (22)

#ifdef FPGA_PLATFORM
bool usb_enable_clock(bool enable)
{
	return true;
}

void usb_phy_poweron(void)
{
}

void usb_phy_savecurrent(void)
{
}

void usb_phy_recover(void)
{
}

// BC1.2
void Charger_Detect_Init(void)
{
}

void Charger_Detect_Release(void)
{
}

void usb_phy_context_save(void)
{
}

void usb_phy_context_restore(void)
{
}


#else

static DEFINE_SPINLOCK(musb_reg_clock_lock);


bool usb_enable_clock(bool enable)
{
    #ifndef CONFIG_MT6575T_FPGA
	static int count = 0;
	bool res = TRUE;
	unsigned long flags;

	spin_lock_irqsave(&musb_reg_clock_lock, flags);

	if (enable && count == 0) {
		res = enable_clock(MT65XX_PDN_PERI_USB1, "PERI_USB");
	} else if (!enable && count == 1) {
		res = disable_clock(MT65XX_PDN_PERI_USB1, "PERI_USB");
	}

	if (enable)
		count++;
	else
		count = (count==0) ? 0 : (count-1);

	spin_unlock_irqrestore(&musb_reg_clock_lock, flags);

	printk(KERN_DEBUG "enable(%d), count(%d) res=%d\n", enable, count, res);
#endif //End of CONFIG_MT6589_FPGA
	return 1;
}

static void hs_slew_rate_cal(void){
  unsigned long data;
  unsigned long x;
  unsigned char value;
  unsigned long start_time, timeout;
  unsigned int timeout_flag = 0;
  //4 s1:enable usb ring oscillator.
  USBPHY_WRITE8(0x15,0x80);

  //4 s2:wait 1us.
  udelay(1);

  //4 s3:enable free run clock
  USBPHY_WRITE8 (0xf00-0x800+0x11,0x01);
  //4 s4:setting cyclecnt.
  USBPHY_WRITE8 (0xf00-0x800+0x01,0x04);
  //make sure USB PORT1
   USBPHY_CLR8 (0xf00-0x800+0x03,0x0C);
  //4 s5:enable frequency meter
  USBPHY_SET8 (0xf00-0x800+0x03,0x01);
  //4 s6:wait for frequency valid.
  start_time = jiffies;
  timeout = jiffies + 3 * HZ;

  while(!(USBPHY_READ8(0xf00-0x800+0x10)&0x1)){
    if(time_after(jiffies, timeout)){
        timeout_flag = 1;
        break;
        }
    }
  //4 s7: read result.
  if(timeout_flag){
    printk("[USBPHY] Slew Rate Calibration: Timeout\n");
    value = 0x4;
    }
  else{
      data = USBPHY_READ32 (0xf00-0x800+0x0c);
      x = ((1024*FRA*PARA)/data);
      value = (unsigned char)(x/1000);
      if((x-value*1000)/100>=5)
        value += 1;
      //printk("[USBPHY]slew calibration:x=%d,value=%d\n",x,value);
    }
  //4 s8: disable Frequency and run clock.
  USBPHY_CLR8 (0xf00-0x800+0x03,0x01);//disable frequency meter
  USBPHY_CLR8 (0xf00-0x800+0x11,0x01);//disable free run clock
  //4 s9:
  USBPHY_WRITE8(0x15,value<<4);

  //4 s10:disable usb ring oscillator.
  USBPHY_CLR8(0x15,0x80);
}

void usb_phy_poweron(void){

    #ifndef CONFIG_MT6575T_FPGA
    usb_enable_clock(true);

    //4 s2: wait 50 usec for PHY3.3v/1.8v stable.
    udelay(50);

    //4 s3: swtich to USB function. (system register, force ip into usb mode.
    USBPHY_CLR8(0x6b, 0x04);
    USBPHY_CLR8(0x6e, 0x01);
    //4 s4: RG_USB20_BC11_SW_EN 1'b0
    USBPHY_CLR8(0x1a, 0x80);

    /*
    USBPHY_CLR8(0x02, 0x7f);
    USBPHY_SET8(0x02, 0x09);
    USBPHY_CLR8(0x22, 0x03);
    */

    //4 s5: release force suspendm.
    USBPHY_CLR8(0x6a, 0x04);
    //USBPHY_SET8(0x1b, 0x08);

    //4 s6: wait for 800 usec.
    udelay(800);
    #endif

    printk("usb power on success\n");
}

static void usb_phy_savecurrent_internal(void){

    //4 1. swtich to USB function. (system register, force ip into usb mode.
    USBPHY_CLR8(0x6b, 0x04);
    USBPHY_CLR8(0x6e, 0x01);

    //4 2. release force suspendm.
    USBPHY_CLR8(0x6a, 0x04);

    //4 3. RG_DPPULLDOWN./RG_DMPULLDOWN.
    USBPHY_SET8(0x68, 0xc0);
    //4 4. RG_XCVRSEL[1:0] =2'b01.
    USBPHY_CLR8(0x68, 0x30);

    USBPHY_SET8(0x68, 0x10);

    //4 5. RG_TERMSEL = 1'b1
	USBPHY_SET8(0x68, 0x04);
    //4 6. RG_DATAIN[3:0]=4'b0000
	USBPHY_CLR8(0x69, 0x3c);
    //4 7.force_dp_pulldown, force_dm_pulldown, force_xcversel,force_termsel.
    USBPHY_SET8(0x6a, 0xba);

    //4 8.RG_USB20_BC11_SW_EN 1'b0
    USBPHY_CLR8(0x1a, 0x80);
    //4 9.RG_USB20_OTG_VBUSSCMP_EN 1'b0
    USBPHY_CLR8(0x1a, 0x10);
    //4 10. delay 800us.

    udelay(800);

    //4 11. rg_usb20_pll_stable = 1
    USBPHY_SET8(0x63, 0x02);

    udelay(1);

    USBPHY_SET8(0x6a, 0x04);

    //4 13.  wait 1us
    udelay(1);
}

void usb_phy_savecurrent(void){

#ifndef CONFIG_MT6589_FPGA
    usb_phy_savecurrent_internal();
    //4 14. turn off internal 48Mhz PLL.
    usb_enable_clock(false);
    #endif

    printk("usb save current success\n");
}

void usb_phy_recover(void){
    #ifndef CONFIG_MT6575T_FPGA
    usb_enable_clock(true);

    udelay(50);

    // clean PUPD_BIST_EN
    // PUPD_BIST_EN = 1'b0
    // PMIC will use it to detect charger type
    USBPHY_CLR8(0x1d, 0x10);

    //4 3. force_uart_en = 1'b0
    USBPHY_CLR8(0x6b, 0x04);
    USBPHY_CLR8(0x6e, 0x1);

    USBPHY_CLR8(0x6a, 0x04);

    USBPHY_CLR8(0x68, 0x40);
    USBPHY_CLR8(0x68, 0x80);
    USBPHY_CLR8(0x68, 0x30);
    USBPHY_CLR8(0x68, 0x04);
    USBPHY_CLR8(0x69, 0x3c);

    USBPHY_CLR8(0x6a, 0x10);
    USBPHY_CLR8(0x6a, 0x20);
    USBPHY_CLR8(0x6a, 0x08);
    USBPHY_CLR8(0x6a, 0x02);
    USBPHY_CLR8(0x6a, 0x80);

    //4 16. RG_USB20_BC11_SW_EN 1'b0
    USBPHY_CLR8(0x1a, 0x80);
    //4 17. RG_USB20_OTG_VBUSSCMP_EN 1'b1
    USBPHY_SET8(0x1a, 0x10);

    //4 18. wait 800 usec.
    udelay(800);

    hs_slew_rate_cal();
    #endif

    printk("usb recovery success\n");
    return;
}
#ifndef CONFIG_MTK_USBFSH

#define USB11_PHY_ADDR USB_SIF_BASE + 0x900
#define USB11PHY_READ32(offset)         __raw_readl(USB11_PHY_ADDR+(offset))
#define USB11PHY_READ8(offset)          __raw_readb(USB11_PHY_ADDR+(offset))
#define USB11PHY_WRITE8(offset, value)  __raw_writeb(value, USB11_PHY_ADDR+(offset))
#define USB11PHY_SET8(offset, mask)     USB11PHY_WRITE8((offset), USB11PHY_READ8(offset) | (mask))
#define USB11PHY_CLR8(offset, mask)     USB11PHY_WRITE8((offset), USB11PHY_READ8(offset) & (~(mask)))

void mt65xx_host_phy_savecurrent(void)
{
    USB11PHY_CLR8(0x6b, 0x04);
    USB11PHY_CLR8(0x6e, 0x01);

    //4 2. release force suspendm.
    USB11PHY_CLR8(0x6a, 0x04);
    //4 3. RG_DPPULLDOWN./RG_DMPULLDOWN.
    USB11PHY_SET8(0x68, 0xc0);
    //4 4. RG_XCVRSEL[1:0] =2'b01.
    USB11PHY_CLR8(0x68, 0x30);
    USB11PHY_SET8(0x68, 0x10);
    //4 5. RG_TERMSEL = 1'b1
    USB11PHY_SET8(0x68, 0x04);
    //4 6. RG_DATAIN[3:0]=4'b0000
    USB11PHY_CLR8(0x69, 0x3c);
    //4 7.force_dp_pulldown, force_dm_pulldown, force_xcversel,force_termsel.
    USB11PHY_SET8(0x6a, 0xba);
    
    //4 8.RG_USB20_BC11_SW_EN 1'b0
    USB11PHY_CLR8(0x1a, 0x80);
    //4 9.RG_USB20_OTG_VBUSSCMP_EN 1'b0
    USB11PHY_CLR8(0x1a, 0x10);
    //4 10. delay 800us.
    udelay(800);
    //4 11. rg_usb20_pll_stable = 1
    USB11PHY_SET8(0x63, 0x02);

    udelay(1);
    //4 12.  force suspendm = 1.hiok
    USB11PHY_SET8(0x6a, 0x04);

    USB11PHY_CLR8(0x6C, 0x2C);
    USB11PHY_SET8(0x6C, 0x10);
    USB11PHY_CLR8(0x6D, 0x3C);	
}

#endif

void usb_phy_context_save(void)
{
}

void usb_phy_context_restore(void)
{
  enable_pll(MT65XX_UPLL, "USB_PLL");
  #ifndef CONFIG_MTK_USBFSH
  mt65xx_host_phy_savecurrent();
  #endif
  usb_phy_savecurrent_internal();
  disable_pll(MT65XX_UPLL,"USB_PLL");
}

// BC1.2
void Charger_Detect_Init(void)
{
}

void Charger_Detect_Release(void)
{
}
#endif
