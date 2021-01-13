/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif 
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/rtpm_prio.h>

#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#ifdef MT6575
#include "tpd_custom_GT818B.h"
#include <mach/mt6575_pm_ldo.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_boot.h>
#endif
#include "tpd.h"
#include <cust_eint.h>
#include <linux/jiffies.h>

#ifndef TPD_NO_GPIO 
#include "cust_gpio_usage.h"
#endif

#define CREATE_WR_NODE
#include "gt818_fw.h"

extern struct tpd_device *tpd;
extern void uninit_wr_node(void);
extern s32 init_wr_node(struct i2c_client *client);
static int tpd_flag = 0;
static int tpd_halt=0;
static struct task_struct *thread = NULL;
static DECLARE_WAIT_QUEUE_HEAD(waiter);

#ifdef TPD_HAVE_BUTTON 
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_dim_local[TPD_KEY_COUNT][4] = TPD_KEYS_DIM;
#endif
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
static int tpd_wb_start_local[TPD_WARP_CNT] = TPD_WARP_START;
static int tpd_wb_end_local[TPD_WARP_CNT]   = TPD_WARP_END;
#endif
#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
static int tpd_calmat_local[8]     = TPD_CALIBRATION_MATRIX;
static int tpd_def_calmat_local[8] = TPD_CALIBRATION_MATRIX;
#endif

static void tpd_eint_interrupt_handler(void);
static int touch_event_handler(void *unused);
static int tpd_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tpd_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int tpd_i2c_remove(struct i2c_client *client);
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
                                     kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
                                     kal_bool auto_umask);

extern int  gt818_downloader( struct i2c_client *client, unsigned short ver, unsigned char * data );

#define CREATE_WR_NODE
#ifdef CREATE_WR_NODE
extern s32 init_wr_node(struct i2c_client*);
extern void uninit_wr_node(void);
#endif
static int virtual_flag=0;
//#define TPD_DITO_SENSOR
//#define TPD_CONDITION_SWITCH
#define TPD_RESET_ISSUE_WORKAROUND
#define TPD_MAX_RESET_COUNT 3

#define TPD_OK 0

#define TPD_CONFIG_REG_BASE           0x6A2
#define TPD_FREQ_CAL_RESULT           0x70F
#define TPD_MODULE_INFO_REG          0x710
#define TPD_TOUCH_INFO_REG_BASE       0x712
#define TPD_POINT_INFO_REG_BASE       0x722
#define TPD_KEY_INFO_REG_BASE         0x721
#define TPD_VERSION_INFO_REG          0x713
#define TPD_POWER_MODE_REG            0x692
#define TPD_HANDSHAKING_START_REG     0xFFF
#define TPD_HANDSHAKING_END_REG       0x8000
#define TPD_FREQ_REG                  0x1522
#define TPD_SOFT_RESET_MODE     0x01
#define TPD_POINT_INFO_LEN      8
#define TPD_MAX_POINTS          5
#define MAX_TRANSACTION_LENGTH 8
#define I2C_DEVICE_ADDRESS_LEN 2
#define I2C_MASTER_CLOCK       200

//#define TPD_X_RES 320
//#define TPD_Y_RES 480

#ifdef MT6573
#define CHR_CON0        (0xF7000000+0x2FA00)
#endif
#ifdef MT6575
extern kal_bool upmu_is_chr_det(void);
#endif
 
#define MAX_I2C_TRANSFER_SIZE (MAX_TRANSACTION_LENGTH - I2C_DEVICE_ADDRESS_LEN)

#define GT818_CONFIG_PROC_FILE "gt818_config"
#define CONFIG_LEN (106)

struct tpd_info_t
{
    u8 vendor_id_1;
    u8 vendor_id_2;
    u8 product_id_1;
    u8 product_id_2;
    u8 version_1;
    u8 version_2;
};

#ifdef TPD_DITO_SENSOR
#else //SITO
static u8 *cfg_data_version_D = cfg_data_version_D;
//static u8 *cfg_data_with_charger_version_D = cfg_data_with_charger_version_D;
#endif

static struct i2c_client *i2c_client = NULL;
static const struct i2c_device_id tpd_i2c_id[] = {{"gt818b",0},{}};
static unsigned short force[] = {0, 0xBA, I2C_CLIENT_END,I2C_CLIENT_END};
static const unsigned short * const forces[] = { force, NULL };
//static struct i2c_client_address_data addr_data = { .forces = forces,};
static struct i2c_board_info __initdata i2c_tpd={ I2C_BOARD_INFO("gt818b", (0xBA>>1))};
static struct i2c_driver tpd_i2c_driver =
{                       
    .probe = tpd_i2c_probe,                                   
    .remove = tpd_i2c_remove,                           
    .detect = tpd_i2c_detect,                           
    .driver.name = "gt818b", 
    .id_table = tpd_i2c_id,                             
    //.address_data = &addr_data, 
	.address_list = (const unsigned short*) forces,                       
}; 
struct tpd_info_t tpd_info;
static u8 *cfg_data = NULL;
//static u8 *cfg_data_with_charger = NULL;

