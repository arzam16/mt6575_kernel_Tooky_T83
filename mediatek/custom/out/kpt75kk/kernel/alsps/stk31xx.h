/* 
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*
 * Definitions for stk31xx als/ps sensor chip.
 */
#ifndef __STK31xx_H__
#define __STK31xx_H__

#include <linux/ioctl.h>
extern int stk31xx_als_cmd_val;
extern int stk31xx_ps_cmd_val;
extern int stk31xx_ps_gain_setting;

/*ALSPS REGS*/
#define ALS_CMD          0x01
#define ALS_DT1          0x02
#define ALS_DT2          0x03
#define ALS_THDH1        0x04
#define ALS_THDH2        0x05
#define ALS_THDL1        0x06
#define ALS_THDL2        0x07
#define ALSPS_STATUS     0x08
#define PS_CMD           0x09
#define PS_DT            0x0A
#define PS_THDH          0x0B
#define PS_THDL          0x0C
#define PS_GAIN          0x82

/*ALS Command*/
#define SD_ALS      (1      << 0)
#define INT_ALS     (1      << 1)
#define IT_ALS      (0x03   << 2)
#define THD_ALS     (0x03   << 4)
#define GAIN_ALS    (0x03   << 6)

/*Proximity sensor command*/
#define SD_PS       (1      << 0)
#define INT_PS      (1      << 1)
#define IT_PS       (0x03   << 2)
#define DR_PS       (1      << 4)
#define SLP_PS      (0x03   << 5)
#define INTM_PS     (1      << 7)

#define CONFIG_STK_ALS_CHANGE_THRESHOLD	5

#endif
