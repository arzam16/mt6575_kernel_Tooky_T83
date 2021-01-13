
#ifndef __MT65XX_PARTITION_H__
#define __MT65XX_PARTITION_H__
/*autogen by Ptgen(Kai.Zhu mtk81086)*/
#define PART_PRO_INFO			"PRO_INFO"
#define PART_BLKS_PRO_INFO		"BLK_NUM(PART_SIZE_PRO_INFO)"
#define PART_MOBILE_INFO			"MOBILE_INFO"
#define PART_BLKS_MOBILE_INFO		"BLK_NUM(PART_SIZE_MOBILE_INFO)"
#define PART_EBR3			"EBR3"
#define PART_BLKS_EBR3		"BLK_NUM(PART_SIZE_EBR3)"
#define PART_FAT			"FAT"
#define PART_BLKS_FAT		"BLK_NUM(PART_SIZE_FAT)"
/*autogen by Ptgen(Kai.Zhu mtk81086)*/

#include <part.h>
#include "partition_define.h"
//-------------------------------------------
// central control in mt6575_partition.h 
#define CFG_PRELOADER
#define CFG_USE_MBL_PARTITION
//-------------------------------------------

#define PART_MBOOT      "MBOOT"
#define PART_MBL        "MBL"
#define PART_PRELOADER           "PRELOADER"
#define PART_DSP_DL                  "DSP_DL"
#define PART_UBOOT      "UBOOT"
#define PART_SECURE     "SECCNFG"
#define PART_LOGO       "LOGO"
#define PART_IMAGE      "IMAGE"

#ifdef MTK_EMMC_SUPPORT
#define PART_MBR		"MBR"
#define PART_EBR1		"EBR1"
#define PART_EBR2		"EBR2"
#define PART_PMT		"PMT"
#endif

//-------------------------------------------
// Android Boot Image
#define PART_BOOTIMG    "BOOTIMG"

#define PART_KERNEL     "KERNEL"
#define PART_ROOTFS     "ROOTFS"
//-------------------------------------------

#define PART_USER       "USER"
#define PART_ANDSYSIMG  "ANDSYSIMG"
#define PART_CACHE	    "CACHE"
#define PART_MISC	    "MISC"
#define PART_NVRAM	    "NVRAM"
#define PART_RECOVERY	"RECOVERY"
#define PART_APANIC 	"APANIC"
#define PART_SECSTATIC  "SECSTATIC"

#define KB               (1024)
#define MB               (1024 * KB)
#define GB               (1024 * MB)

#define NAND_WRITE_SIZE	 2048

#define BLK_BITS         (9)
#define BLK_SIZE         (1 << BLK_BITS)
#define BLK_NUM(size)    ((unsigned long long)(size) / BLK_SIZE)

#define PART_BLKS_PRELOADER   BLK_NUM(PART_SIZE_PRELOADER)
#define PART_BLKS_DSP_BL      BLK_NUM(PART_SIZE_DSP_BL)
#define PART_BLKS_MBL           BLK_NUM(PART_SIZE_PRELOADER+PART_SIZE_DSP_BL)
#define PART_BLKS_UBOOT  	    BLK_NUM(PART_SIZE_UBOOT)
#define PART_BLKS_SECURE  	    BLK_NUM(PART_SIZE_SECCFG)
#define PART_BLKS_LOGO   	BLK_NUM(PART_SIZE_LOGO)

#ifdef MTK_EMMC_SUPPORT
#define PART_BLKS_MBR		BLK_NUM(PART_SIZE_MBR)
#define PART_BLKS_EBR1		BLK_NUM(PART_SIZE_EBR1)
#define PART_BLKS_EBR2		BLK_NUM(PART_SIZE_EBR2)
#define PART_BLKS_PMT		BLK_NUM(PART_SIZE_PMT)
#endif
//--------------------------------------------
// Android Boot Image
#define PART_BLKS_BOOTIMIG 	    BLK_NUM(PART_SIZE_BOOTIMG)
//--------------------------------------------

#define PART_BLKS_RECOVERY      BLK_NUM(PART_SIZE_RECOVERY)
#define PART_BLKS_SYSIMG 	    BLK_NUM(PART_SIZE_ANDROID)
#define PART_BLKS_CACHE  	    BLK_NUM(PART_SIZE_CACHE)
#define PART_BLKS_NVRAM  	    BLK_NUM(PART_SIZE_NVRAM)
#define PART_BLKS_SECSTATIC	    BLK_NUM(PART_SIZE_SEC_RO)
#define PART_BLKS_MISC		    BLK_NUM(PART_SIZE_MISC)
#define PART_BLKS_USER   	    BLK_NUM(PART_SIZE_USRDATA)
#define PART_BLKS_APANIC   	    BLK_NUM(PART_SIZE_EXPDB)

#define PART_FLAG_NONE    0
#define PART_FLAG_LEFT    0x1
#define PART_FLAG_END     0x2

#define PART_MAGIC        0x58881688

typedef union {
    struct {    
        unsigned int magic;        /* partition magic */
        unsigned int dsize;        /* partition data size */
        char         name[32];     /* partition name */
    } info;
    unsigned char data[BLK_SIZE];
} part_hdr_t;

typedef struct {
    unsigned char *name;        /* partition name */
    unsigned long  blknum;      /* partition blks */
    unsigned long  flags;       /* partition flags */
    unsigned long  startblk;    /* partition start blk */
} part_t;

struct mt6575_CMD{
	u32	u4ColAddr;
	u32 u4RowAddr;
	u32 u4OOBRowAddr;
	u8	au1OOB[64];
	u8*	pDataBuf;
};
typedef struct part_dev part_dev_t;

struct part_dev {
    int init;
    int id;
    block_dev_desc_t *blkdev;
    int (*init_dev) (int id);
#ifdef MTK_EMMC_SUPPORT
	int (*read)  (part_dev_t *dev, u64 src, uchar *dst, int size);
    int (*write) (part_dev_t *dev, uchar *src, u64 dst, int size);
#else
    int (*read)  (part_dev_t *dev, ulong src, uchar *dst, int size);
    int (*write) (part_dev_t *dev, uchar *src, ulong dst, int size);
#endif
};

extern int mt65xx_part_register_device(part_dev_t *dev);
extern part_t* mt65xx_part_get_partition(char *name);
extern part_dev_t* mt65xx_part_get_device(void);
extern void mt65xx_part_init(unsigned long totalblks);
extern void mt65xx_part_dump(void);

#endif /* __MT65XX_PARTITION_H__ */


