/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Source code of Sensor driver
 *
 *
 * Author:
 * -------
 *   PC Huang (MTK02204)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * 10 27 2010 sean.cheng
 * [ALPS00130222] [MPEG4 recording] Frame rate is 30fps by nigh mode
 * .check in for YUV night mode fps = 15
 *
 * 10 12 2010 sean.cheng
 * [ALPS00021722] [Need Patch] [Volunteer Patch][Camera]MT6573 Camera related function
 * .rollback the lib3a for mt6573 camera related files
 *
 * 09 10 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .alps dual sensor
 *
 * 09 02 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .roll back dual sensor
 *
 * 07 27 2010 sean.cheng
 * [ALPS00003112] [Need Patch] [Volunteer Patch] ISP/Sensor driver modification for Customer support
 * .1. add master clock switcher 
 *  2. add master enable/disable 
 *  3. add dummy line/pixel for sensor 
 *  4. add sensor driving current setting
 *
 * 07 19 2010 sean.cheng
 * [ALPS00002994][Need Patch] [Volunteer Patch] E1K YUV sensor update customer parameters 
 * .Optimize the sensor paramter & flicker caputre shutter setting
 *
 * 07 06 2010 sean.cheng
 * [ALPS00121501][Need Resolved][E1K][camera]The preview display abnormal when switch scen mode between auto  and night 
 * .Remove the gamma setting in night mode
 *
 * 07 06 2010 sean.cheng
 * [ALPS00121385][Camera] set EV as one non-zero value, after capturing one iamge , the value seems to be set to zero 
 * .change effect_off setting to reserve the EV setting
 *
 * 07 02 2010 sean.cheng
 * [ALPS00121364][Camera] when set AE value, the effect is disabled 
 * .Modify exposure setting to let effect remain
 *
 * 07 01 2010 sean.cheng
 * [ALPS00121215][Camera] Change color when switch low and high 
 * .Add video delay frame.
 *
 * 06 18 2010 sean.cheng
 * [ALPS00008131][E1K][Camera]Screen will flash some times in this case 
 * .Add 2 frame delay for capture back to preview
 *
 * 06 13 2010 sean.cheng
 * [ALPS00002514][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for E1k Camera 
 * .Modify e1k sensor setting
 *
 * 06 13 2010 sean.cheng
 * [ALPS00002514][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for E1k Camera 
 * .
 * 1. Add set zoom factor and capdelay frame for YUV sensor 
 * 2. Modify e1k sensor setting
 *
 * 06 09 2010 sean.cheng
 * [ALPS00007960][E1K][Camera]There will be a yellow block show on screen left side when preview 
 * .Change the VGA setting
 *
 * 05 27 2010 sean.cheng
 * [ALPS00002309][Need Patch] [Volunteer Patch] ALPS.10X.W10.24 Volunteer patch for E1k YUV Sensor support 
 * .
 * Update OV3660 yuv sensor init setting
 *
 * 05 26 2010 sean.cheng
 * [ALPS00001357][Meta]CameraTool 
 * .
 * Update OV3660 yuv sensor init setting
 *
 * 05 25 2010 sean.cheng
 * [ALPS00001357][Meta]CameraTool 
 * .
 * Add OV3660 YUV sensor driver support
 *
 * 05 03 2010 sean.cheng
 * [ALPS00001357][Meta]CameraTool 
 * .
 * Fix OV3660 YUV sensor frame rate to 30fps in vidoe mode
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Mar 1 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Feb 24 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Nov 24 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Oct 29 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Oct 27 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Aug 13 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Aug 5 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Jul 17 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Jul 7 2009 mtk01051
 * [DUMA00008051] [Camera Driver] Add drivers for camera high ISO binning mode.
 * Add ISO query info for Sensor
 *
 * May 18 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * May 16 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * May 16 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * Apr 7 2009 mtk02204
 * [DUMA00004012] [Camera] Restructure and rename camera related custom folders and folder name of came
 * 
 *
 * Mar 27 2009 mtk02204
 * [DUMA00002977] [CCT] First check in of MT6516 CCT drivers
 *
 *
 * Mar 25 2009 mtk02204
 * [DUMA00111570] [Camera] The system crash after some operations
 *
 *
 * Mar 20 2009 mtk02204
 * [DUMA00002977] [CCT] First check in of MT6516 CCT drivers
 *
 *
 * Mar 2 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Feb 24 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Dec 27 2008 MTK01813
 * DUMA_MBJ CheckIn Files
 * created by clearfsimport
 *
 * Dec 10 2008 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Oct 27 2008 mtk01051
 * [DUMA00000851] Camera related drivers check in
 * Modify Copyright Header
 *
 * Oct 24 2008 mtk02204
 * [DUMA00000851] Camera related drivers check in
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
//#include <mach/mt6516_pll.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "ov3660yuv_Sensor.h"
#include "ov3660yuv_Camera_Sensor_para.h"
#include "ov3660yuv_CameraCustomized.h"

#define OV3660YUV_DEBUG
#ifdef OV3660YUV_DEBUG
#define SENSORDB printk
#else
#define SENSORDB(x,...)
#endif

extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);
static int sensor_id_fail = 0; 
#define OV3660_write_cmos_sensor(addr, para) iWriteReg((u16) addr , (u32) para ,1,OV3660_WRITE_ID)
#define OV3660_write_cmos_sensor_2(addr, para, bytes) iWriteReg((u16) addr , (u32) para ,bytes,OV3660_WRITE_ID)
kal_uint16 OV3660_read_cmos_sensor(kal_uint32 addr)
{
kal_uint16 get_byte=0;
    iReadReg((u16) addr ,(u8*)&get_byte,OV3660_WRITE_ID);
    return get_byte;
}


/*******************************************************************************
* // Adapter for Winmo typedef 
********************************************************************************/

#define Sleep(ms) mdelay(ms)
#define RETAILMSG(x,...)
#define TEXT


/*******************************************************************************
* // End Adapter for Winmo typedef 
********************************************************************************/


#define	OV3660_LIMIT_EXPOSURE_LINES				(1253)
#define	OV3660_VIDEO_NORMALMODE_30FRAME_RATE       (30)
#define	OV3660_VIDEO_NORMALMODE_FRAME_RATE         (15)
#define	OV3660_VIDEO_NIGHTMODE_FRAME_RATE          (7.5)
#define BANDING50_30HZ
/* Global Valuable */

static kal_uint32 zoom_factor = 0; 

static kal_uint8 OV3660_exposure_line_h = 0, OV3660_exposure_line_l = 0,OV3660_extra_exposure_line_h = 0, OV3660_extra_exposure_line_l = 0;

static kal_bool OV3660_gPVmode = KAL_TRUE; //PV size or Full size
static kal_bool OV3660_VEDIO_encode_mode = KAL_FALSE; //Picture(Jpeg) or Video(Mpeg4)
static kal_bool OV3660_sensor_cap_state = KAL_FALSE; //Preview or Capture

static kal_uint16 OV3660_dummy_pixels=0, OV3660_dummy_lines=0;
kal_uint32 OV3660_FULL_dummy_pixels = 0;
kal_uint32 OV3660_FULL_dummy_lines = 0;


static kal_uint16 OV3660_exposure_lines=0, OV3660_extra_exposure_lines = 0;


static kal_int8 OV3660_DELAY_AFTER_PREVIEW = -1;

static kal_uint8 OV3660_Banding_setting = AE_FLICKER_MODE_50HZ;  //Wonder add

/****** OVT 6-18******/
static kal_uint16 OV3660_Capture_Max_Gain16= 6*16;
static kal_uint16 OV3660_Capture_Gain16=0 ;    
static kal_uint16 OV3660_Capture_Shutter=0;
static kal_uint16 OV3660_Capture_Extra_Lines=0;

static kal_uint16  OV3660_PV_Dummy_Pixels =0, OV3660_Capture_Dummy_Pixels =0, OV3660_Capture_Dummy_Lines =0;
static kal_uint16  OV3660_PV_Gain16 = 0;
static kal_uint16  OV3660_PV_Shutter = 0;
static kal_uint16  OV3660_PV_Extra_Lines = 0;

kal_uint16 OV3660_sensor_gain_base=0,OV3660_FAC_SENSOR_REG=0,OV3660_iOV3660_Mode=0,OV3660_max_exposure_lines=0;
kal_uint32 OV3660_capture_pclk_in_M=520,OV3660_preview_pclk_in_M=390,OV3660_PV_dummy_pixels=0,OV3660_PV_dummy_lines=0,OV3660_isp_master_clock=0;
static kal_uint32  OV3660_preview_pclk = 0, OV3660_capture_pclk = 0;
kal_bool OV3660_Night_mode = KAL_FALSE;
kal_bool OV3660_Y_Target_L = 64; 
kal_bool OV3660_Y_Target_H = 72; 

OV3660_OP_TYPE OV3660_g_iOV3660_Mode = OV3660_MODE_NONE;

static kal_uint32  OV3660_sensor_pclk=390;
kal_bool OV3660_VEDIO_MPEG4 = KAL_FALSE; //Picture(Jpeg) or Video(Mpeg4);


UINT8 OV3660_PixelClockDivider=0;

static kal_bool OV3660_AWB_ENABLE = KAL_TRUE; 
static kal_bool OV3660_AE_ENABLE = KAL_TRUE; 

static kal_uint32 Capture_Shutter = 0; 
static kal_uint32 Capture_Gain = 0; 

int XVCLK = 2600;
// real clock/10000
int preview_sysclk, preview_HTS;
int AE_Target = 52;
int AE_high, AE_low;
int m_iCombo_NightMode = 0;


MSDK_SENSOR_CONFIG_STRUCT OV3660SensorConfigData;

void OV3660_set_dummy(kal_uint16 pixels, kal_uint16 lines)
{
    kal_uint8 temp_reg1, temp_reg2;
    kal_uint16 temp_reg;

    /*Very Important: The line_length must < 0x1000, it is to say 0x3028 must < 0x10, or else the sensor will crash*/
    /*The dummy_pixel must < 2156*/
    if (pixels >= 2156) 
        pixels = 2155;
    if (pixels < 0x100)
    {
        OV3660_write_cmos_sensor(0x302c,(pixels&0xFF)); //EXHTS[7:0]
        temp_reg = OV3660_FULL_PERIOD_PIXEL_NUMS;
        OV3660_write_cmos_sensor(0x3029,(temp_reg&0xFF));         //H_length[7:0]
        OV3660_write_cmos_sensor(0x3028,((temp_reg&0xFF00)>>8));  //H_length[15:8]
    }
    else
    {
        OV3660_write_cmos_sensor(0x302c,0);
        temp_reg = pixels + OV3660_FULL_PERIOD_PIXEL_NUMS;
        OV3660_write_cmos_sensor(0x3029,(temp_reg&0xFF));         //H_length[7:0]
        OV3660_write_cmos_sensor(0x3028,((temp_reg&0xFF00)>>8));  //H_length[15:8]
    }

    // read out and + line
    temp_reg1 = OV3660_read_cmos_sensor(0x302B);    // VTS[b7~b0]
    temp_reg2 = OV3660_read_cmos_sensor(0x302A);    // VTS[b15~b8]
    temp_reg = (temp_reg1 & 0xFF) | (temp_reg2 << 8);

    temp_reg += lines;

    OV3660_write_cmos_sensor(0x302B,(temp_reg&0xFF));         //VTS[7:0]
    OV3660_write_cmos_sensor(0x302A,((temp_reg&0xFF00)>>8));  //VTS[15:8]
}    /* OV3660_set_dummy */

kal_uint16 OV3660_read_OV3660_gain(void)
{
	// read gain, 16 = 1x
	int gain16;
	gain16 = OV3660_read_cmos_sensor(0x350a) & 0x03;
	gain16 = (gain16<<8) + OV3660_read_cmos_sensor(0x350b);
	return gain16;
}  /* OV3660_read_OV3660_gain */


kal_uint16 OV3660_read_shutter(void)
{
	// read shutter, in number of line period
	int shutter;
	shutter = (OV3660_read_cmos_sensor(0x03500) & 0x0f);
	shutter = (shutter<<8) + OV3660_read_cmos_sensor(0x3501);
	shutter = (shutter<<4) + (OV3660_read_cmos_sensor(0x3502)>>4);

	return shutter;
}    /* OV3660_read_shutter */

void OV3660_write_OV3660_gain(kal_uint16 gain16)
{    
	// write gain, 16 = 1x
	int temp;
	gain16 = gain16 & 0x3ff;
	temp = gain16 & 0xff;
	OV3660_write_cmos_sensor(0x350b, temp);
	temp = gain16>>8;
	OV3660_write_cmos_sensor(0x350a, temp);
	return 0;
}  /* OV3660_write_OV3660_gain */

static void OV3660_write_shutter(kal_uint16 shutter)
{
	// write shutter, in number of line period
	int temp;
	shutter = shutter & 0xffff;
	temp = shutter & 0x0f;
	temp = temp<<4;
	OV3660_write_cmos_sensor(0x3502, temp);
	temp = shutter & 0xfff;
	temp = temp>>4;
	OV3660_write_cmos_sensor(0x3501, temp);
	temp = shutter>>12;
	OV3660_write_cmos_sensor(0x3500, temp);
	return 0;

}    /* OV3660_write_shutter */
int OV3660_get_sysclk()
{
	// calculate sysclk
	bool pllbypass, PclkManual;
	int temp1, temp2;
	int Multiplier, SysDiv, PreDiv2x, RootDiv, Seld52x, PclkDiv, VCO, PLLCLK, sysclk, PCLK;
	int Pre_div2x_map[] = {
	2, 3, 4, 6};
	int Seld52x_map[] = {
	2, 2, 4, 5};
	temp1 = OV3660_read_cmos_sensor(0x303a);
	if (temp1 & 0x80) {
	pllbypass = true;
	}
	else {
	pllbypass = false;
	}
	temp1 = OV3660_read_cmos_sensor(0x303b);
	Multiplier = temp1 & 0x1f;
	temp1 = OV3660_read_cmos_sensor(0x303c);
	SysDiv = temp1 & 0x0f;
	temp1 = OV3660_read_cmos_sensor(0x303d);
	temp2 = (temp1>>4) & 0x03;
	PreDiv2x = Pre_div2x_map[temp2];
	if (temp1 & 0x40) {
	RootDiv = 2;
	}
	else {
	RootDiv = 1;
	}
	temp2 = temp1 & 0x03;
	Seld52x = Seld52x_map[temp2];
	temp1 = OV3660_read_cmos_sensor(0x3824);
	PclkDiv = temp1 & 0x1f;
	temp1 = OV3660_read_cmos_sensor(0x460c);
	if (temp1 & 0x02) {
		PclkManual = true;
	}
	else {
		PclkManual = false;
	}
	VCO = XVCLK * Multiplier * RootDiv * 2 / PreDiv2x;
	if (pllbypass) {
		PLLCLK = XVCLK;
	}
	else {
		PLLCLK = VCO * 2 / SysDiv / Seld52x;
	}
	sysclk = PLLCLK / 4;
	if (PclkManual) {
		PCLK = PLLCLK / 2 / PclkDiv;
	}
	else {
		PCLK = PLLCLK / 2;
	}
	return sysclk;
}

int OV3660_get_HTS()
{
	// read HTS from register settings
	int HTS;
	HTS = OV3660_read_cmos_sensor(0x380c);
	HTS = (HTS<<8) + OV3660_read_cmos_sensor(0x380d);
	return HTS;
}

int OV3660_get_VTS()
{
	// read VTS from register settings
	int VTS;
	VTS = OV3660_read_cmos_sensor(0x380e);
	VTS = (VTS<<8) + OV3660_read_cmos_sensor(0x380f);
	return VTS;
}

int OV3660_set_VTS(int VTS)
{
	// write VTS to registers
	int temp;
	temp = VTS & 0xff;
	OV3660_write_cmos_sensor(0x380f, temp);
	temp = VTS>>8;
	OV3660_write_cmos_sensor(0x380e, temp);
	return 0;
}


int OV3660_get_light_frequency()
{
	// get banding filter value
	int temp, temp1, light_frequency;
	temp = OV3660_read_cmos_sensor(0x3c01);
	if (temp & 0x80) 
	{
		// manual
		temp1 = OV3660_read_cmos_sensor(0x3c00);
		if (temp1 & 0x04) {
		// 50Hz
			light_frequency = 50;
		}
		else {
		// 60Hz
			light_frequency = 60;
		}
	}
	else {
		// auto
		temp1 = OV3660_read_cmos_sensor(0x3c0c);
		if (temp1 & 0x01) {
			// 50Hz
			light_frequency = 50;
		}
		else {
			// 60Hz
			light_frequency = 60;
		}
	}
	return light_frequency;
}

void OV3660_set_bandingfilter()
{
	int preview_VTS;
	int band_step60, max_band60, band_step50, max_band50;
	// read preview PCLK
	preview_sysclk = OV3660_get_sysclk();
	// read preview HTS
	preview_HTS = OV3660_get_HTS();
	// read preview VTS
	preview_VTS = OV3660_get_VTS();
	// calculate banding filter
	// 60Hz
	band_step60 = preview_sysclk * 100/preview_HTS * 100/120;
	OV3660_write_cmos_sensor(0x3a0a, (band_step60 >> 8));
	OV3660_write_cmos_sensor(0x3a0b, (band_step60 & 0xff));
	max_band60 = ((preview_VTS-4)/band_step60);
	OV3660_write_cmos_sensor(0x3a0d, max_band60);
	// 50Hz
	band_step50 = preview_sysclk * 100/preview_HTS;
	OV3660_write_cmos_sensor(0x3a08, (band_step50 >> 8));
	OV3660_write_cmos_sensor(0x3a09, (band_step50 & 0xff));
	max_band50 = ((preview_VTS-4)/band_step50);
	OV3660_write_cmos_sensor(0x3a0e, max_band50);
}
int OV3660_set_AE_target(int target)
{
	// stable in high
	int fast_high, fast_low;
	AE_low = target * 23 / 25; // 0.92
	AE_high = target * 27 / 25; // 1.08
	fast_high = AE_high<<1;
	if(fast_high>255)
	fast_high = 255;
	fast_low = AE_low>>1;
	OV3660_write_cmos_sensor(0x3a0f, AE_high);
	OV3660_write_cmos_sensor(0x3a10, AE_low);
	OV3660_write_cmos_sensor(0x3a1b, AE_high);
	OV3660_write_cmos_sensor(0x3a1e, AE_low);
	OV3660_write_cmos_sensor(0x3a11, fast_high);
	OV3660_write_cmos_sensor(0x3a1f, fast_low);
	return 0;
}

void OV3660_stream_on()
{
	OV3660_write_cmos_sensor(0x4202, 0x00);
}

void OV3660_stream_off()
{
	OV3660_write_cmos_sensor(0x4202, 0x0f);
}

void OV3660_Computer_AECAGC(kal_uint16 preview_clk_in_M, kal_uint16 capture_clk_in_M)
{
    kal_uint16 PV_Line_Width;
    kal_uint16 Capture_Line_Width;
    kal_uint16 Capture_Maximum_Shutter;
    kal_uint16 Capture_Exposure;
    kal_uint16 Capture_Gain16;
    kal_uint32 Capture_Banding_Filter;
    kal_uint32 Gain_Exposure=0;

    PV_Line_Width = OV3660_PV_PERIOD_PIXEL_NUMS + OV3660_PV_Dummy_Pixels;   

    Capture_Line_Width = OV3660_FULL_PERIOD_PIXEL_NUMS + OV3660_Capture_Dummy_Pixels;
    Capture_Maximum_Shutter = OV3660_FULL_EXPOSURE_LIMITATION + OV3660_Capture_Dummy_Lines;

    if (OV3660_Banding_setting == AE_FLICKER_MODE_50HZ)
        Capture_Banding_Filter = (kal_uint32)(capture_clk_in_M*100000/100/(2*Capture_Line_Width));
    else
        Capture_Banding_Filter = (kal_uint32)(capture_clk_in_M*100000/120/(2*Capture_Line_Width) );

    /*   Gain_Exposure = OV3660_PV_Gain16*(OV3660_PV_Shutter+OV3660_PV_Extra_Lines)*PV_Line_Width/g_Preview_PCLK_Frequency/Capture_Line_Width*g_Capture_PCLK_Frequency
    ;*/
    OV3660_PV_Gain16 = OV3660_read_OV3660_gain();
    Gain_Exposure = 1 * OV3660_PV_Gain16;  //For OV3660
    ///////////////////////
    Gain_Exposure *=(OV3660_PV_Shutter+OV3660_PV_Extra_Lines);
    Gain_Exposure *=PV_Line_Width;  //970
    //   Gain_Exposure /=g_Preview_PCLK_Frequency;
    Gain_Exposure /=Capture_Line_Width;//1940
    Gain_Exposure = Gain_Exposure*capture_clk_in_M/preview_clk_in_M;// for clock   

    //redistribute gain and exposure
    if (Gain_Exposure < (kal_uint32)(Capture_Banding_Filter * 16))     // Exposure < 1/100/120
    {
       if(Gain_Exposure<16){//exposure line smaller than 2 lines and gain smaller than 0x08 
            Gain_Exposure = Gain_Exposure*4;     
            Capture_Exposure = 1;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2/4;
        }
        else
        {
            Capture_Exposure = Gain_Exposure /16;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2;
        }
    }
    else 
    {
        if (Gain_Exposure >(kal_uint32)( Capture_Maximum_Shutter * 16)) // Exposure > Capture_Maximum_Shutter
        {
           
            Capture_Exposure = Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter;
            Capture_Gain16 = (Gain_Exposure*2 + 1)/Capture_Exposure/2;
            if (Capture_Gain16 > OV3660_Capture_Max_Gain16) 
            {
                // gain reach maximum, insert extra line
                Capture_Exposure = (kal_uint16)(Gain_Exposure*11 /10 /OV3660_Capture_Max_Gain16);
                
                // Exposure = n/100/120
                Capture_Exposure = Capture_Exposure/Capture_Banding_Filter * Capture_Banding_Filter;
                Capture_Gain16 = ((Gain_Exposure *4)/ Capture_Exposure+3)/4;
            }
        }
        else  // 1/100 < Exposure < Capture_Maximum_Shutter, Exposure = n/100/120
        {
            Capture_Exposure = Gain_Exposure/16/Capture_Banding_Filter;
            Capture_Exposure = Capture_Exposure * Capture_Banding_Filter;
            Capture_Gain16 = (Gain_Exposure*2 +1) / Capture_Exposure/2;
        }
    }
    
    OV3660_Capture_Gain16 = Capture_Gain16;
    OV3660_Capture_Extra_Lines = (Capture_Exposure > Capture_Maximum_Shutter)?
            (Capture_Exposure - Capture_Maximum_Shutter/Capture_Banding_Filter*Capture_Banding_Filter):0;     
    
    OV3660_Capture_Shutter = Capture_Exposure - OV3660_Capture_Extra_Lines;
}




/*************************************************************************
* FUNCTION
*	OV3660_NightMode
*
* DESCRIPTION
*	This function night mode of OV3660.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void OV3660_night_mode(kal_bool enable)
{
	int temp;
   
	if (enable)
	{
		temp = OV3660_read_cmos_sensor(0x3a00);
		temp = temp | 0x04;
		OV3660_write_cmos_sensor(0x3a00, temp);
		OV3660_write_cmos_sensor(0x3a02, 0x09);
		OV3660_write_cmos_sensor(0x3a03, 0x3c);
		OV3660_write_cmos_sensor(0x3a14, 0x09);
		OV3660_write_cmos_sensor(0x3a15, 0x3c);
	}
	else 
	{
		temp = OV3660_read_cmos_sensor(0x3a00);
		temp = temp & 0xfb;
		OV3660_write_cmos_sensor(0x3a00, temp);
		OV3660_write_cmos_sensor(0x3a02, 0x30);
		OV3660_write_cmos_sensor(0x3a03, 0xc0);
		OV3660_write_cmos_sensor(0x3a14, 0x30);
		OV3660_write_cmos_sensor(0x3a15, 0x72);
	}

}	/* OV3660_NightMode */


/* Register setting from capture to preview. */
static void OV3660_set_SVGA_mode(void)
{
	//-----------------------------------
	//From capture to preview
	//-----------------------------------

	//-------------------------------------------------------------------------------
    // PLL MY_OUTPUT clock(fclk)
    // fclk = (0x40 - 0x300E[5:0]) x N x Bit8Div x MCLK / M, where
    //      N = 1, 1.5, 2, 3 for 0x300F[7:6] = 0~3, respectively
    //      M = 1, 1.5, 2, 3 for 0x300F[1:0] = 0~3, respectively
    //      Bit8Div = 1, 1, 4, 5 for 0x300F[5:4] = 0~3, respectively
    // Sys Clk = fclk / Bit8Div / SenDiv
    // Sensor MY_OUTPUT clock(DVP PCLK)
    // DVP PCLK = ISP CLK / DVPDiv, where
    //      ISP CLK =  fclk / Bit8Div / SenDiv / CLKDiv / 2, where
    //          Bit8Div = 1, 1, 4, 5 for 0x300F[5:4] = 0~3, respectively
    //          SenDiv = 1, 2 for 0x3010[4] = 0 or 1 repectively
    //          CLKDiv = (0x3011[5:0] + 1)
    //      DVPDiv = 0x304C[3:0] * (2 ^ 0x304C[4]), if 0x304C[3:0] = 0, use 16 instead
    //
    // Base shutter calculation
    //      60Hz: (1/120) * ISP Clk / QXGA_MODE_WITHOUT_DUMMY_PIXELS
    //      50Hz: (1/100) * ISP Clk / QXGA_MODE_WITHOUT_DUMMY_PIXELS
    //-------------------------------------------------------------------------------

	OV3660_write_cmos_sensor(0x303c,0x11);
	OV3660_write_cmos_sensor(0x303b,0x12);
	OV3660_write_cmos_sensor(0x3824,0x02);
	OV3660_write_cmos_sensor(0x460c,0x22);

	OV3660_write_cmos_sensor(0x5001,0x87);
	OV3660_write_cmos_sensor(0x3800,0x00);
	OV3660_write_cmos_sensor(0x3801,0x00);
	OV3660_write_cmos_sensor(0x3802,0x00);
	OV3660_write_cmos_sensor(0x3803,0x00);
	OV3660_write_cmos_sensor(0x3804,0x08);
	OV3660_write_cmos_sensor(0x3805,0x1f);
	OV3660_write_cmos_sensor(0x3806,0x06);
	OV3660_write_cmos_sensor(0x3807,0x09);//0x07);
	OV3660_write_cmos_sensor(0x3808,0x04);
	OV3660_write_cmos_sensor(0x3809,0x00);
	OV3660_write_cmos_sensor(0x380a,0x03);
	OV3660_write_cmos_sensor(0x380b,0x00);
	OV3660_write_cmos_sensor(0x380c,0x08);
	OV3660_write_cmos_sensor(0x380d,0xfc);
	OV3660_write_cmos_sensor(0x380e,0x03);
	OV3660_write_cmos_sensor(0x380f,0x14);
	OV3660_write_cmos_sensor(0x3810,0x00);
	OV3660_write_cmos_sensor(0x3811,0x08);
	OV3660_write_cmos_sensor(0x3812,0x00);
	OV3660_write_cmos_sensor(0x3813,0x02);
	OV3660_write_cmos_sensor(0x3814,0x31);
	OV3660_write_cmos_sensor(0x3815,0x31);
	
	OV3660_write_cmos_sensor(0x3708,0x66);
	OV3660_write_cmos_sensor(0x3709,0x12);
	OV3660_write_cmos_sensor(0x370c,0x0c);
	OV3660_write_cmos_sensor(0x4520,0x0b);
	OV3660_write_cmos_sensor(0x302c,0x03);
		

    //    OV3660_write_cmos_sensor(0x308d,0x04);				  //reset block sleep enable																											
	//    OV3660_write_cmos_sensor(0x3086,0x03);				  //sleep on																											
	 //   OV3660_write_cmos_sensor(0x3086,0x00);				  // sleep off	
}

static void OV3660_YUV_sensor_initial_setting(void)
{	

	OV3660_write_cmos_sensor(0x3031,0x08);//close sensor ldo
	OV3660_write_cmos_sensor(0x3103,0x13);
	OV3660_write_cmos_sensor(0x3008,0x42);
	OV3660_write_cmos_sensor(0x3017,0xff);
	OV3660_write_cmos_sensor(0x3018,0xff);
	OV3660_write_cmos_sensor(0x302c,0x83);

	OV3660_write_cmos_sensor(0x3032,0x00);
	OV3660_write_cmos_sensor(0x3901,0x13);
	OV3660_write_cmos_sensor(0x3704,0x80);
	OV3660_write_cmos_sensor(0x3717,0x00);
	OV3660_write_cmos_sensor(0x371b,0x60);
	OV3660_write_cmos_sensor(0x370b,0x10);
	OV3660_write_cmos_sensor(0x3624,0x03);
	OV3660_write_cmos_sensor(0x3622,0x80);
	OV3660_write_cmos_sensor(0x3614,0x80);
	OV3660_write_cmos_sensor(0x3630,0x52);
	OV3660_write_cmos_sensor(0x3632,0x07);
	OV3660_write_cmos_sensor(0x3633,0xd2);
	OV3660_write_cmos_sensor(0x3619,0x75);
	OV3660_write_cmos_sensor(0x371c,0x00);
	OV3660_write_cmos_sensor(0x370b,0x12);
	OV3660_write_cmos_sensor(0x3704,0x80);
	OV3660_write_cmos_sensor(0x3600,0x08);
	OV3660_write_cmos_sensor(0x3620,0x43);
	OV3660_write_cmos_sensor(0x3702,0x20);
	OV3660_write_cmos_sensor(0x3739,0x48);
	OV3660_write_cmos_sensor(0x3730,0x20);
	OV3660_write_cmos_sensor(0x370c,0x0c);

	OV3660_write_cmos_sensor(0x3a18,0x00);
	OV3660_write_cmos_sensor(0x3a19,0x80);

	OV3660_write_cmos_sensor(0x3000,0x10);
	OV3660_write_cmos_sensor(0x3004,0xef);

	OV3660_write_cmos_sensor(0x6700,0x05);
	OV3660_write_cmos_sensor(0x6701,0x19);
	OV3660_write_cmos_sensor(0x6702,0xfd);
	OV3660_write_cmos_sensor(0x6703,0xd1);
	OV3660_write_cmos_sensor(0x6704,0xff);
	OV3660_write_cmos_sensor(0x6705,0xff);

	OV3660_write_cmos_sensor(0x3002,0x1c);
	OV3660_write_cmos_sensor(0x3006,0xc3);

	//window
	OV3660_write_cmos_sensor(0x3800,0x00);
	OV3660_write_cmos_sensor(0x3801,0x00);
	OV3660_write_cmos_sensor(0x3802,0x00);
	OV3660_write_cmos_sensor(0x3803,0x00);
	OV3660_write_cmos_sensor(0x3804,0x08);
	OV3660_write_cmos_sensor(0x3805,0x1f);
	OV3660_write_cmos_sensor(0x3806,0x06);
	OV3660_write_cmos_sensor(0x3807,0x07);
	OV3660_write_cmos_sensor(0x3808,0x04);
	OV3660_write_cmos_sensor(0x3809,0x00);
	OV3660_write_cmos_sensor(0x380a,0x03);
	OV3660_write_cmos_sensor(0x380b,0x00);
	OV3660_write_cmos_sensor(0x380c,0x08);
	OV3660_write_cmos_sensor(0x380d,0xfc);
	OV3660_write_cmos_sensor(0x380e,0x03);
	OV3660_write_cmos_sensor(0x380f,0x14);
	OV3660_write_cmos_sensor(0x3810,0x00);
	OV3660_write_cmos_sensor(0x3811,0x08);
	OV3660_write_cmos_sensor(0x3812,0x00);
	OV3660_write_cmos_sensor(0x3813,0x02);

	OV3660_write_cmos_sensor(0x3814,0x31);
	OV3660_write_cmos_sensor(0x3815,0x31);

	OV3660_write_cmos_sensor(0x3820,0x07);
	OV3660_write_cmos_sensor(0x3821,0x01);//;01
	OV3660_write_cmos_sensor(0x3824,0x01);
	OV3660_write_cmos_sensor(0x3826,0x23);
	OV3660_write_cmos_sensor(0x3a02,0x30);
	OV3660_write_cmos_sensor(0x3a03,0xc0);
	OV3660_write_cmos_sensor(0x3a08,0x00);
	OV3660_write_cmos_sensor(0x3a09,0xec);
	OV3660_write_cmos_sensor(0x3a0a,0x00);
	OV3660_write_cmos_sensor(0x3a0b,0xc5);
	OV3660_write_cmos_sensor(0x3a0d,0x04);
	OV3660_write_cmos_sensor(0x3a0e,0x03);
	OV3660_write_cmos_sensor(0x3a14,0x30);
	OV3660_write_cmos_sensor(0x3a15,0x72);
	OV3660_write_cmos_sensor(0x3618,0x00);
	OV3660_write_cmos_sensor(0x3623,0x00);
	OV3660_write_cmos_sensor(0x3708,0x64);
	OV3660_write_cmos_sensor(0x3709,0x52);
	OV3660_write_cmos_sensor(0x370c,0x03);
	OV3660_write_cmos_sensor(0x4300,0x32);//;YUYV
	OV3660_write_cmos_sensor(0x440e,0x09);
	OV3660_write_cmos_sensor(0x4514,0xbb);//;aa//kimi
	OV3660_write_cmos_sensor(0x4520,0xb0);
	OV3660_write_cmos_sensor(0x460b,0x37);
	OV3660_write_cmos_sensor(0x460c,0x20);
	OV3660_write_cmos_sensor(0x4713,0x02);
	OV3660_write_cmos_sensor(0x471c,0xd0);
	OV3660_write_cmos_sensor(0x5086,0x00);
	OV3660_write_cmos_sensor(0x5000,0x07);
	OV3660_write_cmos_sensor(0x5001,0x03);
	OV3660_write_cmos_sensor(0x5002,0x00);
	OV3660_write_cmos_sensor(0x501f,0x00);
	OV3660_write_cmos_sensor(0x3008,0x02);

	OV3660_write_cmos_sensor(0x370c,0x0f); // ;v09
	OV3660_write_cmos_sensor(0x3a00,0x38);
	//20fps
	OV3660_write_cmos_sensor(0x303c,0x11);//20fps, 12,1b 15fps
	OV3660_write_cmos_sensor(0x303b,0x12);
	OV3660_write_cmos_sensor(0x440e,0x08); //;for MBIST


	//IQ setting
	OV3660_write_cmos_sensor(0x5000,0xA7);
	OV3660_write_cmos_sensor(0x3821,0x1 );//flip
	OV3660_write_cmos_sensor(0x4514,0xbb);

	OV3660_write_cmos_sensor(0x4407,0x04);
	                           
	//awb                           
	OV3660_write_cmos_sensor(0x5180,0xff);
	OV3660_write_cmos_sensor(0x5181,0xf0);
	OV3660_write_cmos_sensor(0x5182,0x0 );
	OV3660_write_cmos_sensor(0x5183,0x14);
	OV3660_write_cmos_sensor(0x5184,0x25);
	OV3660_write_cmos_sensor(0x5185,0x24);
	OV3660_write_cmos_sensor(0x5186,0x4 );
	OV3660_write_cmos_sensor(0x5187,0xc );
	OV3660_write_cmos_sensor(0x5188,0x1b);
	OV3660_write_cmos_sensor(0x5189,0x83);
	OV3660_write_cmos_sensor(0x518a,0x63);
	OV3660_write_cmos_sensor(0x518b,0xa8);
	OV3660_write_cmos_sensor(0x518c,0x6 );
	OV3660_write_cmos_sensor(0x518d,0x7f);
	OV3660_write_cmos_sensor(0x518e,0x3a);
	OV3660_write_cmos_sensor(0x518f,0x3c);
	OV3660_write_cmos_sensor(0x5190,0x4c);
	OV3660_write_cmos_sensor(0x5191,0xf0);
	OV3660_write_cmos_sensor(0x5192,0x4 );
	OV3660_write_cmos_sensor(0x5193,0x70);
	OV3660_write_cmos_sensor(0x5194,0xf0);
	OV3660_write_cmos_sensor(0x5195,0xf0);
	OV3660_write_cmos_sensor(0x5196,0x3 );
	OV3660_write_cmos_sensor(0x5197,0x1 );
	OV3660_write_cmos_sensor(0x5198,0x6 );
	OV3660_write_cmos_sensor(0x5199,0x5d);
	OV3660_write_cmos_sensor(0x519a,0x4 );
	OV3660_write_cmos_sensor(0x519b,0x0 );
	OV3660_write_cmos_sensor(0x519c,0x5 );
	OV3660_write_cmos_sensor(0x519d,0x99);
	OV3660_write_cmos_sensor(0x519e,0x38);
	                           
	                           
	//LENC_90 setting for A60 2011-09-27
	//OV3660 LENC setting           
	OV3660_write_cmos_sensor(0x5800,0x20);
	OV3660_write_cmos_sensor(0x5801,0xf );
	OV3660_write_cmos_sensor(0x5802,0xe );
	OV3660_write_cmos_sensor(0x5803,0xd );
	OV3660_write_cmos_sensor(0x5804,0x11);
	OV3660_write_cmos_sensor(0x5805,0x20);
	OV3660_write_cmos_sensor(0x5806,0x9 );
	OV3660_write_cmos_sensor(0x5807,0x6 );
	OV3660_write_cmos_sensor(0x5808,0x5 );
	OV3660_write_cmos_sensor(0x5809,0x6 );
	OV3660_write_cmos_sensor(0x580a,0x7 );
	OV3660_write_cmos_sensor(0x580b,0xc );
	OV3660_write_cmos_sensor(0x580c,0x7 );
	OV3660_write_cmos_sensor(0x580d,0x2 );
	OV3660_write_cmos_sensor(0x580e,0x1 );
	OV3660_write_cmos_sensor(0x580f,0x1 );
	OV3660_write_cmos_sensor(0x5810,0x4 );
	OV3660_write_cmos_sensor(0x5811,0x8 );
	OV3660_write_cmos_sensor(0x5812,0x7 );
	OV3660_write_cmos_sensor(0x5813,0x2 );
	OV3660_write_cmos_sensor(0x5814,0x1 );
	OV3660_write_cmos_sensor(0x5815,0x1 );
	OV3660_write_cmos_sensor(0x5816,0x3 );
	OV3660_write_cmos_sensor(0x5817,0x9 );
	OV3660_write_cmos_sensor(0x5818,0x9 );
	OV3660_write_cmos_sensor(0x5819,0x6 );
	OV3660_write_cmos_sensor(0x581a,0x4 );
	OV3660_write_cmos_sensor(0x581b,0x4 );
	OV3660_write_cmos_sensor(0x581c,0x7 );
	OV3660_write_cmos_sensor(0x581d,0xb );
	OV3660_write_cmos_sensor(0x581e,0x1a);
	OV3660_write_cmos_sensor(0x581f,0xd );
	OV3660_write_cmos_sensor(0x5820,0xb );
	OV3660_write_cmos_sensor(0x5821,0xc );
	OV3660_write_cmos_sensor(0x5822,0xe );
	OV3660_write_cmos_sensor(0x5823,0x19);
	OV3660_write_cmos_sensor(0x5824,0x45);
	OV3660_write_cmos_sensor(0x5825,0x29);
	OV3660_write_cmos_sensor(0x5826,0xb );
	OV3660_write_cmos_sensor(0x5827,0x19);
	OV3660_write_cmos_sensor(0x5828,0x35);
	OV3660_write_cmos_sensor(0x5829,0x3a);
	OV3660_write_cmos_sensor(0x582a,0x28);
	OV3660_write_cmos_sensor(0x582b,0x36);
	OV3660_write_cmos_sensor(0x582c,0x28);
	OV3660_write_cmos_sensor(0x582d,0x1b);
	OV3660_write_cmos_sensor(0x582e,0x29);
	OV3660_write_cmos_sensor(0x582f,0x43);
	OV3660_write_cmos_sensor(0x5830,0x52);
	OV3660_write_cmos_sensor(0x5831,0x33);
	OV3660_write_cmos_sensor(0x5832,0x18);
	OV3660_write_cmos_sensor(0x5833,0x2a);
	OV3660_write_cmos_sensor(0x5834,0x37);
	OV3660_write_cmos_sensor(0x5835,0x35);
	OV3660_write_cmos_sensor(0x5836,0x27);
	OV3660_write_cmos_sensor(0x5837,0x2a);
	OV3660_write_cmos_sensor(0x5838,0x46);
	OV3660_write_cmos_sensor(0x5839,0x1a);
	OV3660_write_cmos_sensor(0x583a,0xb );
	OV3660_write_cmos_sensor(0x583b,0x1a);
	OV3660_write_cmos_sensor(0x583c,0x36);
	OV3660_write_cmos_sensor(0x583d,0xce);
	                           
	//cmx                           
	OV3660_write_cmos_sensor(0x5381,0x25);
	OV3660_write_cmos_sensor(0x5382,0x56);
	OV3660_write_cmos_sensor(0x5383,0x5 );
	OV3660_write_cmos_sensor(0x5384,0xa );
	OV3660_write_cmos_sensor(0x5385,0x80);
	OV3660_write_cmos_sensor(0x5386,0x8a);
	OV3660_write_cmos_sensor(0x5387,0x80);
	OV3660_write_cmos_sensor(0x5388,0x6b);
	OV3660_write_cmos_sensor(0x5389,0x15);
	OV3660_write_cmos_sensor(0x538b,0x98);
	OV3660_write_cmos_sensor(0x538a,0x1 );
	                           
	//gamma                         
	OV3660_write_cmos_sensor(0x5490,0x11);
	OV3660_write_cmos_sensor(0x5481,0xa );
	OV3660_write_cmos_sensor(0x5482,0x18 );
	OV3660_write_cmos_sensor(0x5483,0x38);
	OV3660_write_cmos_sensor(0x5484,0x69);
	OV3660_write_cmos_sensor(0x5485,0x7e);
	OV3660_write_cmos_sensor(0x5486,0x8e);
	OV3660_write_cmos_sensor(0x5487,0x99);
	OV3660_write_cmos_sensor(0x5488,0xa8);
	OV3660_write_cmos_sensor(0x5489,0xb1);
	OV3660_write_cmos_sensor(0x548a,0xbb);
	OV3660_write_cmos_sensor(0x548b,0xc4);
	OV3660_write_cmos_sensor(0x548c,0xcd);
	OV3660_write_cmos_sensor(0x548d,0xdb);
	OV3660_write_cmos_sensor(0x548e,0xe6);
	OV3660_write_cmos_sensor(0x548f,0xf3);
	                           
	//ev                            
	OV3660_write_cmos_sensor(0x3a0f,0x28);
	OV3660_write_cmos_sensor(0x3a10,0x20);
	OV3660_write_cmos_sensor(0x3a1b,0x28);
	OV3660_write_cmos_sensor(0x3a1e,0x20);
	                           
	                           
	//CIP                            
	OV3660_write_cmos_sensor(0x5300,0x0c);// 2x = real gain
	OV3660_write_cmos_sensor(0x5301,0x20);
	OV3660_write_cmos_sensor(0x5302,0x10);
	OV3660_write_cmos_sensor(0x5303,0x0 );
	OV3660_write_cmos_sensor(0x5304,0xc );
	OV3660_write_cmos_sensor(0x5305,0x20);
	OV3660_write_cmos_sensor(0x5306,0xc );
	OV3660_write_cmos_sensor(0x5307,0x20);
	OV3660_write_cmos_sensor(0x5309,0x0c);
	OV3660_write_cmos_sensor(0x530a,0x20);
	OV3660_write_cmos_sensor(0x530b,0x0 );
	OV3660_write_cmos_sensor(0x530c,0x10);
	                           
	                           
	//UV adjust                    
	OV3660_write_cmos_sensor(0x5001,0x87);
	OV3660_write_cmos_sensor(0x5580,0x2 );
	OV3660_write_cmos_sensor(0x5583,0x40);
	OV3660_write_cmos_sensor(0x5584,0x38);
	OV3660_write_cmos_sensor(0x5589,0x30);
	OV3660_write_cmos_sensor(0x558a,0x0);
	OV3660_write_cmos_sensor(0x558b,0x78);
	                           
	                           
	//dns                           
	OV3660_write_cmos_sensor(0x5308,0x65);
	OV3660_write_cmos_sensor(0x5304,0x08);
	OV3660_write_cmos_sensor(0x5305,0x30);
	OV3660_write_cmos_sensor(0x5306,0x08);
	OV3660_write_cmos_sensor(0x5307,0x50);
	                           
	//Sharpness Auto               
	OV3660_write_cmos_sensor(0x5308,0x00);
	OV3660_write_cmos_sensor(0x5300,0x08);
	OV3660_write_cmos_sensor(0x5301,0x18);
	OV3660_write_cmos_sensor(0x5302,0x18);
	OV3660_write_cmos_sensor(0x5303,0x00);
	OV3660_write_cmos_sensor(0x5309,0x10);
	OV3660_write_cmos_sensor(0x530a,0x18);
	OV3660_write_cmos_sensor(0x530b,0x04);
	OV3660_write_cmos_sensor(0x530c,0x18);
	                           
	//banding                       
	OV3660_write_cmos_sensor(0x3a00,0x38);
	OV3660_write_cmos_sensor(0x3a08,0x00);
	OV3660_write_cmos_sensor(0x3a09,0xEA);

	OV3660_write_cmos_sensor(0x4002,0xc5);
	OV3660_write_cmos_sensor(0x4005,0x12);

} /* OV3660_YUV_sensor_initial_setting */

static void OV3660_set_AE_mode(kal_bool AE_enable)
{
	kal_uint8 temp_AE_reg = 0;
	
	if (AE_enable == KAL_TRUE)
	{
		// turn on AEC/AGC
		temp_AE_reg = OV3660_read_cmos_sensor(0x3503);
		OV3660_write_cmos_sensor(0x3503, temp_AE_reg| 0x03);
	}
	else
	{
		// turn off AEC/AGC
		temp_AE_reg = OV3660_read_cmos_sensor(0x3503);
		OV3660_write_cmos_sensor(0x3503, temp_AE_reg&~0x03);
	}
}



/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	OV3660Open
*
* DESCRIPTION
*	This function initialize the registers of CMOS sensor
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 OV3660Open(void)
{
    volatile signed char i;
    kal_uint16 sensor_id=0;
       sensor_id_fail = 0; 

    //s_move to here from CISModulePowerOn()
    
    OV3660_write_cmos_sensor(0x3008,0x82);// Reset sensor
    Sleep(10);

     zoom_factor = 0; 
     //  Read sensor ID to adjust I2C is OK?
    i = 3; 
    while (i > 0)
    {
        sensor_id = (OV3660_read_cmos_sensor(0x300A) << 8) | OV3660_read_cmos_sensor(0x300B);
        if (sensor_id == OV3660_SENSOR_ID)
        {
            break;
        }
        i --; 
    }
        printk("[OV3660YUV]: Sensor ID:0x%x\n", sensor_id); 
    if(sensor_id != OV3660_SENSOR_ID)
    {
        printk("[OV3660YUV]:Read Sensor ID fail:0x%x\n", sensor_id); 	    
        sensor_id_fail = 1; 
        return ERROR_SENSOR_CONNECT_FAIL;
    }

    /*9. Apply sensor initail setting*/
     OV3660_YUV_sensor_initial_setting();
     
    return ERROR_NONE;
}	/* OV3660Open() */


UINT32 OV3660GetSensorID(UINT32 *sensorID) 
{
	volatile signed char i;

	
	OV3660_write_cmos_sensor(0x3008,0x82);// Reset sensor
	Sleep(10);

	 //  Read sensor ID to adjust I2C is OK?
	i = 3; 
	while (i > 0)
	{
		*sensorID = (OV3660_read_cmos_sensor(0x300A) << 8) | OV3660_read_cmos_sensor(0x300B);
		if (*sensorID == OV3660_SENSOR_ID)
		{
			break;
		}
		printk("[OV3640YUV]:Error Sensor ID:0x%x\n", *sensorID); 
		i --; 
	}

	if(*sensorID != OV3660_SENSOR_ID)
	{
		printk("[OV3640YUV]:Read Sensor ID fail:0x%x\n", *sensorID);		
		return ERROR_SENSOR_CONNECT_FAIL;
	}

    return ERROR_NONE;
}

/*************************************************************************
* FUNCTION
*	OV3660Close
*
* DESCRIPTION
*	This function is to turn off sensor module power.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 OV3660Close(void)
{
//	CISModulePowerOn(FALSE);

	return ERROR_NONE;
}	/* OV3660Close() */


static kal_uint16 OV3660_Reg2Gain(const kal_uint8 iReg)
{
    kal_uint8 iI;
    kal_uint16 iGain = BASEGAIN;    // 1x-gain base

    // Range: 1x to 32x
    // Gain = (GAIN[7] + 1) * (GAIN[6] + 1) * (GAIN[5] + 1) * (GAIN[4] + 1) * (1 + GAIN[3:0] / 16)
    for (iI = 7; iI >= 4; iI--) {
        iGain *= (((iReg >> iI) & 0x01) + 1);
    }

    return iGain +  iGain * (iReg & 0x0F) / 16;
}

static kal_uint8 OV3660_Gain2Reg(const kal_uint16 iGain)
{
    kal_uint8 iReg = 0x00;

    if (iGain < 2 * BASEGAIN) {
        // Gain = 1 + GAIN[3:0](0x00) / 16
        iReg = 16 * (iGain - BASEGAIN) / BASEGAIN;
    }else if (iGain < 4 * BASEGAIN) {
        // Gain = 2 * (1 + GAIN[3:0](0x00) / 16)
        iReg |= 0x10;
        iReg |= 8 * (iGain - 2 * BASEGAIN) / BASEGAIN;
    }else if (iGain < 8 * BASEGAIN) {
        // Gain = 4 * (1 + GAIN[3:0](0x00) / 16)
        iReg |= 0x30;
        iReg |= 4 * (iGain - 4 * BASEGAIN) / BASEGAIN;
    }else if (iGain < 16 * BASEGAIN) {
        // Gain = 8 * (1 + GAIN[3:0](0x00) / 16)
        iReg |= 0x70;
        iReg |= 2 * (iGain - 8 * BASEGAIN) / BASEGAIN;
    }else if (iGain < 32 * BASEGAIN) {
        // Gain = 16 * (1 + GAIN[3:0](0x00) / 16)
        iReg |= 0xF0;
        iReg |= (iGain - 16 * BASEGAIN) / BASEGAIN;
    }else {
        ASSERT(0);
    }

    return iReg;
}

kal_uint16 OV3660_write_gain(kal_uint16 gain)
{
    kal_uint16 temp_reg;
   
	RETAILMSG(1, (TEXT("OV3660 write gain: %d\r\n"), gain));
   
   if(gain > 248)  return ;//ASSERT(0);
   
    temp_reg = 0;
    if (gain > 31)
    {
        temp_reg |= 0x10;
        gain = gain >> 1;
    }
    if (gain > 31)
    {
        temp_reg |= 0x20;
        gain = gain >> 1;
    }

    if (gain > 31)
    {
        temp_reg |= 0x40;
        gain = gain >> 1;
    }
    if (gain > 31)
    {
        temp_reg |= 0x80;
        gain = gain >> 1;
    }
    
    if (gain > 16)
    {
        temp_reg |= ((gain -16) & 0x0f);
    }   
  
   OV3660_write_cmos_sensor(0x3001,temp_reg);


}

kal_uint16 OV3660_read_gain(void)
{
    kal_uint8  temp_reg;
    kal_uint16 sensor_gain;

    temp_reg=OV3660_read_cmos_sensor(0x3001);  


    sensor_gain=(16+(temp_reg&0x0F));
    if(temp_reg&0x10)
        sensor_gain<<=1;
    if(temp_reg&0x20)
        sensor_gain<<=1;
      
    if(temp_reg&0x40)
        sensor_gain<<=1;
      
    if(temp_reg&0x80)
        sensor_gain<<=1;
      
    return sensor_gain;
	
}  /* read_OV3660_gain */



static void OV3660_set_mirror_flip(kal_uint8 image_mirror, kal_uint16 *iStartX, kal_uint16 *iStartY)
{
	kal_uint8 iTemp = 0;
	kal_uint8 iTemp2 = 0;
        printk("[OV3660YUV]:image_mirror:0x%x\n", image_mirror); 
    switch (image_mirror) {
    case IMAGE_NORMAL:
	OV3660_write_cmos_sensor(0x3820, 0x07);
	OV3660_write_cmos_sensor(0x3821, 0x01);
	OV3660_write_cmos_sensor(0x4514, 0xbb);		// To mirror the Array, or there is horizontal lines when turn on mirror.
        break;
	case IMAGE_H_MIRROR:
	OV3660_write_cmos_sensor(0x3820, 0x01);
	OV3660_write_cmos_sensor(0x3821, 0x07);
	OV3660_write_cmos_sensor(0x4514, 0xbb);		// To mirror the Array, or there is horizontal lines when turn on mirror.
        break;
	case IMAGE_V_MIRROR:	//Flip Register 0x04[6] and 0x04[4] (FF = 01)
	OV3660_write_cmos_sensor(0x3820, 0x01);
	OV3660_write_cmos_sensor(0x3821, 0x01);
	OV3660_write_cmos_sensor(0x4514, 0xaa);		// To mirror the Array, or there is horizontal lines when turn on mirror.
        break;

    case IMAGE_HV_MIRROR:
	OV3660_write_cmos_sensor(0x3820, 0x07);
	OV3660_write_cmos_sensor(0x3821, 0x07);
	OV3660_write_cmos_sensor(0x4514, 0xaa);		// To mirror the Array, or there is horizontal lines when turn on mirror.
        break;

    default:
        ASSERT(0);
    }
}

/*************************************************************************
* FUNCTION
*	OV3660Preview
*
* DESCRIPTION
*	This function start the sensor preview.
*
* PARAMETERS
*	*image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 OV3660Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	//kal_uint8 iTemp = 0;
	kal_uint16 iStartX = 0, iStartY = 0;
	kal_uint16 	shutter = 0,pv_gain = 0;


	OV3660_sensor_cap_state = KAL_FALSE;
	OV3660_gPVmode = KAL_TRUE;

	OV3660_stream_off();

	/*Step1. set output size*/
	OV3660_set_SVGA_mode();

	OV3660_VEDIO_encode_mode = KAL_FALSE;
	OV3660_VEDIO_MPEG4 = KAL_FALSE;
	//4  <2> if preview of capture PICTURE
	OV3660_g_iOV3660_Mode = OV3660_MODE_PREVIEW;
	OV3660_PV_dummy_pixels = 1;
	OV3660_PV_dummy_lines = 0;	

		//Set preview pixel clock freqency and pixel clock division.
	//	OV3660_g_Preview_PCLK_Frequency = ;		
	//	OV3660_g_Preview_PCLK_Division = 0;	



	//Step 3. record preview ISP_clk
	OV3660_preview_pclk = 563;//56330000;   //22500000;

	//4 <3> set mirror and flip
	OV3660_set_mirror_flip(sensor_config_data->SensorImageMirror, &iStartX, &iStartY);


	// set banding filter
	OV3660_set_bandingfilter();
	// set ae_target
	OV3660_set_AE_target(AE_Target);
	// update night mode setting
	OV3660_night_mode(m_iCombo_NightMode);
	OV3660_stream_on();


	image_window->GrabStartX = IMAGE_SENSOR_START_GRAB_X;    
	image_window->GrabStartY = IMAGE_SENSOR_START_GRAB_Y;       
	image_window->ExposureWindowWidth = OV3660_IMAGE_SENSOR_PV_WIDTH;
	image_window->ExposureWindowHeight = OV3660_IMAGE_SENSOR_PV_HEIGHT;

	//debug 
	//shutter = OV3660_read_shutter();
	//pv_gain = OV3660_read_gain();

	//Enable Color bar   

	return TRUE; 
}	/* OV3660_Preview */

