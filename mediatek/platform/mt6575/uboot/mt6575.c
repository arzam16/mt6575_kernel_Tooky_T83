

#include <common.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/mt65xx.h>
#include <asm/mach-types.h>
#include <asm/arch/mt65xx_typedefs.h>
#include <asm/arch/boot_mode.h>
#include <config.h>

//#define UBOOT_DL_CHECK

#ifdef UBOOT_DL_CHECK
/*block if check dl fail*/
#undef UBOOT_DL_CHECK_BLOCK_LEVEL
#endif

DECLARE_GLOBAL_DATA_PTR;
extern int Uboot_power_saving(void);
extern void cpu_clean_env (void);

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0" (loops));
}

void system_init(void)
{
/* CC: remove temporarily for MT6573 porting */
#if 0
    mt6573_timer_init();
#endif

    /* enabling audio depop takes time,
     * so init it here rather than linux kernel
     */

    delay(2000);
/* CC: remove temporarily for MT6573 porting */
#if 0
    mt6573_aud_init();

 //   led_init();
#endif

    return;
}

#if 0
int cleanup_before_linux (void)
{
	printf("system cleanup before entering linux ...\n");

	// (1) make sure the cache is off (turn off I/D-cache)
	// (2) make sure the cache is clean (flush I/D-cache)
	printf(" > clean cpu env\n");
	cpu_clean_env();

/* CC: remove temporarily for MT6573 porting */
#if 1
	// (3) deinit leds
	printf(" > deinit leds..\n");	
	leds_deinit();
#endif
	
	// (4) power off some unused LDO
	printf(" > perform power saving\n");		
	Uboot_power_saving();

	return (0);
}
#endif

int mt65xx_sw_env (void)
{  
	int dl_status = 0;
#ifdef CFG_RECOVERY_MODE
  if(g_boot_mode != META_BOOT && g_boot_mode != FACTORY_BOOT && recovery_detection() == TRUE)
  {    
      
  }
#endif   

#ifdef UBOOT_DL_CHECK
#ifdef CONFIG_MMC	
		dl_status = mmc_get_dl_info();
		printf("mt65xx_sw_env--dl_status: %d\n",dl_status);
		if(dl_status!=0){
			video_printf("=> TOOL DL image Fail!\n");
			printf("TOOL DL image Fail\n");
#ifdef UBOOT_DL_CHECK_BLOCK_LEVEL
			printf("uboot is blocking by dl info\n");
			while(1);
#endif
		}
#endif
#endif
    //**************************************
    //* CHECK BOOT MODE
    //**************************************
    #ifndef USER_BUILD
    switch(g_boot_mode)
    {        
         case META_BOOT :
	         video_printf(" => META MODE\n");
	         break;
	     case FACTORY_BOOT :
	         video_printf(" => FACTORY MODE\n");
	         break;
	     case RECOVERY_BOOT :
	         video_printf(" => RECOVERY MODE\n");
	         break;
	     case SW_REBOOT :
	         //video_printf(" => SW RESET\n");
	         break;
	     case NORMAL_BOOT :
	         video_printf(" => NORMAL BOOT\n");
	         break;
         case ADVMETA_BOOT:
            video_printf(" => ADVANCED META MODE\n");
            break;
		   case ATE_FACTORY_BOOT:
            video_printf(" => ATE FACTORY MODE\n");
            break;     
          case ALARM_BOOT:
            video_printf(" => ALARM BOOT\n");
            break;
	  default :
            video_printf(" => UNKNOWN BOOT\n");
    }
    return 0;
#endif    
}


/* Transparent to DRAM customize */
static int g_nr_bank;
static int g_rank_size;
BOOT_ARGUMENT *g_boot_arg;
extern BOOT_ARGUMENT *g_boot_arg;
int dram_init(void)
{  
  int i, index, num_record;  
  unsigned int dram_rank_num;
  unsigned int dram_rank_size;
  
  //Get parameters from pre-loader. Get as early as possible
  g_boot_arg = (volatile BOOT_ARGUMENT *)(BOOT_ARGUMENT_LOCATION);
    
  dram_rank_num = g_boot_arg->dram_rank_num;
  dram_rank_size = g_boot_arg->dram_rank_size;
    
  g_nr_bank = dram_rank_num;
  g_rank_size = dram_rank_size;


  if (g_nr_bank == 1){
    gd->bd->bi_dram[0].start = RIL_SIZE;
    gd->bd->bi_dram[0].size =  dram_rank_size - RIL_SIZE;
  }
  else if (g_nr_bank == 2){
    gd->bd->bi_dram[0].start = RIL_SIZE;
    gd->bd->bi_dram[0].size =  dram_rank_size - RIL_SIZE;
    gd->bd->bi_dram[1].start = dram_rank_size * 1;      
    gd->bd->bi_dram[1].size =  dram_rank_size;
  }
  else if (g_nr_bank == 3){
    gd->bd->bi_dram[0].start = RIL_SIZE;
    gd->bd->bi_dram[0].size =  dram_rank_size - RIL_SIZE;
    gd->bd->bi_dram[1].start = dram_rank_size * 1;
    gd->bd->bi_dram[1].size =  dram_rank_size;
    gd->bd->bi_dram[2].start = dram_rank_size * 2;      
    gd->bd->bi_dram[2].size =  dram_rank_size;
  }
  else if (g_nr_bank == 4){
    gd->bd->bi_dram[0].start = RIL_SIZE;
    gd->bd->bi_dram[0].size =  dram_rank_size - RIL_SIZE;
    gd->bd->bi_dram[1].start = dram_rank_size * 1;      
    gd->bd->bi_dram[1].size =  dram_rank_size;
    gd->bd->bi_dram[2].start = dram_rank_size * 2;      
    gd->bd->bi_dram[2].size =  dram_rank_size;
    gd->bd->bi_dram[3].start = dram_rank_size * 3;      
    gd->bd->bi_dram[3].size =  dram_rank_size;
  }
  else{
    //ERROR! DRAM bank number is not correct
  }

  return 0;
}

int get_bank_nr(void)
{
  return g_nr_bank;
}

u32 memory_size(void)
{
	// each bank mapping to 256 mb physical address
	int nr_bank = g_nr_bank;
    int size;
    if(nr_bank == 1)
    {
	  size = g_rank_size * (nr_bank - 1) + gd->bd->bi_dram[nr_bank-1].size + RIL_SIZE;
    }
    else
    {
      size = g_rank_size * (nr_bank - 1) + gd->bd->bi_dram[nr_bank-1].size;
    }
	return size;
}