/* proc file system */
static int i2c_read_bytes( struct i2c_client *client, u16 addr, u8 *rxbuf, int len );
static int i2c_write_bytes( struct i2c_client *client, u16 addr, u8 *txbuf, int len );
static int i2c_write_dummy( struct i2c_client *client, u16 addr );
static struct proc_dir_entry *gt818_config_proc = NULL;

static int gt818_config_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *ptr = page;
    char temp_data[CONFIG_LEN] = {0};
    int i;

    ptr += sprintf( ptr, "==== GT818 config init value====\n" );

    for ( i = 0 ; i < CONFIG_LEN ; i++ )
    {
        ptr += sprintf( ptr, "0x%02X ", cfg_data[i] );

        if ( i%8 == 7 )
            ptr += sprintf( ptr, "\n" );
    }    
      
  ptr += sprintf( ptr, "\n" );
  ptr += sprintf( ptr, "==== GT818 charger init config ====\n" );
  for ( i = 0 ; i < CONFIG_LEN ; i++ )
  {
        //ptr += sprintf( ptr, "0x%02X ", cfg_data_with_charger[i] );

        if ( i%8 == 7 )
    ptr += sprintf( ptr, "\n" );
  }
  ptr += sprintf( ptr, "\n" );
	ptr += sprintf( ptr, "==== GT818 config real value====\n" );
  i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );	
	i2c_read_bytes( i2c_client, TPD_CONFIG_REG_BASE, temp_data, CONFIG_LEN);
  i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );	
	for ( i = 0 ; i < CONFIG_LEN ; i++ )
  {
        ptr += sprintf( ptr, "0x%02X ", temp_data[i] );

        if ( i%8 == 7 )
            ptr += sprintf( ptr, "\n" );
  }    
  ptr += sprintf( ptr, "\n" );
  *eof = 1;
  return ( ptr - page );
}

static int gt818_config_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
    #ifdef MT6573
    u32 temp = *(volatile u32 *)CHR_CON0;
        
    temp &= (1<<13);
    #endif   
    #ifdef MT6575
    kal_bool temp = upmu_is_chr_det();
    #endif 

    TPD_DEBUG("write count %ld\n", count );

    if ( count != (CONFIG_LEN*2 ) )
    {
        TPD_DEBUG("size not match [%d:%ld]\n", CONFIG_LEN*2, count );
        return -EFAULT;
    }

    if (copy_from_user( cfg_data, buffer, count/2))
    {
        TPD_DEBUG("copy from user fail\n");
        return -EFAULT;
    }
    
    /*if (copy_from_user( cfg_data_with_charger, buffer + CONFIG_LEN, count/2))
    {
        TPD_DEBUG("copy from user fail\n");
        return -EFAULT;
    }*/
    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );

    /*if ( temp )
        i2c_write_bytes( i2c_client, TPD_CONFIG_REG_BASE, cfg_data_with_charger, CONFIG_LEN );
    else*/
        i2c_write_bytes( i2c_client, TPD_CONFIG_REG_BASE, cfg_data, CONFIG_LEN );

    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );
    return count;
}

static int gt818_check_data(unsigned char *buffer, int count)
{
	unsigned char buf[128] = {0}; // need > sizeof(buffer)
	int i = 0, error = -1, ret = -1;

	do
	{
    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );		
		ret = i2c_read_bytes( i2c_client, TPD_CONFIG_REG_BASE, buf, CONFIG_LEN);
    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );		
		if(ret)
		{
			TPD_DMESG("read i2c error\n");
			break;
		}
		for(i = 0; i < CONFIG_LEN; i++) // the last one byte will be changed
		{
			if(buf[i] != cfg_data[i]) 
			{
				TPD_DMESG(TPD_DEVICE " fail to write touch panel config, %d bytes, expect:0x%x, real:0x%x\n", i,cfg_data[i], buf[i]);
				error = -1;
				break;
			}
		}
		if(i == CONFIG_LEN)
		{
			TPD_DMESG(TPD_DEVICE " write touch panel config OK, count:%d\n", count);
			error = 0;
			break;
		}

		if(error == -1)
		{
			for(i = 0; i < CONFIG_LEN - 1; i++)
			{
				printk("  0x%02X", buf[i]);
				if(i%8 == 7)
					printk("\n");
			}
      i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );				
			i2c_write_bytes( i2c_client, TPD_CONFIG_REG_BASE, cfg_data, CONFIG_LEN );
      i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );				
		}
	}while(count--);

	return error;
}

