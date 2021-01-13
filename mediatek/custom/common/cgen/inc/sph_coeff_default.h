

#ifndef SPEECH_COEFF_DEFAULT_H
#define SPEECH_COEFF_DEFAULT_H

#ifndef FALSE
#define FALSE 0
#endif

//speech parameter depen on BT_CHIP cersion
#if defined(MTK_MT6611)

#define BT_COMP_FILTER (1 << 15)
#define BT_SYNC_DELAY  86

#elif defined(MTK_MT6612)

#define BT_COMP_FILTER (1 << 15)
#define BT_SYNC_DELAY  86

#elif defined(MTK_MT6616) || defined(MTK_MT6620) || defined(MTK_MT6622) || defined(MTK_MT6626) || defined(MTK_MT6628)

#define BT_COMP_FILTER (1 << 15)
#define BT_SYNC_DELAY  86

#else // MTK_MT6620

#define BT_COMP_FILTER (0 << 15)
#define BT_SYNC_DELAY  86

#endif

#ifdef MTK_DUAL_MIC_SUPPORT
#define SPEECH_MODE_PARA13 (371)
#define SPEECH_MODE_PARA14 (1031)
#else
#define SPEECH_MODE_PARA13 (0)
#define SPEECH_MODE_PARA14 (0)
#endif

#define DEFAULT_SPEECH_NORMAL_MODE_PARA \
   32,   253,  16388,     29,   57351,     31,   400,     0, \
   336,  4325,      99,       0,   29192,      0|SPEECH_MODE_PARA13,     0|SPEECH_MODE_PARA14,  8192

#define DEFAULT_SPEECH_EARPHONE_MODE_PARA \
    512,  253,  10756,      31,   57351,     0,   400,    34, \
    16656,  4325,    99,     0, 20488,     0,     0,     0  

#define DEFAULT_SPEECH_BT_EARPHONE_MODE_PARA \
     0,   253, 10756,    31, 53255,  31,   400,     0, \
    80,  4325,    99,     0, 20488 | BT_COMP_FILTER,  0,     0,     BT_SYNC_DELAY

#define DEFAULT_SPEECH_LOUDSPK_MODE_PARA \
    96,   224,  5256,    31, 57351, 24607,   400,   6, \
    80,  4325,    99,     0, 20488,     0,     0,     0

#define DEFAULT_SPEECH_CARKIT_MODE_PARA \
    96,   224,  5256,    31, 57351, 24607,   400,   132, \
    80,  4325,    99,     0, 20488,        0,     0,     0

#define DEFAULT_SPEECH_BT_CORDLESS_MODE_PARA \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0

#define DEFAULT_SPEECH_AUX1_MODE_PARA \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0

#define DEFAULT_SPEECH_AUX2_MODE_PARA \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0

#define DEFAULT_SPEECH_COMMON_PARA \
    0,  55997,  31000,    10752,      32769,      0,      0,      0, \
    0,      0,      0,      0

#define DEFAULT_SPEECH_VOL_PARA \
    0,      0,      0,      0

#define DEFAULT_AUDIO_DEBUG_INFO \
    0,      0,      0,      0,      0,      0,      0,      0, \
    0,      0,      0,      0,      0,      0,      0,      0

#define DEFAULT_VM_SUPPORT  FALSE

#define DEFAULT_AUTO_VM     FALSE

#define MICBAIS     1900

#define DEFAULT_WB_SPEECH_NORMAL_MODE_PARA \
    96,   253, 16388,    31, 57607,    31,   400,     0, \
    80,  4325,   99,     0,  16392,    0|SPEECH_MODE_PARA13,     0|SPEECH_MODE_PARA14,  8192  

#define DEFAULT_WB_SPEECH_EARPHONE_MODE_PARA \
     0,   189, 10756,    31, 57607,     31,  400,     0, \
    80,  4325,   99,     0,  16392,     0,     0,     0  

#define DEFAULT_WB_SPEECH_BT_EARPHONE_MODE_PARA \
     0,   253, 10756,    31, 53511,  31,   400,     0, \
    80,  4325,   99,     0,  16392 | BT_COMP_FILTER,  0,     0, BT_SYNC_DELAY  

#define DEFAULT_WB_SPEECH_LOUDSPK_MODE_PARA \
    96,   224,  5256,    31, 57607, 24607,   400,   132, \
    84,  4325,   99,     0,  16392,     0,     0,     0  

#define DEFAULT_WB_SPEECH_CARKIT_MODE_PARA \
    96,   224,  5256,    31, 57607, 24607,   400,   132, \
    84,  4325,   99,     0,  16392,     0,     0,     0  

#define DEFAULT_WB_SPEECH_BT_CORDLESS_MODE_PARA \
    0,      0,     0,     0,       0,     0,     0,     0, \
    0,      0,     0,     0,       0,     0,     0,     0  

#define DEFAULT_WB_SPEECH_AUX1_MODE_PARA \
    96,   224,  5256,    31, 57607,     0,   400,     0,  4112,  4325,    11,     0,     0,     0,     0,     0 

#define DEFAULT_WB_SPEECH_AUX2_MODE_PARA \
    0,      0,     0,     0,       0,     0,     0,     0, \
    0,      0,     0,     0,       0,     0,     0,     0  


/* The Bluetooth PCM digital volume */
#define DEFAULT_BT_PCM_IN_VOL        0x100
#define DEFAULT_BT_PCM_OUT_VOL       0x1000

#define DEFAULT_SPEECH_DUAL_MIC_ABF_PARA \
	4,4,4,4,4, \
	4,4,5,10965,11818, \
	11632,13521,9888,11096,12944, \
	8567,2,5,27,24, \
	25,25,24,26,30, \
	32,16501,23640,1946,2337, \
	3366,5060,8799,3956,12986, \
	15874,0,0,2,21930, \
	68,0,0,0


#define DEFAULT_SPEECH_DUAL_MIC_ABFWB_PARA \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
      0, 0, 68, 0, 0, 0

#endif
