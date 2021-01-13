 
#ifndef STK8313_H
#define STK8313_H 
	 
#include <linux/ioctl.h>
	 
#define STK8313_I2C_SLAVE_ADDR		0x22 
	 
	 /* STK8313 Register Map  (Please refer to STK8313 Specifications) */

#define STK8313_REG_DEVID			0x0E //use  device id = 0x3A
#define STK8313_REG_MODE    		0x0A //use


#define STK8313_REG_OFSX			0x0F //use
#define STK8313_REG_OFSY			0x10 //use
#define STK8313_REG_OFSZ			0x11 //use


#define STK8313_REG_XYZ_DATA_CFG		0x16 //use
#define STK8313_REG_INT    		0x17 //use
#define STK8313_REG_RESET    		0x20 //use


#define STK8313_REG_DATAX0			0x00 //use



//end define register



#define STK8313_FIXED_DEVID			0x58 //use
	 

	 
#define STK8313_MEASURE_MODE		0x01 //use	

	 
#define STK8313_RANGE_2G			0x00 //use
#define STK8313_RANGE_4G			0x01 //use
#define STK8313_RANGE_8G			0x02 //use
#define STK8313_RANGE_16G			0x03 //use

#define STK8313_10BIT_RES	        0x00
#define STK8313_11BIT_RES	        0x01
#define STK8313_12BIT_RES	        0x02

#define STK8313_SUCCESS						0
#define STK8313_ERR_I2C						-1
#define STK8313_ERR_STATUS					-3
#define STK8313_ERR_SETUP_FAILURE			-4
#define STK8313_ERR_GETGSENSORDATA			-5
#define STK8313_ERR_IDENTIFICATION			-6
	 
	 
	 
#define STK8313_BUFSIZE				256
	 
#endif