static int i2c_read_bytes( struct i2c_client *client, u16 addr, u8 *rxbuf, int len )
{
    u8 buffer[I2C_DEVICE_ADDRESS_LEN];
    u8 retry;
    u16 left = len;
    u16 offset = 0;

    struct i2c_msg msg[2] =
    {
        {
            .addr = ((client->addr&I2C_MASK_FLAG )|(I2C_ENEXT_FLAG )),
            .flags = 0,
            .buf = buffer,
            .len = I2C_DEVICE_ADDRESS_LEN,
            .timing = I2C_MASTER_CLOCK
        },
        {
            .addr = ((client->addr&I2C_MASK_FLAG )|(I2C_ENEXT_FLAG )),
            .flags = I2C_M_RD,
            .timing = I2C_MASTER_CLOCK
        },
    };

    if ( rxbuf == NULL )
        return -1;

    TPD_DEBUG("i2c_read_bytes to device %02X address %04X len %d\n", client->addr, addr, len );

    while ( left > 0 )
    {
        buffer[0] = ( ( addr+offset ) >> 8 ) & 0xFF;
        buffer[1] = ( addr+offset ) & 0xFF;

        msg[1].buf = &rxbuf[offset];

        if ( left > MAX_TRANSACTION_LENGTH )
        {
            msg[1].len = MAX_TRANSACTION_LENGTH;
            left -= MAX_TRANSACTION_LENGTH;
            offset += MAX_TRANSACTION_LENGTH;
        }
        else
        {
            msg[1].len = left;
            left = 0;
        }

        retry = 0;

        while ( i2c_transfer( client->adapter, &msg[0], 2 ) != 2 )
        {
            retry++;

            if ( retry == 20 )
            {
                TPD_DEBUG("I2C read 0x%X length=%d failed\n", addr + offset, len);
                TPD_DMESG("I2C read 0x%X length=%d failed\n", addr + offset, len);
                return -1;
            }
        }
    }

    return 0;
}

static int i2c_write_bytes( struct i2c_client *client, u16 addr, u8 *txbuf, int len )
{
    u8 buffer[MAX_TRANSACTION_LENGTH];
    u16 left = len;
    u16 offset = 0;
    u8 retry = 0;

    struct i2c_msg msg = 
    {
        .addr = ((client->addr&I2C_MASK_FLAG )|(I2C_ENEXT_FLAG )),
        .flags = 0,
        .buf = buffer,
        .timing = I2C_MASTER_CLOCK,
    };


    if ( txbuf == NULL )
        return -1;

    TPD_DEBUG("i2c_write_bytes to device %02X address %04X len %d\n", client->addr, addr, len );

    while ( left > 0 )
    {
        retry = 0;

        buffer[0] = ( (addr+offset) >> 8 ) & 0xFF;
        buffer[1] = ( addr+offset ) & 0xFF;

        if ( left > MAX_I2C_TRANSFER_SIZE )
        {
            memcpy( &buffer[I2C_DEVICE_ADDRESS_LEN], &txbuf[offset], MAX_I2C_TRANSFER_SIZE );
            msg.len = MAX_TRANSACTION_LENGTH;
            left -= MAX_I2C_TRANSFER_SIZE;
            offset += MAX_I2C_TRANSFER_SIZE;
        }
        else
        {
            memcpy( &buffer[I2C_DEVICE_ADDRESS_LEN], &txbuf[offset], left );
            msg.len = left + I2C_DEVICE_ADDRESS_LEN;
            left = 0;
        }

        TPD_DEBUG("byte left %d offset %d\n", left, offset );

        while ( i2c_transfer( client->adapter, &msg, 1 ) != 1 )
        {
            retry++;

            if ( retry == 20 )
            {
                TPD_DEBUG("I2C write 0x%X%X length=%d failed\n", buffer[0], buffer[1], len);
                TPD_DMESG("I2C write 0x%X%X length=%d failed\n", buffer[0], buffer[1], len);
                return -1;
            }
            else
                 TPD_DEBUG("I2C write retry %d addr 0x%X%X\n", retry, buffer[0], buffer[1]);

        }
    }

    return 0;
}

static int i2c_write_dummy( struct i2c_client *client, u16 addr )
{
    u8 buffer[MAX_TRANSACTION_LENGTH];

    struct i2c_msg msg =
    {
        .addr = client->addr,
        .flags = 0,
        .buf = buffer,
        .timing = I2C_MASTER_CLOCK,
        .len = 2
    };

    TPD_DEBUG("i2c_write_dummy to device %02X address %04X\n", client->addr, addr );

    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = (addr) & 0xFF;

    i2c_transfer( client->adapter, &msg, 1 ); 

    return 0;
}

static int tpd_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info)
{
    strcpy(info->type, "mtk-tpd");
    return 0;
}