UINT32 OV3660Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window, MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	int preview_shutter, preview_gain16;
	int capture_shutter, capture_gain16, capture_sysclk, capture_HTS, capture_VTS;
	int light_frequency, capture_bandingfilter, capture_max_band;
	long capture_gain16_shutter, average;
	int i;
	int tmp3808,tmp3809;
// set OV3660 to capture mode
	int regCapture[] =
	{
		// YUV Capture
		// 2048 x 1536 5fps
		// 24 MHz input clock, 36Mhz PCLK
		0x3002, 0x1c,
		0x3006, 0xc3,
		0x3800, 0x00, //HS
		0x3801, 0x00, //HS
		0x3802, 0x00, //VS
		0x3803, 0x00, //VS
		0x3804, 0x08, //HW=2080
		0x3805, 0x1f, //HW
		0x3806, 0x06, //VH=1544
		0x3807, 0x0b, //VH
		0x3808, 0x08, //DVPHO=2048
		0x3809, 0x00, //DVPHO
		0x380a, 0x06, //DVPVO=1536
		0x380b, 0x00, //DVPVO
		0x380c, 0x08, //HTS=2300
		0x380d, 0xfc, //HTS
		0x380e, 0x06, //VTS=1564
		0x380f, 0x1c, //VTS
		0x3810, 0x00, //HOFFSET
		0x3811, 0x10, //HOFFSET
		0x3812, 0x00, //VOFFSET
		0x3813, 0x06, //VOFFSET
		0x3814, 0x11, //X INC
		0x3815, 0x11,
		0x3820, 0x46,
		0x3821, 0x00,
		0x3824, 0x01,
		0x3826, 0x23,
		//Y INC
		//FLIP
		//MIRROR
		//PCLK RATIO
		0x3618, 0x78,
		0x3708, 0x21,
		0x3709, 0x12,
		0x4300, 0x32, //YUV422,YUYV
		0x440e, 0x09,
		0x4520, 0xb0,
		0x460b, 0x37,
		0x460c, 0x20,
		0x4713, 0x02,
		0x471c, 0xd0,
		0x303b, 0x19, //5fps
		0x303c, 0x12, //5fps
		0x3821, 0x00, //mirror on
		0x4514, 0x88,
		0x3503, 0x03,
	// AEC/AGC off, delay gain 1 frame
	};

	tmp3808 = OV3660_read_cmos_sensor(0x3808);
	
	// read preview shutter
	preview_shutter = OV3660_read_shutter();
	// read preview gain
	preview_gain16 = OV3660_read_OV3660_gain();
	if(tmp3808!=0x08)
	{
		OV3660_PV_Shutter = preview_shutter;
		OV3660_PV_Gain16 = preview_gain16;
	}
	  
	// get average
	average = OV3660_read_cmos_sensor(0x56a1);
	 
	// turn off night mode for capture
	OV3660_night_mode(0);
	OV3660_stream_off();
	
	// Write capture table
	for (i=0; i<sizeof(regCapture)/sizeof(int); i+=2)
	{
		OV3660_write_cmos_sensor(regCapture[i], regCapture[i+1]);
	}
	
	// read capture PCLK
	capture_sysclk = OV3660_get_sysclk();
	// read capture HTS
	capture_HTS = OV3660_get_HTS();
	// read capture VTS
	capture_VTS = OV3660_get_VTS();
	  
	// calculate capture banding filter
	light_frequency = OV3660_get_light_frequency();
	
	if (light_frequency == 60) {
	// 60Hz
		capture_bandingfilter = capture_sysclk * 100 / capture_HTS * 100 / 120;
	}
	else {
	// 50Hz
		capture_bandingfilter = capture_sysclk * 100 / capture_HTS;
	}
	
	capture_max_band = ((capture_VTS-4)/capture_bandingfilter);
	
	// gain to shutter
	if (average > AE_low && average < AE_high) {
		// in stable range, adjust to target
		capture_gain16_shutter = OV3660_PV_Gain16 * OV3660_PV_Shutter * capture_sysclk/preview_sysclk *
		preview_HTS/capture_HTS * AE_Target / average;
	}
	else {
		// not in stable range
		capture_gain16_shutter = OV3660_PV_Gain16 * OV3660_PV_Shutter * capture_sysclk/preview_sysclk *
		preview_HTS/capture_HTS;
	}
	
	if(capture_gain16_shutter < (capture_bandingfilter * 16))
	{
		// shutter < 1/100
		capture_shutter = capture_gain16_shutter/16;
		if(capture_shutter<1)
		capture_shutter = 1;
		capture_gain16 = capture_gain16_shutter/capture_shutter;
		if(capture_gain16<16)
		capture_gain16 = 16;
	}
	else
	{
		if(capture_gain16_shutter > (capture_bandingfilter*capture_max_band*16))
		{
			// exposure reach max
			capture_shutter = capture_bandingfilter*capture_max_band;
			capture_gain16 = capture_gain16_shutter/ capture_shutter;
		}
		else 
		{
			// 1/100 < capture_shutter < max, capture_shutter = n/100
			capture_shutter = ( (capture_gain16_shutter/16/capture_bandingfilter)) * capture_bandingfilter;
			capture_gain16 = capture_gain16_shutter/ capture_shutter;
		}
	}
	
	// write capture gain
	OV3660_write_OV3660_gain(capture_gain16);
	
	// write capture shutter
	if (capture_shutter > (capture_VTS - 4)) {
		capture_VTS = capture_shutter + 4;
		OV3660_set_VTS(capture_VTS);
	}
	
	OV3660_write_shutter(capture_shutter);
	OV3660_stream_on();
	// skip 2 vysnc
	// start capture at 3rd vsync
	return TRUE; 

}	/* OV3660_Capture */



