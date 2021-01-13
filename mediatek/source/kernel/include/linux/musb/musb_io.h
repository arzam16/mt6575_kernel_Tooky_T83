

#ifndef __MUSB_LINUX_PLATFORM_ARCH_H__
#define __MUSB_LINUX_PLATFORM_ARCH_H__

#include <linux/io.h>
#include <linux/spinlock.h>
extern bool mtk_usb_power;
extern bool usb_enable_clock(bool enable);

static DEFINE_SPINLOCK(usb_io_lock);

static inline u16 musb_readw(const void __iomem *addr, unsigned offset)
{
	u16 rc = 0;
	if(mtk_usb_power)
		rc =  __raw_readw(addr + offset); 
	else
	{
    	unsigned long flags = 0;
    	spin_lock_irqsave(&usb_io_lock, flags);
		usb_enable_clock(true);
		printk("[MUSB]:access %s function when usb clock is off 0x%X\n",__func__, offset);
		rc =  __raw_readw(addr + offset); 
		usb_enable_clock(false);
    	spin_unlock_irqrestore(&usb_io_lock, flags);
	}
	return rc;
}
static inline u32 musb_readl(const void __iomem *addr, unsigned offset)
{
	u32 rc = 0;
	if(mtk_usb_power)
		rc =  __raw_readl(addr + offset); 
	else
	{
    	unsigned long flags = 0;
    	spin_lock_irqsave(&usb_io_lock, flags);
		usb_enable_clock(true);
		printk("[MUSB]:access %s function when usb clock is off 0x%X\n",__func__, offset);
		rc =  __raw_readl(addr + offset); 
		usb_enable_clock(false);
    	spin_unlock_irqrestore(&usb_io_lock, flags);
	}
	return rc;
}


static inline void musb_writew(void __iomem *addr, unsigned offset, u16 data)
{
	if(mtk_usb_power)
		__raw_writew(data, addr + offset); 
	else
	{
    	unsigned long flags = 0;
    	spin_lock_irqsave(&usb_io_lock, flags);
		usb_enable_clock(true);
		printk("[MUSB]:access %s function when usb clock is off 0x%X\n",__func__, offset);
		__raw_writew(data, addr + offset); 
		usb_enable_clock(false);
    	spin_unlock_irqrestore(&usb_io_lock, flags);
	}	
}

static inline void musb_writel(void __iomem *addr, unsigned offset, u32 data)
{ 
	if(mtk_usb_power)
		__raw_writel(data, addr + offset); 
	else
	{
    	unsigned long flags = 0;
    	spin_lock_irqsave(&usb_io_lock, flags);
		usb_enable_clock(true);
		printk("[MUSB]:access %s function when usb clock is off 0x%X\n",__func__, offset);
		__raw_writel(data, addr + offset); 
		usb_enable_clock(false);
    	spin_unlock_irqrestore(&usb_io_lock, flags);
	}	
}

static inline u8 musb_readb(const void __iomem *addr, unsigned offset)
{ 
	u8 rc = 0;
	if(mtk_usb_power)
		rc =  __raw_readb(addr + offset); 
	else
	{
    	unsigned long flags = 0;
    	spin_lock_irqsave(&usb_io_lock, flags);
		usb_enable_clock(true);
		printk("[MUSB]:access %s function when usb clock is off 0x%X\n",__func__, offset);
		rc =  __raw_readb(addr + offset); 
		usb_enable_clock(false);
    	spin_unlock_irqrestore(&usb_io_lock, flags);
	}	
	return rc;
}

static inline void musb_writeb(void __iomem *addr, unsigned offset, u8 data)
{
	if(mtk_usb_power)
		__raw_writeb(data, addr + offset); 
	else
	{
    	unsigned long flags = 0;
    	spin_lock_irqsave(&usb_io_lock, flags);
		usb_enable_clock(true);
		printk("[MUSB]:access %s function when usb clock is off 0x%X\n",__func__, offset);
		__raw_writeb(data, addr + offset); 
		usb_enable_clock(false);
    	spin_unlock_irqrestore(&usb_io_lock, flags);	
	}	
}

#if 0
struct musb_reg {
    u8      (*_readb)(u32 addr, u32 offset);
    u16     (*_readw)(u32 addr, u32 offset);
    u32     (*_readl)(u32 addr, u32 offset);
    void    (*_writeb)(u32 addr, u32 offset,u8 data);
    void    (*_writew)(u32 addr, u32 offset,u16 data);
    void    (*_writel)(u32 addr, u32 offset,u32 data); 
};
/* NOTE:  these offsets are all in bytes */

static inline u16 power_on_readw(const void __iomem *addr, unsigned offset)
	{ return __raw_readw(addr + offset); }

static inline u32 power_on_readl(const void __iomem *addr, unsigned offset)
	{ return __raw_readl(addr + offset); }


static inline void power_on_writew(void __iomem *addr, unsigned offset, u16 data)
	{ __raw_writew(data, addr + offset); }

static inline void power_on_writel(void __iomem *addr, unsigned offset, u32 data)
	{ __raw_writel(data, addr + offset); }

static inline u8 power_on_readb(const void __iomem *addr, unsigned offset)
	{ return __raw_readb(addr + offset); }

static inline void power_on_writeb(void __iomem *addr, unsigned offset, u8 data)
	{ __raw_writeb(data, addr + offset); }


static inline u16 power_off_readw(const void __iomem *addr, unsigned offset)
	{
		printk("MUSB:access %s function when usb clock is off\n",__func__);
		return 0; }

static inline u32 power_off_readl(const void __iomem *addr, unsigned offset)
	{ 
		printk("MUSB:access %s function when usb clock is off\n",__func__);
		return 0; }


static inline void power_off_writew(void __iomem *addr, unsigned offset, u16 data)
	{ printk("MUSB:access %s function when usb clock is off\n",__func__); }

static inline void power_off_writel(void __iomem *addr, unsigned offset, u32 data)
	{ printk("MUSB:access %s function when usb clock is off\n",__func__); }

static inline u8 power_off_readb(const void __iomem *addr, unsigned offset)
	{		
		printk("MUSB:access %s function when usb clock is off\n",__func__);
		return 0; }

static inline void power_off_writeb(void __iomem *addr, unsigned offset, u8 data)
	{ printk("MUSB:access %s function when usb clock is off\n",__func__);}

extern struct musb_reg musb_reg;
#define musb_readb  musb_reg._readb
#define musb_readw  musb_reg._readl
#define musb_readl  musb_reg._readw
#define musb_writeb musb_reg._writeb
#define musb_writew musb_reg._writew
#define musb_writel musb_reg._writel
#endif

#endif