static int tpd_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{             
    int err = 0;
    int MODULE_TYPE = 0;
#ifdef TPD_RESET_ISSUE_WORKAROUND
    int reset_count = 0;

reset_proc:   
#ifdef MT6573	
    // power on CTP
    mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
#endif
#ifdef MT6575
    //power on, need confirm with SA
    hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_2800, "TP");
    hwPowerOn(MT65XX_POWER_LDO_VGP, VOL_1800, "TP");  

    //mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
    //mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
    //mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);  
	msleep(50);  
#endif
    // set INT mode
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_DISABLE);

    // reset
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    msleep(10);

    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
    msleep(20);

#else
#ifdef MT6575
    //power on, need confirm with SA
    hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_2800, "TP");
    hwPowerOn(MT65XX_POWER_LDO_VGP, VOL_1800, "TP");  
#endif    
    // set deep sleep off
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);  
    msleep(10);  
#ifdef MT6573
    // power down CTP
    mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ZERO);
    msleep(10);

    // power on CTP
    mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
    msleep(10);
#endif    
#endif
     
    memset( &tpd_info, 0, sizeof( struct tpd_info_t ) );
    i2c_write_dummy( client, TPD_HANDSHAKING_START_REG );
    err = i2c_read_bytes( client, TPD_VERSION_INFO_REG, (u8 *)&tpd_info, sizeof( struct tpd_info_t ) );
    i2c_write_dummy( client, TPD_HANDSHAKING_END_REG );		
    if ( err )
    {
        TPD_DMESG(TPD_DEVICE " fail to get tpd info %d\n", err );
#ifdef TPD_RESET_ISSUE_WORKAROUND
        if ( reset_count < TPD_MAX_RESET_COUNT )
        {
            reset_count++;
            goto reset_proc;
        }
#endif
        return err;
    }
    else
    {
        TPD_DMESG( "TPD info\n");
        TPD_DMESG( "vendor %02X %02X\n", tpd_info.vendor_id_1, tpd_info.vendor_id_2 );
        TPD_DMESG( "product %02X %02X\n", tpd_info.product_id_1, tpd_info.product_id_2 );
        TPD_DMESG( "version %02X %02X\n", tpd_info.version_1, tpd_info.version_2 );

        // check if upgradable

        gt818_downloader( client, tpd_info.version_2*256 + tpd_info.version_1, goodix_gt818_firmware );

    }

    i2c_client = client;
   
    // Create proc file system
    gt818_config_proc = create_proc_entry( GT818_CONFIG_PROC_FILE, 0666, NULL);

    if ( gt818_config_proc == NULL )
    {
        TPD_DEBUG("create_proc_entry %s failed\n", GT818_CONFIG_PROC_FILE );
    }
    else 
    {
        gt818_config_proc->read_proc = gt818_config_read_proc;
        gt818_config_proc->write_proc = gt818_config_write_proc;
    }

#ifdef CREATE_WR_NODE
  	init_wr_node(client);
#endif

	i2c_write_dummy( client, TPD_HANDSHAKING_START_REG );
	i2c_read_bytes( client, TPD_MODULE_INFO_REG, &MODULE_TYPE, 1 );
	i2c_write_dummy( client, TPD_HANDSHAKING_END_REG );	
	MODULE_TYPE = MODULE_TYPE&0x07;
	TPD_DMESG(TPD_DEVICE " MODULE_TYPE%d\n", MODULE_TYPE);
 if ( tpd_info.version_1 < 0x9A ) 
    {
        
     TPD_DMESG(TPD_DEVICE " read version %02X, use D version config\n", tpd_info.version_1 );
	 TPD_DMESG(TPD_DEVICE "  D version: 0x7A~0x99, E version 0x9A~0xB9\n" );
	 if(MODULE_TYPE == 0)//BYD
	{	
		TPD_DMESG(TPD_DEVICE "MODULE_TYPE is BYD\n");
		cfg_data = cfg_data_version_F;       //SENSOR_ID CONNECT TO FLOATING
		//cfg_data_with_charger = cfg_data_with_charger_version_D;
	}
	 else if(MODULE_TYPE == 1)//ShenHua
	{  
		TPD_DMESG(TPD_DEVICE "MODULE_TYPE is ShenHua\n");
		cfg_data = cfg_data_version_E;       //SENSOR_ID CONNECT TO VDD
		//cfg_data_with_charger = cfg_data_with_charger_version_D;
	}
	 else if(MODULE_TYPE == 2)//truly
	{                                     
		TPD_DMESG(TPD_DEVICE "MODULE_TYPE is truly\n");
		cfg_data = cfg_data_version_D;       //SENSOR_ID CONNECT TO GND      
		//cfg_data_with_charger = cfg_data_with_charger_version_D;
	}
  	}
    else
    {
        TPD_DMESG(TPD_DEVICE " unknow Chip version %02X ,use B version config\n", tpd_info.version_1 );
        cfg_data = cfg_data_version_E;
        //cfg_data_with_charger = cfg_data_with_charger_version_E;
    }
    // setting resolution, RES_X, RES_Y