UINT32 OV3660GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	pSensorResolution->SensorFullWidth=OV3660_IMAGE_SENSOR_FULL_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;  //modify by yanxu
	pSensorResolution->SensorFullHeight=OV3660_IMAGE_SENSOR_FULL_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;
	pSensorResolution->SensorPreviewWidth=OV3660_IMAGE_SENSOR_PV_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;
	pSensorResolution->SensorPreviewHeight=OV3660_IMAGE_SENSOR_PV_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;

	return ERROR_NONE;
}	/* OV3660GetResolution() */

UINT32 OV3660GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	pSensorInfo->SensorPreviewResolutionX=OV3660_IMAGE_SENSOR_PV_WIDTH;
	pSensorInfo->SensorPreviewResolutionY=OV3660_IMAGE_SENSOR_PV_HEIGHT;
	pSensorInfo->SensorFullResolutionX=OV3660_IMAGE_SENSOR_FULL_WIDTH;
	pSensorInfo->SensorFullResolutionY=OV3660_IMAGE_SENSOR_FULL_HEIGHT;

	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=1;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	/*??? */
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;

	pSensorInfo->CaptureDelayFrame = 3; 
	pSensorInfo->PreviewDelayFrame = 3; 
	pSensorInfo->VideoDelayFrame = 20; 
	pSensorInfo->SensorMasterClockSwitch = 0; 
       pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   		
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
                     pSensorInfo->SensorGrabStartX = IMAGE_SENSOR_START_GRAB_X; 
                     pSensorInfo->SensorGrabStartY = IMAGE_SENSOR_START_GRAB_Y;     			
			
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
                     pSensorInfo->SensorGrabStartX = IMAGE_FULL_GRAB_START_X; 
                     pSensorInfo->SensorGrabStartY = IMAGE_FULL_GRAB_START_Y;     			
		break;
		default:
			pSensorInfo->SensorClockFreq=26;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
                     pSensorInfo->SensorGrabStartX = 0; 
                     pSensorInfo->SensorGrabStartY = 0;     			
		break;
	}
	OV3660_PixelClockDivider=pSensorInfo->SensorPixelClockCount;
	memcpy(pSensorConfigData, &OV3660SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* OV3660GetInfo() */


UINT32 OV3660Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			OV3660Preview(pImageWindow, pSensorConfigData);
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
			OV3660Capture(pImageWindow, pSensorConfigData);
		break;
		default:
		    break; 
	}
	return TRUE;
}	/* OV3660Control() */

