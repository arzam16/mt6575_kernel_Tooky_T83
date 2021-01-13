#ifndef _CFG_TUNING_IMGSENSOR_H_
#define _CFG_TUNING_IMGSENSOR_H_

/*******************************************************************************
* Dynamic Frame Rate
*******************************************************************************/
VdoDynamicFrameRate_T const&
getParamVdoDynamicFrameRate()
{
    static VdoDynamicFrameRate_T inst = { 
        EVThresNormal  : 50,
        EVThresNight   : 50,
        isEnableDFps    : false,
    };
    return inst;
}

/*******************************************************************************
* 
*******************************************************************************/
#endif //  _CFG_TUNING_IMGSENSOR_H_