//#if RES_AUTO_CONFIG
    cfg_data[59] = (TPD_X_RES&0xff);//(TPD_RES_X&0xff);
    cfg_data[60] = ((TPD_X_RES>>8)&0xff);//((TPD_RES_X>>8)&0xff);
    cfg_data[61] = (TPD_Y_RES&0xff);//(TPD_RES_Y&0xff);
    cfg_data[62] = ((TPD_Y_RES>>8)&0xff);//((TPD_RES_Y>>8)&0xff);    
//#endif 
  i2c_write_dummy( client, TPD_HANDSHAKING_START_REG );
  err = i2c_write_bytes( client, TPD_CONFIG_REG_BASE, cfg_data, CONFIG_LEN );
  i2c_write_dummy( client, TPD_HANDSHAKING_END_REG );	
#if 0 //#ifdef TPD_RESET_ISSUE_WORKAROUND
	err = gt818_check_data(cfg_data, TPD_MAX_RESET_COUNT); 
	if ( err )
	{
		TPD_DMESG(TPD_DEVICE " retry TPD_MAX_RESET_COUNT fail to write tpd cfg %d\n", err );
		return err;
	}
#endif

    thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);

    if (IS_ERR(thread))
    { 
        err = PTR_ERR(thread);
        TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", err);
    }
 

#ifndef TPD_RESET_ISSUE_WORKAROUND
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    msleep(10);
#endif

    // set INT mode
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_DISABLE);
    //mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);

    msleep(50);

    mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
    mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
    //mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1);
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_POLARITY_HIGH, tpd_eint_interrupt_handler, 1);
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);

#ifndef TPD_RESET_ISSUE_WORKAROUND
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#endif

    tpd_load_status = 1;
    
    return 0;
}

static void tpd_eint_interrupt_handler(void)
{ 
    //TPD_DEBUG_PRINT_INT;
    tpd_flag=1;
    wake_up_interruptible(&waiter);
} 
static int tpd_i2c_remove(struct i2c_client *client)
{
	#ifdef CREATE_WR_NODE
	uninit_wr_node();
	#endif
    return 0;
}

static void tpd_down(int x, int y, int size, int id)
{
    input_report_abs(tpd->dev, ABS_PRESSURE, size/100);
    input_report_key(tpd->dev, BTN_TOUCH, 1);
    input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, size/100);
    input_report_abs(tpd->dev, ABS_MT_WIDTH_MAJOR, 0);
    input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
    /* track id Start 0 */
    input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, id-1);
    input_mt_sync(tpd->dev);
    //input_sync(tpd->dev);
    //TPD_DEBUG_PRINT_POINT( x, y, 1 );
    TPD_EM_PRINT(x, y, x, y, size, 1);
	TPD_DMESG("down x= %d , y = %d \n",x,y);
    if (FACTORY_BOOT == get_boot_mode() || RECOVERY_BOOT == get_boot_mode())
    {   
        tpd_button(x, y, 1);  
    }
    if(y > TPD_RES_Y) //virtual key debounce to avoid android ANR issue
    {
	    msleep(50);
	    printk("D virtual key \n");
	    virtual_flag = 1;
    }
}

static void tpd_up(int x, int y, int id)
{
    //input_report_abs(tpd->dev, ABS_PRESSURE, 0);
    input_report_key(tpd->dev, BTN_TOUCH, 0);
    input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
    input_report_abs(tpd->dev, ABS_MT_WIDTH_MAJOR, 0);
    //input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    //input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
    /* track id Start 0 */
    //input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, id-1);
    input_mt_sync(tpd->dev);
    //input_sync(tpd->dev);
    TPD_EM_PRINT(x, y, x, y, id, 0);
    TPD_DMESG("up x= %d , y = %d \n",x,y);
    if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
    {   
        tpd_button(x, y, 0); 
    }      
     if(y > TPD_RES_Y) {
	    virtual_flag = 1;
    } 
    //TPD_DEBUG_PRINT_POINT( x, y, 0 );
}

