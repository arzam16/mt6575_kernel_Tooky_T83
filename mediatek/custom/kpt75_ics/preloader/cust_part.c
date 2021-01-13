
#include "typedefs.h"
#include "platform.h"
#include "blkdev.h"
#include "cust_part.h"

static part_t platform_parts[PART_MAX_COUNT];

void cust_part_init(void)
{
	u32 index = 0;
    memset(platform_parts, 0, sizeof(part_t) * PART_MAX_COUNT);
	{
		platform_parts[index].name = PART_PRELOADER;
		platform_parts[index].size = PART_SIZE_PRELOADER;
		platform_parts[index].flags = PART_FLAG_NONE;
		platform_parts[index].startblk = 0;
		index++;
	}
	{
		platform_parts[index].name = PART_DSP_BL;
		platform_parts[index].size = PART_SIZE_DSP_BL;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_MBR;
		platform_parts[index].size = PART_SIZE_MBR;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_EBR1;
		platform_parts[index].size = PART_SIZE_EBR1;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_PMT;
		platform_parts[index].size = PART_SIZE_PMT;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_NVRAM;
		platform_parts[index].size = PART_SIZE_NVRAM;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_SECURE;
		platform_parts[index].size = PART_SIZE_SECCFG;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_UBOOT;
		platform_parts[index].size = PART_SIZE_UBOOT;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_BOOTIMG;
		platform_parts[index].size = PART_SIZE_BOOTIMG;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_RECOVERY;
		platform_parts[index].size = PART_SIZE_RECOVERY;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_SECSTATIC;
		platform_parts[index].size = PART_SIZE_SEC_RO;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_MISC;
		platform_parts[index].size = PART_SIZE_MISC;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_LOGO;
		platform_parts[index].size = PART_SIZE_LOGO;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_EXPDB;
		platform_parts[index].size = PART_SIZE_EXPDB;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_PRO_INFO;
		platform_parts[index].size = PART_SIZE_PRO_INFO;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_EBR2;
		platform_parts[index].size = PART_SIZE_EBR2;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_MOBILE_INFO;
		platform_parts[index].size = PART_SIZE_MOBILE_INFO;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_EBR3;
		platform_parts[index].size = PART_SIZE_EBR3;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_ANDSYSIMG;
		platform_parts[index].size = PART_SIZE_ANDROID;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_CACHE;
		platform_parts[index].size = PART_SIZE_CACHE;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_USER;
		platform_parts[index].size = PART_SIZE_USRDATA;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}
	{
		platform_parts[index].name = PART_FAT;
		platform_parts[index].size = PART_SIZE_FAT;
		platform_parts[index].flags = PART_FLAG_NONE;
		index++;
	}

#ifdef CFG_USE_DSP_ROM_PARTITION    
    {
        platform_parts[index].name = PART_DSP_ROM;
        platform_parts[index].size = 1 * MB;
        platform_parts[index].flags = PART_FLAG_NONE;
        index++;
    }
#endif

#ifdef CFG_USE_AP_ROM_PARTITION
    {
        platform_parts[index].name = PART_AP_ROM;
        platform_parts[index].size = 50 * MB;
        platform_parts[index].flags = PART_FLAG_NONE;
        index++;
    }
#endif

#ifdef CFG_USE_MD_ROM_PARTITION
    {
        platform_parts[index].name = PART_MD_ROM;
        platform_parts[index].size = 8 * MB;
        platform_parts[index].flags = PART_FLAG_NONE;
        index++;
    }
#endif

    {
        platform_parts[index].name = NULL;
        platform_parts[index].size = 0;
        platform_parts[index].flags = PART_FLAG_END;
        index++;
    }
}

part_t *cust_part_tbl(void)
{
    return &platform_parts[0];
}

