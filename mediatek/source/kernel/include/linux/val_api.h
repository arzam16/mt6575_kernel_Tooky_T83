

#ifndef _VAL_API_H_
#define _VAL_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "val_types.h"


VAL_UINT32_T eVideoDecInitMVA(VAL_VOID_T** a_pvHandle);
VAL_UINT32_T eVideoDecAllocMVA(VAL_VOID_T* a_pvHandle, VAL_UINT32_T a_u4Va, VAL_UINT32_T* ap_u4Pa, VAL_UINT32_T a_u4Size);
VAL_UINT32_T eVideoDecFreeMVA(VAL_VOID_T* a_pvHandle, VAL_UINT32_T a_u4Va, VAL_UINT32_T a_u4Pa, VAL_UINT32_T a_u4Size);
VAL_UINT32_T eVideoDecDeInitMVA(VAL_VOID_T* a_pvHandle);

VAL_UINT32_T eVideoMp4EncAllocMVA(
    VAL_VOID_T** a_pvHandle,
    VAL_UINT32_T a_u4Va, 
    VAL_UINT32_T* ap_u4Pa, 
    VAL_UINT32_T a_u4Size
);

VAL_UINT32_T eVideoMp4EncFreeMVA(
    VAL_VOID_T* a_pvHandle,
    VAL_UINT32_T a_u4Va, 
    VAL_UINT32_T ap_u4Pa, 
    VAL_UINT32_T a_u4Size
);

VAL_RESULT_T eVideoMemAlloc(
    VAL_MEMORY_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoMemFree(
    VAL_MEMORY_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoMemSet(
    VAL_MEMORY_T *a_prParam,
    VAL_UINT32_T a_u4ParamSize,
    VAL_INT32_T a_u4Value,
    VAL_UINT32_T a_u4Size
);

VAL_RESULT_T eVideoMemCpy(
    VAL_MEMORY_T *a_prParamDst,
    VAL_UINT32_T a_u4ParamDstSize,
    VAL_MEMORY_T *a_prParamSrc,
    VAL_UINT32_T a_u4ParamSrcSize,
    VAL_UINT32_T a_u4Size
);

VAL_RESULT_T eVideoMemCmp(
    VAL_MEMORY_T *a_prParamSrc1,
    VAL_UINT32_T a_u4ParamSrc1Size,
    VAL_MEMORY_T *a_prParamSrc2,
    VAL_UINT32_T a_u4ParamSrc2Size,
    VAL_UINT32_T a_u4Size
);

VAL_RESULT_T eVideoIntMemUsed(
    VAL_INTMEM_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoIntMemAlloc(
    VAL_INTMEM_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoIntMemFree(
    VAL_INTMEM_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoCreateEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoCloseEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoWaitEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoSetEvent(
    VAL_EVENT_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoCreateMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoCloseMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoWaitMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoReleaseMutex(
    VAL_MUTEX_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoGetTimeOfDay(
    VAL_TIME_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoStrStr(
    VAL_STRSTR_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoAtoi(
    VAL_ATOI_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);




#ifdef MT6575

 VAL_RESULT_T eVideoCacheSwitch(
    VAL_MEMORY_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize,
    VAL_CACHE_TYPE_T a_eCacheType
);	
 	

 	

 /**
 * @par Function	   
 *	 eVideoInvalidateCache
 * @par Description    
 *	 invalid cache of memory range
 * @param			   
 *	 a_prParam			 [IN/OUT]	 The VAL_MEMORY_T structure
 * @param			   
 *	 a_u4ParamSize		 [IN]		 The size of VAL_MEMORY_T
 * @par Returns 	   
 *	 VAL_RESULT_T
 */  
 VAL_RESULT_T eVideoInvalidateCache(
	 VAL_MEMORY_T *a_prParam,
	 VAL_UINT32_T a_u4ParamSize,
	 VAL_UINT32_T optype	 
 );
 
 
 /**
 * @par Function	   
 *	 eVideoFlushCache
 * @par Description    
 *	 Flush Cache of memory range
 * @param			   
 *	 a_prParam			 [IN/OUT]	 The VAL_MEMORY_T structure
 * @param			   
 *	 a_u4ParamSize		 [IN]		 The size of VAL_MEMORY_T
 * @par Returns 	   
 *	 VAL_RESULT_T
 */
 VAL_RESULT_T eVideoFlushCache(
	 VAL_MEMORY_T *a_prParam,
	 VAL_UINT32_T a_u4ParamSize,
 	 VAL_UINT32_T optype
 );

 	
 
 /*=============================================================================
 *                             WAIT/ISR Function Declaration
 *===========================================================================*/
 // Note:  here a_u4ParamSize is reserved for future used.


VAL_RESULT_T WaitISR(
    VAL_ISR_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);




 /*=============================================================================
 *                             TIME Function Declaration
 *===========================================================================*/

VAL_RESULT_T eVideoGetTimeOfDay(
    VAL_TIME_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);


 /*=============================================================================
 *                             POWER/CLOCK Function Declaration
 *===========================================================================*/
 // global power control
VAL_RESULT_T eHalHwPowerCtrl(
    VAL_POWER_T     *a_prParam,
    VAL_UINT32_T    a_u4ParamSize
);

//may not necessary since hw clock can be controlled by library.

 /*=============================================================================
 *                             MMAP Function Declaration
 *===========================================================================*/

VAL_RESULT_T eVideoMMAP(
    VAL_MMAP_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);

VAL_RESULT_T eVideoUnMMAP(
    VAL_MMAP_T *a_prParam, 
    VAL_UINT32_T a_u4ParamSize
);



 /*=============================================================================
 *                              HW Resource Function Declaration
 *===========================================================================*/
VAL_RESULT_T eVideoLockHW(
    VAL_HW_LOCK_T *a_prParam, 
    VAL_UINT32_T  a_u4ParamSize
);

VAL_RESULT_T eVideoUnLockHW(
    VAL_HW_LOCK_T *a_prParam, 
    VAL_UINT32_T  a_u4ParamSize
);

VAL_RESULT_T eVideoInitLockHW( VAL_VCODEC_OAL_HW_REGISTER_T *prParam, int size);
VAL_RESULT_T eVideoDeInitLockHW( VAL_VCODEC_OAL_HW_REGISTER_T *prParam, int size);
VAL_RESULT_T eVideoVcodecSetThreadID (VAL_VCODEC_THREAD_ID_T *a_prThreadID);

#endif

#ifdef __cplusplus
}
#endif

#endif // #ifndef _VAL_API_H_