static int touch_event_handler(void *unused)
{
    struct sched_param param = { .sched_priority = RTPM_PRIO_TPD }; 
    int x, y, id, size, finger_num = 0;
    static u8 buffer[ TPD_POINT_INFO_LEN*TPD_MAX_POINTS ];
    static char buf_status;
    static u8 id_mask = 0;
    u8 cur_mask;
    int idx;
    static int x_history[TPD_MAX_POINTS+1];
    static int y_history[TPD_MAX_POINTS+1];

    u8 key = 0, key1 = 0, key2 = 0, key3 = 0;
    static u8 key1_old = 0,key2_old = 0,key3_old = 0; 
	
//#ifdef TPD_CONDITION_SWITCH
    u8 charger_plug = 0;
    u8 *cfg;
    u32 temp;    
//#endif

    sched_setscheduler(current, SCHED_RR, &param); 

    do
    {
        set_current_state(TASK_INTERRUPTIBLE);

        while ( tpd_halt )
        {
            tpd_flag = 0;
            msleep(20);
        }

        wait_event_interruptible(waiter, tpd_flag != 0);
        tpd_flag = 0;
        TPD_DEBUG_SET_TIME;
        set_current_state(TASK_RUNNING); 
        
        i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );

#ifdef TPD_CONDITION_SWITCH
        /* Workaround for common mode noise */
#ifdef MT6573        
        temp = *(volatile u32 *)CHR_CON0;
        temp &= (1<<13);
#endif
#ifdef MT6575
        temp = upmu_is_chr_det();
        //TPD_DMESG("check charge, status:%d \n", upmu_is_chr_det());
#endif        
        cfg = NULL;

        if ( temp ) // charger is on
        {
            if ( charger_plug == 0 )
            {
                TPD_DEBUG("update configure for charger\n");
                //TPD_DMESG("update configure for charger\n");
                charger_plug = 1;
                //cfg = cfg_data_with_charger;
            }
        }
        else
        {
            if ( charger_plug == 1 )
            {
                TPD_DEBUG("update configure for no charger\n");
                //TPD_DMESG("update configure for no charger\n");
                charger_plug = 0;
                cfg = cfg_data;
            }
        }

        if ( cfg )
        {
            i2c_write_bytes( i2c_client, TPD_CONFIG_REG_BASE, cfg, CONFIG_LEN );
            i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );
            continue;
        }
#endif

        i2c_read_bytes( i2c_client, TPD_TOUCH_INFO_REG_BASE, buffer, 1);
        TPD_DEBUG("[mtk-tpd] STATUS : %x\n", buffer[0]);
#ifdef TPD_HAVE_BUTTON		      
        i2c_read_bytes( i2c_client, TPD_KEY_INFO_REG_BASE, &key, 1);
        TPD_DEBUG("[mtk-tpd] STATUS : %x\n", key);
        key = key&0x0f;
		
	key1 = (key & 0x01);
        key2 = (key & 0x02);
	#ifdef GN_MTK_BSP_GOOIDX_HOMEKEY_SUPPORT
	key3 = (key & 0x04);
	#endif	
   	if(key1 == 1) {
                tpd_down( key_1, 1, 100);
	}
	else if((key1_old == 1)&(key1 == 0)) {
		tpd_up( key_1, 1); 
	}
	if(key2 == 2) {
                tpd_down( key_2, 1, 100);
	}
	else if((key2_old == 2)&(key2 == 0)) {
		tpd_up( key_2, 1); 
	}
	#ifdef GN_MTK_BSP_GOOIDX_HOMEKEY_SUPPORT
	if(key3 == 4) {
                tpd_down( key_3, 1, 100);
	}
	else if((key3_old == 4)&(key3 == 0)) {
		tpd_up( key_3, 1); 
	}
	#endif
        key1_old = key1;
        key2_old = key2;
	#ifdef GN_MTK_BSP_GOOIDX_HOMEKEY_SUPPORT
	key3_old = key3;
	#endif
