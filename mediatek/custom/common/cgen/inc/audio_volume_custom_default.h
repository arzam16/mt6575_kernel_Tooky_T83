

#ifndef AUDIO_VOLUME_CUSTOM_DEFAULT_H
#define AUDIO_VOLUME_CUSTOM_DEFAULT_H

#define AUD_VOLUME_RING \
     0,32,64,96,128,160,192,  /*4-> normal mode*/	\
     0,32,64,96,128,132,144,  /*4-> headset mode*/	\
     0,43,85,128,171,213,255
     
#define AUD_VOLUME_KEY \
     0,43,85,128,171,213,255,\
     0,43,85,128,171,213,255,\
     0,43,85,128,171,213,255

#define AUD_VOLUME_MIC \
     64,255,255,144,255,255,160,  /*3-> normal mode*/	\
     255,192,192,180,255,192,172, /*3-> headset mode*/	\
     255,208,208,148,255,208,172   /*3-> speaker mode*/	
#define AUD_VOLUME_FMR \
     0,43,85,128,171,213,255,\
     0,43,68,109,150,191,232,/* headset/speaker mode FM volume control*/	\
     0,43,85,128,171,213,255  
     
#define AUD_VOLUME_SPH \
     0,108,120,132,140,148,156, 	   /*normal mode*/	\
     0,72,84,96,108,120,132,  	   /*headset mode*/	\
     0,96,108,120,132,144,156        /*speaker mode*/
#define AUD_VOLUME_SID \
     0,0,16,0,0,0,0, 		/*normal mode*/		\
     0,0,32,32,0,0,0, 		/*headset mode*/	\
     0,0,0,0,0,0,0		/*speaker mode*/	

#define AUD_VOLUME_MEDIA \
     100,148,148,148,128,140,148,  /*max level control[0 1 2 3 4 5 6]5->headset, 6->speaker*/	\
     0,43,85,128,171,213,255,\
     0,43,85,128,171,213,255

#define AUD_VOLUME_MATV \
     0,43,85,128,171,213,255,\
     0,43,85,128,171,213,255,\
     0,43,85,128,171,213,255

#endif