/* [TC] YUV sensor */	

/*************************************************************************
* FUNCTION
*	OV3660_set_param_wb
*
* DESCRIPTION
*	wb setting.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
BOOL OV3660_set_param_wb(UINT16 para)
{
	
    //This sensor need more time to balance AWB, 
    //we suggest higher fps or drop some frame to avoid garbage color when preview initial
    kal_uint16 temp=OV3660_read_cmos_sensor(0x3406);
	switch (para)
	{
	        case AWB_MODE_OFF:
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x00);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);
	            break;             
		case AWB_MODE_AUTO:
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x00);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);

		    break;
		case AWB_MODE_CLOUDY_DAYLIGHT:
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x01);
			OV3660_write_cmos_sensor(0x3400, 0x07);
			OV3660_write_cmos_sensor(0x3401, 0xff);
			OV3660_write_cmos_sensor(0x3402, 0x04);
			OV3660_write_cmos_sensor(0x3403, 0x00);
			OV3660_write_cmos_sensor(0x3404, 0x04);
			OV3660_write_cmos_sensor(0x3405, 0x00);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);
		    break;
		case AWB_MODE_DAYLIGHT:
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x01);
			OV3660_write_cmos_sensor(0x3400, 0x07);
			OV3660_write_cmos_sensor(0x3401, 0x29);
			OV3660_write_cmos_sensor(0x3402, 0x04);
			OV3660_write_cmos_sensor(0x3403, 0x00);
			OV3660_write_cmos_sensor(0x3404, 0x04);
			OV3660_write_cmos_sensor(0x3405, 0x5d);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);
		    break;
		case AWB_MODE_INCANDESCENT:	
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x01);
			OV3660_write_cmos_sensor(0x3400, 0x04);
			OV3660_write_cmos_sensor(0x3401, 0xc5);
			OV3660_write_cmos_sensor(0x3402, 0x04);
			OV3660_write_cmos_sensor(0x3403, 0x00);
			OV3660_write_cmos_sensor(0x3404, 0x08);
			OV3660_write_cmos_sensor(0x3405, 0x4d);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);

		    break;  
		case AWB_MODE_FLUORESCENT:
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x01);
			OV3660_write_cmos_sensor(0x3400, 0x06);
			OV3660_write_cmos_sensor(0x3401, 0x57);
			OV3660_write_cmos_sensor(0x3402, 0x04);
			OV3660_write_cmos_sensor(0x3403, 0x00);
			OV3660_write_cmos_sensor(0x3404, 0x07);
			OV3660_write_cmos_sensor(0x3405, 0x6f);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);
		    break;  
		case AWB_MODE_TUNGSTEN:
			OV3660_write_cmos_sensor(0x3212, 0x03);
			OV3660_write_cmos_sensor(0x3406, 0x01);
			OV3660_write_cmos_sensor(0x3400, 0x04);
			OV3660_write_cmos_sensor(0x3401, 0xc5);
			OV3660_write_cmos_sensor(0x3402, 0x04);
			OV3660_write_cmos_sensor(0x3403, 0x00);
			OV3660_write_cmos_sensor(0x3404, 0x08);
			OV3660_write_cmos_sensor(0x3405, 0x4d);
			OV3660_write_cmos_sensor(0x3212, 0x13);
			OV3660_write_cmos_sensor(0x3212, 0xa3);
		    break;
		default:
			return FALSE;
	}

	return TRUE;
	
} /* OV3660_set_param_wb */