#endif

        finger_num = buffer[0] & 0x0f;
        buf_status = buffer[0] & 0xf0;
     
        if ( tpd == NULL || tpd->dev == NULL )
        {
             i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );  //kuuga  add 11082401
            continue;
        }
		
	 if ( finger_num == 0x0f )        //ESD-TEST reset, send cfg again!!
        {
            cfg = cfg_data;
            i2c_write_bytes( i2c_client, TPD_CONFIG_REG_BASE, cfg, CONFIG_LEN );
             i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );  //kuuga  add 11082401
            continue;
        }
         if ( (buf_status&0x30) != 0x20 )        //data not ready£¬so return
        {
             i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );  //kuuga  add 11082401
            continue;
        }

        
        
        if ( finger_num > 5 )        //abnormal state £¬so return
        {
             i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );  //kuuga  add 11082401
            continue;
        }
       
        if(finger_num > 2)
         finger_num = 2;
        if ( finger_num )
        {
            i2c_read_bytes( i2c_client, TPD_POINT_INFO_REG_BASE, buffer, finger_num*TPD_POINT_INFO_LEN);
#if 0
            {
                u8 buf_tmp[ TPD_POINT_INFO_LEN*TPD_MAX_POINTS ];
                int loop = 0;

                for ( loop = 0 ; loop < 20 ; loop++ )
                {
                    i2c_read_bytes( i2c_client, TPD_POINT_INFO_REG_BASE, buf_tmp, finger_num*TPD_POINT_INFO_LEN);
                }
            }
#endif
        }
        else
        {
            //In order to release INT
            i2c_read_bytes( i2c_client, TPD_POINT_INFO_REG_BASE, buffer, 1);
        }
        
        cur_mask = 0;

        for ( idx = 0 ; idx < finger_num ; idx++ )
        {
            u8 *ptr = &buffer[ idx*TPD_POINT_INFO_LEN ];
            id = ptr[0];

            if ( id < TPD_MAX_POINTS+1 )
            {
                x = ptr[1] + (((int)ptr[2]) << 8);
                y = ptr[3] + (((int)ptr[4]) << 8);
                size = ptr[5] + (((int)ptr[6]) << 8);

                //if ( x == 0 )
                //    x = 1;
#if 1
                if(TPD_X_RES== x)
                {
                  x=x-1;
                }
				if(0==x)
				{
					x=x+1;
				}
				if(0==y)
				{ 
					y=y+1;
				}
				if(TPD_Y_RES==y)
				{
					y=y-1;
				}
#endif
				#ifdef GN_MTK_BSP_GOOIDX_EDGE_REBOUND_5POINT
				
				if((x >= 1)&&(x <= 10))
				{
					x = x + 5;
				}
				else if((x >= 10)&&(x <= 20))
				{
					x = x + 3;
				}
				else if((x >= 460)&&(x <= 470))
				{
					x = x - 3;
				}
				else if((x >= 470)&&(x <= 480))
				{
					x = x - 5;
				}
		
				if((y >= 1)&&(y <= 16))
				{
					y = y + 5;
				}
				else if((y >= 16)&&(y <= 26))
				{
					y = y + 3;
				}
				else if((y >= 774)&&(y <= 784))
				{
					y = y - 3;
				}
				else if((y >= 784)&&(y <= 800))
				{
					y = y - 5;
				}
				#endif
				tpd_down(x, y, size, id);

                cur_mask |= ( 1 << id );
                x_history[id] = x;
                y_history[id] = y;
            }
            else
                TPD_DEBUG("Invalid id %d\n", id );
        }         
#if 0              
        if ( cur_mask != id_mask )
        {
            u8 diff = cur_mask^id_mask;
            idx = 0;

            while ( diff )
            {
                if ( ( ( diff & 0x01 ) == 1 ) &&
                     ( ( cur_mask >> idx ) & 0x01 ) == 0 )
                {
                    // check if key release
                    
					
						tpd_up(TPD_WARP_X(x_history[idx]), y_history[idx], id);  
                
                }

                diff = ( diff >> 1 );
                idx++;
            }
            id_mask = cur_mask;
        }

        if ( tpd != NULL && tpd->dev != NULL )
            input_sync(tpd->dev);
#endif
//==============================
		//linux kernel update from 2.6.35 --> 3.0
        if(finger_num)
        {
			if ( tpd != NULL && tpd->dev != NULL )
			{
				input_sync(tpd->dev);
				virtual_flag = 0;
			}
		}
		else
		{
		   	if ( tpd != NULL && tpd->dev != NULL )
		   	{
		        //input_mt_sync(tpd->dev);
				if(virtual_flag == 0) 
		      	{
		      		tpd_up( TPD_WARP_X(x_history[idx]), TPD_WARP_Y(y_history[idx]), idx); 
		      		input_sync(tpd->dev);
				} 
				else 
				{
			    	input_sync(tpd->dev);
					virtual_flag = 0;
				}
		   	}
		}

//==============================



        i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );

    } while ( !kthread_should_stop() ); 

    return 0;
}

static int tpd_local_init(void) 
{

    if(i2c_add_driver(&tpd_i2c_driver)!=0)
    {
        TPD_DMESG("unable to add i2c driver.\n");
        return -1;
    }
    if(tpd_load_status == 0)
    {
    	TPD_DMESG("add error touch panel driver.\n");
    	i2c_del_driver(&tpd_i2c_driver);
    	return -1;
    }

#ifdef TPD_HAVE_BUTTON     
    tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif   
  
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))    
    TPD_DO_WARP = 1;
    memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
    memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif 

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
    memcpy(tpd_calmat, tpd_def_calmat_local, 8*4);
    memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);	
#endif  

    // set vendor string
    tpd->dev->id.vendor =  (tpd_info.vendor_id_2 << 8 ) | tpd_info.vendor_id_1;
    tpd->dev->id.product = (tpd_info.product_id_2 << 8 ) | tpd_info.product_id_1;
    tpd->dev->id.version = (tpd_info.version_2 << 8 ) | tpd_info.version_1;

    TPD_DMESG("end %s, %d\n", __FUNCTION__, __LINE__);  
    tpd_type_cap = 1;

    return 0;
}

/* Function to manage low power suspend */
//void tpd_suspend(struct i2c_client *client, pm_message_t message)
static void tpd_suspend( struct early_suspend *h )
{
    u8 mode = 0x01;
#if 0 // workaround for force tpd into sleep mode   
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_EINT_PIN, GPIO_OUT_ZERO);
#endif
    tpd_halt = 1;     
    msleep(1);     
    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );
    i2c_write_bytes( i2c_client, TPD_POWER_MODE_REG, &mode, 1 );
    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );    
    mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
