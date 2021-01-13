

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include "FM50AF.h"
#include "../camera/kd_camera_hw.h"

#define FM50AF_DRVNAME "FM50AF"
#define FM50AF_VCM_WRITE_ID           0x18

#define FM50AF_DEBUG
#ifdef FM50AF_DEBUG
#define FM50AFDB printk
#else
#define FM50AFDB(x,...)
#endif

static spinlock_t g_FM50AF_SpinLock;

static struct i2c_client * g_pstFM50AF_I2Cclient = NULL;

static dev_t g_FM50AF_devno;
static struct cdev * g_pFM50AF_CharDrv = NULL;
static struct class *actuator_class = NULL;

static int  g_s4FM50AF_Opened = 0;
static long g_i4MotorStatus = 0;
static long g_i4Dir = 0;
static long g_i4Position = 0;
static unsigned long g_u4FM50AF_INF = 0;
static unsigned long g_u4FM50AF_MACRO = 1023;
static unsigned long g_u4TargetPosition = 0;
static unsigned long g_u4CurrPosition   = 0;
//static struct work_struct g_stWork;     // --- Work queue ---
//static XGPT_CONFIG	g_GPTconfig;		// --- Interrupt Config ---


extern s32 mt_set_gpio_mode(u32 u4Pin, u32 u4Mode);
extern s32 mt_set_gpio_out(u32 u4Pin, u32 u4PinOut);
extern s32 mt_set_gpio_dir(u32 u4Pin, u32 u4Dir);


static int s4FM50AF_ReadReg(unsigned short * a_pu2Result)
{
    int  i4RetValue = 0;
    char pBuff[2];

    i4RetValue = i2c_master_recv(g_pstFM50AF_I2Cclient, pBuff , 2);

    if (i4RetValue < 0) 
    {
        FM50AFDB("[FM50AF] I2C read failed!! \n");
        return -1;
    }

    *a_pu2Result = (((u16)pBuff[0]) << 4) + (pBuff[1] >> 4);

    return 0;
}

static int s4FM50AF_WriteReg(u16 a_u2Data)
{
    int  i4RetValue = 0;

    char puSendCmd[2] = {(char)(a_u2Data >> 4) , (char)(((a_u2Data & 0xF) << 4)+0xF)};

	//mt_set_gpio_out(97,1);
    i4RetValue = i2c_master_send(g_pstFM50AF_I2Cclient, puSendCmd, 2);
	//mt_set_gpio_out(97,0);
	
    if (i4RetValue < 0) 
    {
        FM50AFDB("[FM50AF] I2C send failed!! \n");
        return -1;
    }

    return 0;
}

inline static int getFM50AFInfo(__user stFM50AF_MotorInfo * pstMotorInfo)
{
    stFM50AF_MotorInfo stMotorInfo;
    stMotorInfo.u4MacroPosition   = g_u4FM50AF_MACRO;
    stMotorInfo.u4InfPosition     = g_u4FM50AF_INF;
    stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
	if (g_i4MotorStatus == 1)	{stMotorInfo.bIsMotorMoving = TRUE;}
	else						{stMotorInfo.bIsMotorMoving = FALSE;}

	if (g_s4FM50AF_Opened >= 1)	{stMotorInfo.bIsMotorOpen = TRUE;}
	else						{stMotorInfo.bIsMotorOpen = FALSE;}

    if(copy_to_user(pstMotorInfo , &stMotorInfo , sizeof(stFM50AF_MotorInfo)))
    {
        FM50AFDB("[FM50AF] copy to user failed when getting motor information \n");
    }

    return 0;
}