/*************************************************************************
* FUNCTION
*	OV3660_set_param_effect
*
* DESCRIPTION
*	effect setting.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
BOOL OV3660_set_param_effect(UINT16 para)
{
	kal_uint32  ret = TRUE;
	int temp5001,temp5580;
	OV3660_write_cmos_sensor(0x5580, 0x02);
	//temp5580 = OV3660_read_cmos_sensor(0x5580);
	switch (para)
	{
		case MEFFECT_OFF:
			OV3660_write_cmos_sensor(0x5583, 0x40);
			OV3660_write_cmos_sensor(0x5584, 0x38);
	              break;
		case MEFFECT_SEPIA:
			OV3660_write_cmos_sensor(0x5580, 0x18);
			OV3660_write_cmos_sensor(0x5583, 0x40);
			OV3660_write_cmos_sensor(0x5584, 0xa0);
			break;  
		case MEFFECT_NEGATIVE:
			OV3660_write_cmos_sensor(0x5580, 0x40);
			break; 
		case MEFFECT_SEPIAGREEN:		
			OV3660_write_cmos_sensor(0x5580,0x18);
			OV3660_write_cmos_sensor(0x5583, 0x60);
			OV3660_write_cmos_sensor(0x5584, 0x60);

			break;
		case MEFFECT_SEPIABLUE:
			OV3660_write_cmos_sensor(0x5580, 0x18);
			OV3660_write_cmos_sensor(0x5583, 0xa0);
			OV3660_write_cmos_sensor(0x5584, 0x40);
			break;        
		case MEFFECT_MONO:			
			OV3660_write_cmos_sensor(0x5580,0x20);
			break;
		default:
			ret = FALSE;
	}

	return ret;

} /* OV3660_set_param_effect */