#if 0//#ifdef MT6577, using PMIC Power off leakage 0.3mA, sleep mode: 0.1mA    
    hwPowerDown(MT65XX_POWER_LDO_VGP2, "TP");
    hwPowerDown(MT65XX_POWER_LDO_VGP, "TP");
    /* Pull down EINT PIN and RST PIN */    
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_DISABLE);
    mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_DOWN);  
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_RST_PIN, GPIO_PULL_DISABLE);
    mt_set_gpio_pull_select(GPIO_CTP_RST_PIN, GPIO_PULL_DOWN);      
#endif    
    //mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    //return 0;
}

/* Function to manage power-on resume */
//void tpd_resume(struct i2c_client *client)
static void tpd_resume( struct early_suspend *h )
{
    TPD_DMESG(TPD_DEVICE " tpd_resume start \n"); 	
#if 0 //#ifdef MT6577, using PMIC Power off leakage 0.3mA, sleep mode: 0.1mA   
    hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_2800, "TP");
    hwPowerOn(MT65XX_POWER_LDO_VGP, VOL_1800, "TP"); 
#endif    
#ifdef TPD_RESET_ISSUE_WORKAROUND
    struct tpd_info_t tpd_info;
    int err;
    char *crash = 0;
#if 1 //#ifdef MT6573, using PMIC Power off leakage 0.3mA, sleep mode: 0.1mA      
    // use raising edge of INT to wakeup
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_EINT_PIN, GPIO_OUT_ZERO);
    msleep(1);
    mt_set_gpio_out(GPIO_CTP_EINT_PIN, GPIO_OUT_ONE);
    msleep(1);
    mt_set_gpio_out(GPIO_CTP_EINT_PIN, GPIO_OUT_ZERO);
    msleep(1);
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
 #endif
#if 0 //#ifdef MT6577, using PMIC Power off leakage 0.3mA, sleep mode: 0.1mA 
     // set INT mode
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_DISABLE);

    // reset
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    msleep(1);

    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
    msleep(20);// msleep(20)
    
        // set INT mode
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_DISABLE);
    //mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
#endif   
#endif

    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM); 
    //mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);

#ifdef TPD_RESET_ISSUE_WORKAROUND

    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_START_REG );    
    memset( &tpd_info, 0, sizeof( struct tpd_info_t ) );
    err = i2c_read_bytes( i2c_client, TPD_VERSION_INFO_REG, (u8 *)&tpd_info, sizeof( struct tpd_info_t ) );

    if ( err )
    {
        TPD_DMESG(TPD_DEVICE " fail to get tpd info %d\n", err ); 
	 mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    	 mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    	 mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    	 msleep(10);
    	 mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
    	 msleep(50);
	 err = i2c_write_bytes( i2c_client, TPD_CONFIG_REG_BASE, cfg_data, CONFIG_LEN );  
        if(err)
    	{
    	  	TPD_DMESG( "TPD fail to write config\n");
    	}
		
    }
    else
    {
        TPD_DMESG( "TPD info\n");
        TPD_DMESG( "vendor %02X %02X\n", tpd_info.vendor_id_1, tpd_info.vendor_id_2 );
        TPD_DMESG( "product %02X %02X\n", tpd_info.product_id_1, tpd_info.product_id_2 );
        TPD_DMESG( "version %02X %02X\n", tpd_info.version_1, tpd_info.version_2 );
    }

    i2c_write_dummy( i2c_client, TPD_HANDSHAKING_END_REG );
#endif
    tpd_halt = 0;
    TPD_DMESG(TPD_DEVICE " tpd_resume end \n" ); 	    
    //return 0;
}

static struct tpd_driver_t tpd_device_driver =
{
    .tpd_device_name = "gt818",
    .tpd_local_init = tpd_local_init,
    .suspend = tpd_suspend,
    .resume = tpd_resume,
#ifdef TPD_HAVE_BUTTON
    .tpd_have_button = 1,
#else
    .tpd_have_button = 0,
#endif		
};

/* called when loaded into kernel */
static int __init tpd_driver_init(void)
{
    TPD_DMESG("MediaTek gt818 touch panel driver init\n");
    i2c_register_board_info(0, &i2c_tpd, 1);
    if ( tpd_driver_add(&tpd_device_driver) < 0)
        TPD_DMESG("add generic driver failed\n");

    return 0;
}

/* should never be called */
static void __exit tpd_driver_exit(void)
{
    TPD_DMESG("MediaTek gt818 touch panel driver exit\n");
    //input_unregister_device(tpd->dev);
    tpd_driver_remove(&tpd_device_driver);
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);


