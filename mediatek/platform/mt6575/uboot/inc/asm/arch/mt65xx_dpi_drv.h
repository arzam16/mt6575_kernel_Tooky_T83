

#ifndef __mt65xx_DPI_DRV_H__
#define __mt65xx_DPI_DRV_H__

#include <asm/arch/mt65xx_typedefs.h>
#include "lcm_drv.h"
#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------

#define DPI_CHECK_RET(expr)             \
    do {                                \
        DPI_STATUS ret = (expr);        \
        ret = ret;                      \
        ASSERT(DPI_STATUS_OK == ret);   \
    } while (0)

// ---------------------------------------------------------------------------

typedef enum
{	
   DPI_STATUS_OK = 0,

   DPI_STATUS_ERROR,
} DPI_STATUS;


typedef enum
{
    DPI_FB_FORMAT_RGB565 = 0,
    DPI_FB_FORMAT_RGB888 = 1,
    DPI_FB_FORMAT_XRGB888 = 1,
    DPI_FB_FORMAT_RGBX888 = 1,
    DPI_FB_FORMAT_NUM,
} DPI_FB_FORMAT;
        

typedef enum
{
    DPI_RGB_ORDER_RGB = 0,
    DPI_RGB_ORDER_BGR = 1,
    DPI_RGB_ORDER_XRGB8888 = 2,
    DPI_RGB_ORDER_RGBX8888 = 3,
} DPI_RGB_ORDER;


typedef enum
{
    DPI_FB_0   = 0,
    DPI_FB_1   = 1,
    DPI_FB_2   = 2,
    DPI_FB_NUM,
} DPI_FB_ID;


typedef enum
{
    DPI_POLARITY_RISING  = 0,
    DPI_POLARITY_FALLING = 1
} DPI_POLARITY;

// ---------------------------------------------------------------------------

DPI_STATUS DPI_Init(BOOL isDpiPoweredOn);
DPI_STATUS DPI_Deinit(void);
DPI_STATUS DPI_Set_DrivingCurrent(LCM_PARAMS *lcm_params);
DPI_STATUS DPI_PowerOn(void);
DPI_STATUS DPI_PowerOff(void);

DPI_STATUS DPI_EnableClk(void);
DPI_STATUS DPI_DisableClk(void);

DPI_STATUS DPI_EnableSeqOutput(BOOL enable);
DPI_STATUS DPI_SetRGBOrder(DPI_RGB_ORDER input, DPI_RGB_ORDER output);

DPI_STATUS DPI_ConfigPixelClk(DPI_POLARITY polarity, UINT32 divisor, UINT32 duty);
DPI_STATUS DPI_ConfigDataEnable(DPI_POLARITY polarity);
DPI_STATUS DPI_ConfigVsync(DPI_POLARITY polarity,
                           UINT32 pulseWidth, UINT32 backPorch, UINT32 frontPorch);
DPI_STATUS DPI_ConfigHsync(DPI_POLARITY polarity,
                           UINT32 pulseWidth, UINT32 backPorch, UINT32 frontPorch);

DPI_STATUS DPI_FBSyncFlipWithLCD(BOOL enable);
DPI_STATUS DPI_SetDSIMode(BOOL enable);
BOOL 	   DPI_IsDSIMode(void);
DPI_STATUS DPI_FBSetFormat(DPI_FB_FORMAT format);
DPI_STATUS DPI_FBSetSize(UINT32 width, UINT32 height);
DPI_STATUS DPI_FBEnable(DPI_FB_ID id, BOOL enable);
DPI_STATUS DPI_FBSetAddress(DPI_FB_ID id, UINT32 address);
DPI_STATUS DPI_FBSetPitch(DPI_FB_ID id, UINT32 pitchInByte);

DPI_STATUS DPI_SetFifoThreshold(UINT32 low, UINT32 high);

// Debug
DPI_STATUS DPI_DumpRegisters(void);

// ---------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // __mt65xx_DPI_DRV_H__