/*************************************************************************
* FUNCTION
*	OV3660_set_param_banding
*
* DESCRIPTION
*	banding setting.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
BOOL OV3660_set_param_banding(UINT16 para)
{
	int temp;

	switch (para)
	{
		case AE_FLICKER_MODE_50HZ:
			OV3660_write_cmos_sensor(0x3c00, 0x04);
			OV3660_write_cmos_sensor(0x3c01, 0x80);
			temp = OV3660_read_cmos_sensor(0x3a00);
			temp = temp | 0x20;
			OV3660_write_cmos_sensor(0x3a00, temp);

			break;

		case AE_FLICKER_MODE_60HZ:
		
			OV3660_write_cmos_sensor(0x3c00, 0x00);
			OV3660_write_cmos_sensor(0x3c01, 0x80);
			temp = OV3660_read_cmos_sensor(0x3a00);
			temp = temp | 0x20;
			OV3660_write_cmos_sensor(0x3a00, temp);
			break;

	     default:
	          return KAL_FALSE;
	}

	return KAL_TRUE;
} /* OV3660_set_param_banding */




/*************************************************************************
* FUNCTION
*	OV3660_set_param_exposure
*
* DESCRIPTION
*	exposure setting.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
BOOL OV3660_set_param_exposure(UINT16 para)
{

   kal_uint8  temp_reg;
	switch (para)
	{
		case AE_EV_COMP_13:
			OV3660_write_cmos_sensor(0x3a0f, 0x60); 
			OV3660_write_cmos_sensor(0x3a10, 0x58); 
			OV3660_write_cmos_sensor(0x3a1b, 0x60); 
			OV3660_write_cmos_sensor(0x3a1e, 0x58); 
			OV3660_write_cmos_sensor(0x3a11, 0xc0); 
			OV3660_write_cmos_sensor(0x3a1f, 0x30); 
			break;  
		case AE_EV_COMP_10:
			OV3660_write_cmos_sensor(0x3a0f, 0x58); 
			OV3660_write_cmos_sensor(0x3a10, 0x50); 
			OV3660_write_cmos_sensor(0x3a1b, 0x58); 
			OV3660_write_cmos_sensor(0x3a1e, 0x50); 
			OV3660_write_cmos_sensor(0x3a11, 0xb0); 
			OV3660_write_cmos_sensor(0x3a1f, 0x28); 
			break;    
		case AE_EV_COMP_07:
			OV3660_write_cmos_sensor(0x3a0f, 0x50);
			OV3660_write_cmos_sensor(0x3a10, 0x48);
			OV3660_write_cmos_sensor(0x3a1b, 0x50);
			OV3660_write_cmos_sensor(0x3a1e, 0x48);
			OV3660_write_cmos_sensor(0x3a11, 0xa0);
			OV3660_write_cmos_sensor(0x3a1f, 0x24);
			break;    
		case AE_EV_COMP_03:			
			OV3660_write_cmos_sensor(0x3a0f, 0x40);
			OV3660_write_cmos_sensor(0x3a10, 0x38);
			OV3660_write_cmos_sensor(0x3a1b, 0x40);
			OV3660_write_cmos_sensor(0x3a1e, 0x38);
			OV3660_write_cmos_sensor(0x3a11, 0x80);
			OV3660_write_cmos_sensor(0x3a1f, 0x1c);
			break;    
		case AE_EV_COMP_00:
			OV3660_write_cmos_sensor(0x3a0f, 0x28);
			OV3660_write_cmos_sensor(0x3a10, 0x20);
			OV3660_write_cmos_sensor(0x3a1b, 0x28);
			OV3660_write_cmos_sensor(0x3a1e, 0x20);
			OV3660_write_cmos_sensor(0x3a11, 0x70);
			OV3660_write_cmos_sensor(0x3a1f, 0x14);
			break;    
		case AE_EV_COMP_n03:
			OV3660_write_cmos_sensor(0x3a0f, 0x28);
			OV3660_write_cmos_sensor(0x3a10, 0x20);
			OV3660_write_cmos_sensor(0x3a1b, 0x28);
			OV3660_write_cmos_sensor(0x3a1e, 0x20);
			OV3660_write_cmos_sensor(0x3a11, 0x50);
			OV3660_write_cmos_sensor(0x3a1f, 0x10);
			break;    
		case AE_EV_COMP_n07:			
			OV3660_write_cmos_sensor(0x3a0f, 0x20);
			OV3660_write_cmos_sensor(0x3a10, 0x18);
			OV3660_write_cmos_sensor(0x3a1b, 0x20);
			OV3660_write_cmos_sensor(0x3a1e, 0x18);
			OV3660_write_cmos_sensor(0x3a11, 0x40);
			OV3660_write_cmos_sensor(0x3a1f, 0x0c);
			break;    
		case AE_EV_COMP_n10:
			OV3660_write_cmos_sensor(0x3a0f, 0x18);
			OV3660_write_cmos_sensor(0x3a10, 0x10);
			OV3660_write_cmos_sensor(0x3a1b, 0x18);
			OV3660_write_cmos_sensor(0x3a1e, 0x10);
			OV3660_write_cmos_sensor(0x3a11, 0x30);
			OV3660_write_cmos_sensor(0x3a1f, 0x08);
			break;
		case AE_EV_COMP_n13:
			OV3660_write_cmos_sensor(0x3a0f, 0x10); // stable high in
			OV3660_write_cmos_sensor(0x3a10, 0x08); // stable low in
			OV3660_write_cmos_sensor(0x3a1b, 0x10); // stable high out
			OV3660_write_cmos_sensor(0x3a1e, 0x08); // stable low out
			OV3660_write_cmos_sensor(0x3a11, 0x20); // fast zone high
			OV3660_write_cmos_sensor(0x3a1f, 0x04); // fast zone low
			break;
		default:
			return FALSE;
	}

	return TRUE;
	
} /* OV3660_set_param_exposure */