inline static int moveFM50AF(unsigned long a_u4Position)
{
    if((a_u4Position > g_u4FM50AF_MACRO) || (a_u4Position < g_u4FM50AF_INF))
    {
        FM50AFDB("[FM50AF] out of range \n");
        return -EINVAL;
    }

	if (g_s4FM50AF_Opened == 1)
	{
		unsigned short InitPos;
	
		if(s4FM50AF_ReadReg(&InitPos) == 0)
		{
			FM50AFDB("[FM50AF] Init Pos %6d \n", InitPos);
		
			g_u4CurrPosition = (unsigned long)InitPos;
		}
		else
		{
			g_u4CurrPosition = 0;
		}
		
		g_s4FM50AF_Opened = 2;
	}

	if      (g_u4CurrPosition < a_u4Position)	{g_i4Dir = 1;}
	else if (g_u4CurrPosition > a_u4Position)	{g_i4Dir = -1;}
	else										{return 0;}

	if (1)
	{
		g_i4Position = (long)g_u4CurrPosition;
		g_u4TargetPosition = a_u4Position;

		if (g_i4Dir == 1)
		{
			//if ((g_u4TargetPosition - g_u4CurrPosition)<60)
			{		
				g_i4MotorStatus = 0;
				if(s4FM50AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
				{
					g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
				}
				else
				{
					FM50AFDB("[FM50AF] set I2C failed when moving the motor \n");
					g_i4MotorStatus = -1;
				}
			}
			//else
			//{
			//	g_i4MotorStatus = 1;
			//}
		}
		else if (g_i4Dir == -1)
		{
			//if ((g_u4CurrPosition - g_u4TargetPosition)<60)
			{
				g_i4MotorStatus = 0;		
				if(s4FM50AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
				{
					g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
				}
				else
				{
					FM50AFDB("[FM50AF] set I2C failed when moving the motor \n");
					g_i4MotorStatus = -1;
				}
			}
			//else
			//{
			//	g_i4MotorStatus = 1;		
			//}
		}
	}
	else
	{
	g_i4Position = (long)g_u4CurrPosition;
	g_u4TargetPosition = a_u4Position;
	g_i4MotorStatus = 1;
	}

    return 0;
}

inline static int setFM50AFInf(unsigned long a_u4Position)
{
	g_u4FM50AF_INF = a_u4Position;
	return 0;
}

inline static int setFM50AFMacro(unsigned long a_u4Position)
{
	g_u4FM50AF_MACRO = a_u4Position;
	return 0;	
}

////////////////////////////////////////////////////////////////
static long FM50AF_Ioctl(
struct file * a_pstFile,
unsigned int a_u4Command,
unsigned long a_u4Param)
{
    long i4RetValue = 0;

    switch(a_u4Command)
    {
        case FM50AFIOC_G_MOTORINFO :
            i4RetValue = getFM50AFInfo((__user stFM50AF_MotorInfo *)(a_u4Param));
        break;

        case FM50AFIOC_T_MOVETO :
            i4RetValue = moveFM50AF(a_u4Param);
        break;
 
 		case FM50AFIOC_T_SETINFPOS :
			 i4RetValue = setFM50AFInf(a_u4Param);
		break;

 		case FM50AFIOC_T_SETMACROPOS :
			 i4RetValue = setFM50AFMacro(a_u4Param);
		break;
		
        default :
      	     FM50AFDB("[FM50AF] No CMD \n");
            i4RetValue = -EPERM;
        break;
    }

    return i4RetValue;
}
//Main jobs:
// 1.check for device-specified errors, device not ready.
// 2.Initialize the device if it is opened for the first time.
// 3.Update f_op pointer.
// 4.Fill data structures into private_data
//CAM_RESET
static int FM50AF_Open(struct inode * a_pstInode, struct file * a_pstFile)
{
    spin_lock(&g_FM50AF_SpinLock);

    if(g_s4FM50AF_Opened)
    {
        spin_unlock(&g_FM50AF_SpinLock);
        FM50AFDB("[FM50AF] the device is opened \n");
        return -EBUSY;
    }

    g_s4FM50AF_Opened = 1;
		
    spin_unlock(&g_FM50AF_SpinLock);

	// --- Config Interrupt ---
	//g_GPTconfig.num = XGPT7;
	//g_GPTconfig.mode = XGPT_REPEAT;
	//g_GPTconfig.clkDiv = XGPT_CLK_DIV_1;//32K
	//g_GPTconfig.u4Compare = 32*2; // 2ms
	//g_GPTconfig.bIrqEnable = TRUE;
	
	//XGPT_Reset(g_GPTconfig.num);	
	//XGPT_Init(g_GPTconfig.num, FM50AF_ISR);

	//if (XGPT_Config(g_GPTconfig) == FALSE)
	//{
        //FM50AFDB("[FM50AF] ISR Config Fail\n");	
	//	return -EPERM;
	//}

	//XGPT_Start(g_GPTconfig.num);		

	// --- WorkQueue ---	
	//INIT_WORK(&g_stWork,FM50AF_WORK);

    return 0;
}

//Main jobs:
// 1.Deallocate anything that "open" allocated in private_data.
// 2.Shut down the device on last close.
// 3.Only called once on last time.
// Q1 : Try release multiple times.
static int FM50AF_Release(struct inode * a_pstInode, struct file * a_pstFile)
{
	unsigned int cnt = 0;

	if (g_s4FM50AF_Opened)
	{
		moveFM50AF(g_u4FM50AF_INF);

		while(g_i4MotorStatus)
		{
			msleep(1);
			cnt++;
			if (cnt>1000)	{break;}
		}
		
    	spin_lock(&g_FM50AF_SpinLock);

	    g_s4FM50AF_Opened = 0;

    	spin_unlock(&g_FM50AF_SpinLock);

    	//hwPowerDown(CAMERA_POWER_VCAM_A,"kd_camera_hw");

		//XGPT_Stop(g_GPTconfig.num);
	}

    return 0;
}

static const struct file_operations g_stFM50AF_fops = 
{
    .owner = THIS_MODULE,
    .open = FM50AF_Open,
    .release = FM50AF_Release,
    .unlocked_ioctl = FM50AF_Ioctl
};

inline static int Register_FM50AF_CharDrv(void)
{
    struct device* vcm_device = NULL;

    //Allocate char driver no.
    if( alloc_chrdev_region(&g_FM50AF_devno, 0, 1,FM50AF_DRVNAME) )
    {
        FM50AFDB("[FM50AF] Allocate device no failed\n");

        return -EAGAIN;
    }

    //Allocate driver
    g_pFM50AF_CharDrv = cdev_alloc();

    if(NULL == g_pFM50AF_CharDrv)
    {
        unregister_chrdev_region(g_FM50AF_devno, 1);

        FM50AFDB("[FM50AF] Allocate mem for kobject failed\n");

        return -ENOMEM;
    }

    //Attatch file operation.
    cdev_init(g_pFM50AF_CharDrv, &g_stFM50AF_fops);

    g_pFM50AF_CharDrv->owner = THIS_MODULE;

    //Add to system
    if(cdev_add(g_pFM50AF_CharDrv, g_FM50AF_devno, 1))
    {
        FM50AFDB("[FM50AF] Attatch file operation failed\n");

        unregister_chrdev_region(g_FM50AF_devno, 1);

        return -EAGAIN;
    }

    actuator_class = class_create(THIS_MODULE, "actuatordrv");
    if (IS_ERR(actuator_class)) {
        int ret = PTR_ERR(actuator_class);
        FM50AFDB("Unable to create class, err = %d\n", ret);
        return ret;            
    }

    vcm_device = device_create(actuator_class, NULL, g_FM50AF_devno, NULL, FM50AF_DRVNAME);

    if(NULL == vcm_device)
    {
        return -EIO;
    }
    
    return 0;
}

inline static void Unregister_FM50AF_CharDrv(void)
{
    //Release char driver
    cdev_del(g_pFM50AF_CharDrv);

    unregister_chrdev_region(g_FM50AF_devno, 1);
    
    device_destroy(actuator_class, g_FM50AF_devno);

    class_destroy(actuator_class);
}

//////////////////////////////////////////////////////////////////////

/* Kirby: add new-style driver { */
//static int FM50AF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int FM50AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int FM50AF_i2c_remove(struct i2c_client *client);
static const struct i2c_device_id FM50AF_i2c_id[] = {{FM50AF_DRVNAME,0},{}};   
//static unsigned short force[] = {IMG_SENSOR_I2C_GROUP_ID, FM50AF_VCM_WRITE_ID, I2C_CLIENT_END, I2C_CLIENT_END};   
//static const unsigned short * const forces[] = { force, NULL };              
//static struct i2c_client_address_data addr_data = { .forces = forces,}; 
struct i2c_driver FM50AF_i2c_driver = {                       
    .probe = FM50AF_i2c_probe,                                   
    .remove = FM50AF_i2c_remove,                           
    .driver.name = FM50AF_DRVNAME,                 
    .id_table = FM50AF_i2c_id,                             
};  

#if 0 
static int FM50AF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) {         
    strcpy(info->type, FM50AF_DRVNAME);                                                         
    return 0;                                                                                       
}      
#endif 
static int FM50AF_i2c_remove(struct i2c_client *client) {
    return 0;
}
/* Kirby: } */


/* Kirby: add new-style driver {*/
static int FM50AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
/* Kirby: } */
{
    int i4RetValue = 0;

    FM50AFDB("[FM50AF] Attach I2C \n");

    /* Kirby: remove old-style driver
    //Check I2C driver capability
    if (!i2c_check_functionality(a_pstAdapter, I2C_FUNC_SMBUS_BYTE_DATA))
    {
        FM50AFDB("[FM50AF] I2C port cannot support the format \n");
        return -EPERM;
    }

    if (!(g_pstFM50AF_I2Cclient = kzalloc(sizeof(struct i2c_client), GFP_KERNEL)))
    {
        return -ENOMEM;
    }

    g_pstFM50AF_I2Cclient->addr = a_i4Address;
    g_pstFM50AF_I2Cclient->adapter = a_pstAdapter;
    g_pstFM50AF_I2Cclient->driver = &FM50AF_i2c_driver;
    g_pstFM50AF_I2Cclient->flags = 0;

    strncpy(g_pstFM50AF_I2Cclient->name, FM50AF_DRVNAME, I2C_NAME_SIZE);

    if(i2c_attach_client(g_pstFM50AF_I2Cclient))
    {
        kfree(g_pstFM50AF_I2Cclient);
    }
    */
    /* Kirby: add new-style driver { */
    g_pstFM50AF_I2Cclient = client;
    
    g_pstFM50AF_I2Cclient->addr = g_pstFM50AF_I2Cclient->addr >> 1;
    
    /* Kirby: } */

    //Register char driver
    i4RetValue = Register_FM50AF_CharDrv();

    if(i4RetValue){

        FM50AFDB("[FM50AF] register char device failed!\n");

        /* Kirby: remove old-style driver
        kfree(g_pstFM50AF_I2Cclient); */

        return i4RetValue;
    }

    spin_lock_init(&g_FM50AF_SpinLock);

    FM50AFDB("[FM50AF] Attached!! \n");

    return 0;
}


static int FM50AF_probe(struct platform_device *pdev)
{
    return i2c_add_driver(&FM50AF_i2c_driver);
}

static int FM50AF_remove(struct platform_device *pdev)
{
    i2c_del_driver(&FM50AF_i2c_driver);
    return 0;
}

static int FM50AF_suspend(struct platform_device *pdev, pm_message_t mesg)
{
//    int retVal = 0;
//    retVal = hwPowerDown(MT6516_POWER_VCAM_A,FM50AF_DRVNAME);

    return 0;
}

static int FM50AF_resume(struct platform_device *pdev)
{
    return 0;
}

// platform structure
static struct platform_driver g_stFM50AF_Driver = {
    .probe		= FM50AF_probe,
    .remove	= FM50AF_remove,
    .suspend	= FM50AF_suspend,
    .resume	= FM50AF_resume,
    .driver		= {
        .name	= "lens_actuator",
        .owner	= THIS_MODULE,
    }
};

static int __init FM50AF_i2C_init(void)
{
    if(platform_driver_register(&g_stFM50AF_Driver)){
        FM50AFDB("failed to register FM50AF driver\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit FM50AF_i2C_exit(void)
{
	platform_driver_unregister(&g_stFM50AF_Driver);
}

module_init(FM50AF_i2C_init);
module_exit(FM50AF_i2C_exit);

MODULE_DESCRIPTION("FM50AF lens module driver");
MODULE_AUTHOR("Gipi Lin <Gipi.Lin@Mediatek.com>");
MODULE_LICENSE("GPL");


