
#ifndef _CAMERA_3A_PARA_HM3451RAW_MT6573_H
#define _CAMERA_3A_PARA_HM3451RAW_MT6573_H

#include "camera_AE_para_hm3451raw_mt6573.h"
#include "camera_AF_para_hm3451raw_mt6573.h"
#include "camera_AWB_para_hm3451raw_mt6573.h"


// 3A cycle
static UINT32 g_u4AAACycle[AAA_CYCLE_MAX] = 
{
    AAA_CYCLE_AWB,
    AAA_CYCLE_AE,
    AAA_CYCLE_AF, 
    AAA_CYCLE_NONE,
    AAA_CYCLE_NONE,
    AAA_CYCLE_NONE,
    AAA_CYCLE_NONE,
    AAA_CYCLE_NONE,
    AAA_CYCLE_NONE,
    AAA_CYCLE_NONE
};

//____3A Non-NVRAM Parameter____
static AAA_PARAM_T HM3451RAW_CAMERA_3A_PARA = 
{
    TRUE, // Enable AE
    TRUE, // Enable AF
    TRUE, // Enable AWB
    
    // 3A cycle
    {
        g_u4AAACycle[0],
        g_u4AAACycle[1],
        g_u4AAACycle[2],
        g_u4AAACycle[3],
        g_u4AAACycle[4],
        g_u4AAACycle[5],
        g_u4AAACycle[6],
        g_u4AAACycle[7],
        g_u4AAACycle[8],
        g_u4AAACycle[9],
    },
    
    // AE algorithm parameter
    g_rAEParam,    
    
    // AWB algorithm parameter
    g_rAWBParam,

    // AF algorithm parameter
	g_rAFParam,

	// AE preflash algorithm parameter
	g_rAEPFParam
};

//____3A Statistics Config Parameter____
static AAA_STAT_CONFIG_PARAM_T HM3451RAW_CAMERA_3A_STAT_CONFIG_PARAM =
{
    g_rAEPreStatConfig,
    {
	    g_rAWBStatConfig[LIB3A_AWB_MODE_AUTO],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_DAYLIGHT],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_CLOUDY_DAYLIGHT],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_SHADE],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_TWILIGHT],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_FLUORESCENT],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_WARM_FLUORESCENT],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_INCANDESCENT],
	    g_rAWBStatConfig[LIB3A_AWB_MODE_GRAYWORLD]
    },
    g_rAFStatConfig   
};

#endif 