UINT32 OV3660YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{

	switch (iCmd) {
	case FID_SCENE_MODE:	    
//	    printk("Set Scene Mode:%d\n", iPara); 
	    if (iPara == SCENE_MODE_OFF)
	    {
	        OV3660_night_mode(0); 
	    }
	    else if (iPara == SCENE_MODE_NIGHTSCENE)
	    {
               OV3660_night_mode(1); 
	    }	    
	    break; 	    
	case FID_AWB_MODE:
//	    printk("Set AWB Mode:%d\n", iPara); 	    
           OV3660_set_param_wb(iPara);
	break;
	case FID_COLOR_EFFECT:
//	    printk("Set Color Effect:%d\n", iPara); 	    	    
           OV3660_set_param_effect(iPara);
	break;
	case FID_AE_EV:
//           printk("Set EV:%d\n", iPara); 	    	    
           OV3660_set_param_exposure(iPara);
	break;
	case FID_AE_FLICKER:
//           printk("Set Flicker:%d\n", iPara); 	    	    	    
           OV3660_set_param_banding(iPara);
	break;
	case FID_AE_SCENE_MODE: 
	    if (iPara == AE_MODE_OFF) {
                OV3660_AE_ENABLE = KAL_FALSE; 
            }
            else {
                OV3660_AE_ENABLE = KAL_TRUE; 
	    }
            OV3660_set_AE_mode(OV3660_AE_ENABLE);
            break; 

	case FID_ZOOM_FACTOR:
	    zoom_factor = iPara; 		
	break; 
	default:
	break;
	}
	return TRUE;
}   /* OV3660YUVSensorSetting */

