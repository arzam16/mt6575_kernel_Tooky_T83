

#ifndef __MUSB_LINUX_DEBUG_H__
#define __MUSB_LINUX_DEBUG_H__

#define yprintk(facility, format, args...) \
	do { printk(facility "%s %d: " format , \
	__func__, __LINE__ , ## args); } while (0)
	
//workaroud for redefine warning in usb_dump.c
#ifdef WARNING
#undef WARNING
#endif

#ifdef INFO
#undef INFO
#endif

#ifdef ERR
#undef ERR
#endif

#define WARNING(fmt, args...) yprintk(KERN_WARNING, fmt, ## args)
#define INFO(fmt, args...) yprintk(KERN_INFO, fmt, ## args)
#define ERR(fmt, args...) yprintk(KERN_ERR, fmt, ## args)

#define xprintk(level, facility, format, args...) do { \
	if (_dbg_level(level)) { \
		printk(facility "[MUSB]%s %d: " format , \
				__func__, __LINE__ , ## args); \
	} } while (0)

extern unsigned musb_debug;

static inline int _dbg_level(unsigned level)
{
	return level <= musb_debug;
}

#ifdef DBG
#undef DBG
#endif
#define DBG(level, fmt, args...) xprintk(level, KERN_EMERG, fmt, ## args)

//extern const char *otg_state_string(struct musb *);

#ifdef CONFIG_USB_MTK_DEBUG_FS
extern int musb_init_debugfs(struct musb *musb);
extern void musb_exit_debugfs(struct musb *musb);
#else
static inline int musb_init_debugfs(struct musb *musb)
{
	return 0;
}
static inline void musb_exit_debugfs(struct musb *musb)
{
}
#endif

#endif				/*  __MUSB_LINUX_DEBUG_H__ */
