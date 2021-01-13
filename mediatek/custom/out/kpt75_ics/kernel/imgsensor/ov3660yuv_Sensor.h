/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Header file of Sensor driver
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
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
/* SENSOR FULL SIZE */
#ifndef __SENSOR_H
#define __SENSOR_H



typedef enum _OV3660_OP_TYPE_ {
        OV3660_MODE_NONE,
        OV3660_MODE_PREVIEW,
        OV3660_MODE_CAPTURE,
        OV3660_MODE_QCIF_VIDEO,
        OV3660_MODE_CIF_VIDEO,
        OV3660_MODE_QVGA_VIDEO
    } OV3660_OP_TYPE;

extern OV3660_OP_TYPE OV3660_g_iOV3660_Mode;

/* START GRAB PIXEL OFFSET */
#define IMAGE_SENSOR_START_GRAB_X		        8	// 0 or 1 recommended
#define IMAGE_SENSOR_START_GRAB_Y		        6	// 0 or 1 recommended
#define IMAGE_FULL_GRAB_START_X			16
#define IMAGE_FULL_GRAB_START_Y			12
/* MAX/MIN FRAME RATE (FRAMES PER SEC.) */
#define MAX_FRAME_RATE							15		// Limitation for MPEG4 Encode Only
#define MIN_FRAME_RATE							12

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
#define OV3660_FULL_PERIOD_PIXEL_NUMS  (2376)  // default pixel#(w/o dummy pixels) in UXGA mode
#define OV3660_FULL_PERIOD_LINE_NUMS   (1568)  // default line#(w/o dummy lines) in UXGA mode
#define OV3660_PV_PERIOD_PIXEL_NUMS    (OV3660_FULL_PERIOD_PIXEL_NUMS / 2)  // default pixel#(w/o dummy pixels) in SVGA mode
#define OV3660_PV_PERIOD_LINE_NUMS     (784)   // default line#(w/o dummy lines) in SVGA mode

/* SENSOR EXPOSURE LINE LIMITATION */
#define OV3660_FULL_MAX_LINES_PER_FRAME    (1568)  // QXGA mode    
#define OV3660_FULL_EXPOSURE_LIMITATION    (OV3660_FULL_MAX_LINES_PER_FRAME)
#define OV3660_PV_MAX_LINES_PER_FRAME      (784)  // # of lines in one XGA frame    
#define OV3660_PV_EXPOSURE_LIMITATION      (OV3660_PV_MAX_LINES_PER_FRAME)

/* SENSOR FULL SIZE */
#define OV3660_IMAGE_SENSOR_FULL_WIDTH	   (2048)  
#define OV3660_IMAGE_SENSOR_FULL_HEIGHT	 (1536)    



/* SENSOR PV SIZE */
#define OV3660_IMAGE_SENSOR_PV_WIDTH   (1024)   
#define OV3660_IMAGE_SENSOR_PV_HEIGHT (768)


//SENSOR 3M size
#define OV3660_IMAGE_SENSOR_3M_WIDTH 	   (2048)	  
#define OV3660_IMAGE_SENSOR_3M_HEIGHT	   (1536)


#define OV3660_VIDEO_QCIF_WIDTH   (176)
#define OV3660_VIDEO_QCIF_HEIGHT  (144)

#define OV3660_VIDEO_30FPS_FRAME_LENGTH   (0x29E)
#define OV3660_VIDEO_20FPS_FRAME_LENGTH   (0x3ED)
#define OV3660_VIDEO_15FPS_FRAME_LENGTH   (0x53C)
#define OV3660_VIDEO_10FPS_FRAME_LENGTH   (0x7DA)

// SETUP TIME NEED TO BE INSERTED
#define OV3660_IMAGE_SENSOR_PV_INSERTED_PIXELS (390)
#define OV3660_IMAGE_SENSOR_PV_INSERTED_LINES  (9 - 6)

#define OV3660_IMAGE_SENSOR_FULL_INSERTED_PIXELS   (248)
#define OV3660_IMAGE_SENSOR_FULL_INSERTED_LINES    (11 - 2)

#define OV3660_PV_DUMMY_PIXELS			(0)
#define OV3660_VIDEO__CIF_DUMMY_PIXELS  (0)
#define OV3660_VIDEO__QCIF_DUMMY_PIXELS (0)

/* SENSOR SCALER FACTOR */
#define PV_SCALER_FACTOR					    3
#define FULL_SCALER_FACTOR					    1


/* DUMMY NEEDS TO BE INSERTED */


/* SENSOR READ/WRITE ID */
#define OV3660_WRITE_ID							    0x78
#define OV3660_READ_ID								0x79


//export functions
UINT32 OV3660Open(void);
UINT32 OV3660GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 OV3660GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV3660Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV3660FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 OV3660Close(void);




#endif /* __SENSOR_H */