UINT32 OV3660YUVSetVideoMode(UINT16 u2FrameRate)
{
    kal_uint8 iTemp;
    printk("[OV2640YUV] Set Video Mode \n"); 
    OV3660_VEDIO_encode_mode = KAL_TRUE; 

    //iTemp = OV3660_read_cmos_sensor(0x3014);
    //OV3660_write_cmos_sensor(0x3014, iTemp & 0xf7); //Disable night mode

    if (u2FrameRate == 30)
    {
    
        OV3660_write_cmos_sensor(0x3015,0x01);
        OV3660_write_cmos_sensor(0x3011,0x00);
        OV3660_write_cmos_sensor(0x302d,0x00);
        OV3660_write_cmos_sensor(0x302e,0x00);
    }
    else if (u2FrameRate == 15)
    {
        // set Max gain to 16X
        OV3660_write_cmos_sensor(0x3015, 0x01); //jerry
        OV3660_write_cmos_sensor(0x3011,0x01);
        // clear extra exposure line
	 OV3660_write_cmos_sensor(0x302d, 0x00);
	 OV3660_write_cmos_sensor(0x302e, 0x00);
    }
    else 
    {
        printk("Wrong Frame Rate \n"); 
    }
    
    
    return TRUE;
}

UINT32 OV3660FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
       UINT16 u2Temp = 0; 
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;


	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=OV3660_IMAGE_SENSOR_FULL_WIDTH;
			*pFeatureReturnPara16=OV3660_IMAGE_SENSOR_FULL_HEIGHT;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PERIOD:
			*pFeatureReturnPara16++=OV3660_PV_PERIOD_PIXEL_NUMS+OV3660_PV_dummy_pixels;
			*pFeatureReturnPara16=OV3660_PV_PERIOD_LINE_NUMS+OV3660_PV_dummy_lines;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*pFeatureReturnPara32 = OV3660_sensor_pclk/10;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_SET_ESHUTTER:
//			u2Temp = OV3660_read_shutter(); 
//			printk("Shutter:%d\n", u2Temp); 			
		break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			OV3660_night_mode((BOOL) *pFeatureData16);
		break;
		case SENSOR_FEATURE_SET_GAIN:
//			u2Temp = OV3660_read_gain(); 
//			printk("Gain:%d\n", u2Temp); 
//			printk("y_val:%d\n", OV3660_read_cmos_sensor(0x301B));
			break; 
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			OV3660_isp_master_clock=*pFeatureData32;
		break;
		case SENSOR_FEATURE_SET_REGISTER:
			OV3660_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
		break;
		case SENSOR_FEATURE_GET_REGISTER:
			pSensorRegData->RegData = OV3660_read_cmos_sensor(pSensorRegData->RegAddr);
		break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			memcpy(pSensorConfigData, &OV3660SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
			*pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
		break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:

		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
		case SENSOR_FEATURE_GET_GROUP_INFO:
		case SENSOR_FEATURE_GET_ITEM_INFO:
		case SENSOR_FEATURE_SET_ITEM_INFO:
		case SENSOR_FEATURE_GET_ENG_INFO:
		break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
	                *pFeatureReturnPara32++=0;
			*pFeatureParaLen=4;
		    break; 
		
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_SET_YUV_CMD:
//		       printk("OV3660 YUV sensor Setting:%d, %d \n", *pFeatureData32,  *(pFeatureData32+1));
			OV3660YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
		break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		       OV3660YUVSetVideoMode(*pFeatureData16);
		       break; 
        case SENSOR_FEATURE_CHECK_SENSOR_ID:
            		OV3660GetSensorID(pFeatureReturnPara32); 
			break;			
		default:
			break;			
	}
	return ERROR_NONE;
}	/* OV3660FeatureControl() */


SENSOR_FUNCTION_STRUCT	SensorFuncOV3660=
{
	OV3660Open,
	OV3660GetInfo,
	OV3660GetResolution,
	OV3660FeatureControl,
	OV3660Control,
	OV3660Close
};

UINT32 OV3660_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncOV3660;

	return ERROR_NONE;
}	/* SensorInit() */


