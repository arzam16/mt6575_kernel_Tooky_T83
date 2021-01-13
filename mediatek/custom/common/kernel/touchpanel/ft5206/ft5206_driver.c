
 
#include "tpd.h"
#include <linux/interrupt.h>
#include <cust_eint.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/rtpm_prio.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>

#include "tpd_custom_ft5206.h"
#ifdef MT6575
#include <mach/mt6575_pm_ldo.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_boot.h>
#endif

#include "cust_gpio_usage.h"
extern struct tpd_device *tpd;

#define TP_UPGRADE 0
#define FTS_PACKET_LENGTH 128


#if TP_UPGRADE
static unsigned char CTPM_FW_MUD[]=
{
	#include "firmware_mud_v0x0e.h"
};
static unsigned char CTPM_FW_JUNDA[]=
{
	#include "firmware_junda_v0x11.h"
};

 char version[2][10] = {"Mud", "Junda"};
#endif

struct i2c_client *i2c_client = NULL;
struct task_struct *thread = NULL;
 
static DECLARE_WAIT_QUEUE_HEAD(waiter);
 
 
static void tpd_eint_interrupt_handler(void);
 
 
 extern void mt65xx_eint_unmask(unsigned int line);
 extern void mt65xx_eint_mask(unsigned int line);
 extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
 extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
 extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
									  kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
									  kal_bool auto_umask);

 
static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tpd_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int __devexit tpd_remove(struct i2c_client *client);
static int touch_event_handler(void *unused);
unsigned char fts_ctpm_get_panel_factory_setting(void);
static int read_reg(u8 addr, unsigned char *pdata);


static int tpd_flag = 0;
static int point_num = 0;
static int p_point_num = 0;

//#define TPD_CLOSE_POWER_IN_SLEEP

#define TPD_OK 0
//register define

#define DEVICE_MODE 0x00
#define GEST_ID 0x01
#define TD_STATUS 0x02

#define TOUCH1_XH 0x03
#define TOUCH1_XL 0x04
#define TOUCH1_YH 0x05
#define TOUCH1_YL 0x06

#define TOUCH2_XH 0x09
#define TOUCH2_XL 0x0A
#define TOUCH2_YH 0x0B
#define TOUCH2_YL 0x0C

#define TOUCH3_XH 0x0F
#define TOUCH3_XL 0x10
#define TOUCH3_YH 0x11
#define TOUCH3_YL 0x12
//register define

#define TPD_RESET_ISSUE_WORKAROUND

#define TPD_MAX_RESET_COUNT 3

#ifdef TPD_HAVE_BUTTON 
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_dim_local[TPD_KEY_COUNT][4] = TPD_KEYS_DIM;
#endif
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
static int tpd_wb_start_local[TPD_WARP_CNT] = TPD_WARP_START;
static int tpd_wb_end_local[TPD_WARP_CNT]   = TPD_WARP_END;
#endif
#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
static int tpd_calmat_local[8]     = TPD_CALIBRATION_MATRIX;
static int tpd_def_calmat_local[8] = TPD_CALIBRATION_MATRIX;
#endif

//#define VELOCITY_CUSTOM_FT5206
#ifdef VELOCITY_CUSTOM_FT5206
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

#ifndef TPD_VELOCITY_CUSTOM_X
#define TPD_VELOCITY_CUSTOM_X 10
#endif
#ifndef TPD_VELOCITY_CUSTOM_Y
#define TPD_VELOCITY_CUSTOM_Y 10
#endif


// for magnify velocity********************************************
#define TOUCH_IOC_MAGIC 'A'

#define TPD_GET_VELOCITY_CUSTOM_X _IO(TOUCH_IOC_MAGIC,0)
#define TPD_GET_VELOCITY_CUSTOM_Y _IO(TOUCH_IOC_MAGIC,1)

int g_v_magnify_x =TPD_VELOCITY_CUSTOM_X;
int g_v_magnify_y =TPD_VELOCITY_CUSTOM_Y;
static int tpd_misc_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int tpd_misc_release(struct inode *inode, struct file *file)
{
	//file->private_data = NULL;
	return 0;
}
/*----------------------------------------------------------------------------*/
//static int adxl345_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
//       unsigned long arg)
static long tpd_unlocked_ioctl(struct file *file, unsigned int cmd,
       unsigned long arg)
{
	//struct i2c_client *client = (struct i2c_client*)file->private_data;
	//struct adxl345_i2c_data *obj = (struct adxl345_i2c_data*)i2c_get_clientdata(client);	
	char strbuf[256];
	void __user *data;
	
	long err = 0;
	
	if(_IOC_DIR(cmd) & _IOC_READ)
	{
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}

	if(err)
	{
		printk("tpd: access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
		return -EFAULT;
	}

	switch(cmd)
	{
		case TPD_GET_VELOCITY_CUSTOM_X:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}			
			
			if(copy_to_user(data, &g_v_magnify_x, sizeof(g_v_magnify_x)))
			{
				err = -EFAULT;
				break;
			}				 
			break;

	   case TPD_GET_VELOCITY_CUSTOM_Y:
			data = (void __user *) arg;
			if(data == NULL)
			{
				err = -EINVAL;
				break;	  
			}			
			
			if(copy_to_user(data, &g_v_magnify_y, sizeof(g_v_magnify_y)))
			{
				err = -EFAULT;
				break;
			}				 
			break;


		default:
			printk("tpd: unknown IOCTL: 0x%08x\n", cmd);
			err = -ENOIOCTLCMD;
			break;
			
	}

	return err;
}


static struct file_operations tpd_fops = {
//	.owner = THIS_MODULE,
	.open = tpd_misc_open,
	.release = tpd_misc_release,
	.unlocked_ioctl = tpd_unlocked_ioctl,
};
/*----------------------------------------------------------------------------*/
static struct miscdevice tpd_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "touch",
	.fops = &tpd_fops,
};

//**********************************************
#endif

struct touch_info {
    int y[4];
    int x[4];
    int p[4];
    int count;
};
 
 static const struct i2c_device_id ft5206_tpd_id[] = {{"ft5206",0},{}};
 //unsigned short force[] = {0,0x70,I2C_CLIENT_END,I2C_CLIENT_END}; 
 //static const unsigned short * const forces[] = { force, NULL };
 //static struct i2c_client_address_data addr_data = { .forces = forces, };
 static struct i2c_board_info __initdata ft5206_i2c_tpd={ I2C_BOARD_INFO("ft5206", (0x70>>1))};
 
 
 static struct i2c_driver tpd_i2c_driver = {
  .driver = {
	 .name = "ft5206",//.name = TPD_DEVICE,
//	 .owner = THIS_MODULE,
  },
  .probe = tpd_probe,
  .remove = __devexit_p(tpd_remove),
  .id_table = ft5206_tpd_id,
  .detect = tpd_detect,
//  .address_data = &addr_data,
 };
 

static  void tpd_down(int x, int y, int p) {
	// input_report_abs(tpd->dev, ABS_PRESSURE, p);
	 input_report_key(tpd->dev, BTN_TOUCH, 1);
	 input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
	 input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
	 input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
	 //printk("D[%4d %4d %4d] ", x, y, p);
	 input_mt_sync(tpd->dev);
     if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
     {   
       tpd_button(x, y, 1);  
     }
	 if(y > TPD_RES_Y) //virtual key debounce to avoid android ANR issue
	 {
         msleep(50);
		 printk("D virtual key \n");
	 }
	 TPD_EM_PRINT(x, y, x, y, p-1, 1);
 }
 
static  void tpd_up(int x, int y,int *count) {
	 //if(*count>0) {
		 //input_report_abs(tpd->dev, ABS_PRESSURE, 0);
		 input_report_key(tpd->dev, BTN_TOUCH, 0);
		 //input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
		 //input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
		 //input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
		 //printk("U[%4d %4d %4d] ", x, y, 0);
		 input_mt_sync(tpd->dev);
		 TPD_EM_PRINT(x, y, x, y, 0, 0);
	//	 (*count)--;
     if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
     {   
        tpd_button(x, y, 0); 
     }   		 

 }

  static int tpd_touchinfo(struct touch_info *cinfo, struct touch_info *pinfo)
 {

	int i = 0;

	char data[40] = {0};

	u16 high_byte,low_byte;
	u8 report_rate =0;

	p_point_num = point_num;

	i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 8, &(data[0]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x08, 8, &(data[8]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x10, 8, &(data[16]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x18, 8, &(data[24]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0xa6, 1, &(data[33]));
	i2c_smbus_read_i2c_block_data(i2c_client, 0x88, 1, &report_rate);

	 if(report_rate < 8)
	 {
	   report_rate = 0x8;
	   if((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
	   {
		   TPD_DMESG("I2C read report rate error, line: %d\n", __LINE__);
	   }
	 }	 
	
	/* Device Mode[2:0] == 0 :Normal operating Mode*/
	if(data[0] & 0x70 != 0) return false; 

	/*get the number of the touch points*/
	point_num= data[2] & 0x0f;

		
	for(i = 0; i < point_num; i++)
	{
		cinfo->p[i] = data[3+6*i] >> 6; //event flag 

		/*get the X coordinate, 2 bytes*/
		high_byte = data[3+6*i];
		high_byte <<= 8;
		high_byte &= 0x0f00;
		low_byte = data[3+6*i + 1];
		cinfo->x[i] = high_byte |low_byte;

		//cinfo->x[i] =  cinfo->x[i] * 480 >> 11; //calibra

		/*get the Y coordinate, 2 bytes*/

		high_byte = data[3+6*i+2];
		high_byte <<= 8;
		high_byte &= 0x0f00;
		low_byte = data[3+6*i+3];
		cinfo->y[i] = high_byte |low_byte;

		//cinfo->y[i]=  cinfo->y[i] * 800 >> 11;

		cinfo->count++;

	}
		  
	 return true;

 };;

 static int touch_event_handler(void *unused)
 {
  
    struct touch_info cinfo, pinfo;

	 struct sched_param param = { .sched_priority = RTPM_PRIO_TPD };
	 sched_setscheduler(current, SCHED_RR, &param);
 
	 do
	 {
	  mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM); 
		 set_current_state(TASK_INTERRUPTIBLE); 
		  wait_event_interruptible(waiter,tpd_flag!=0);
						 
			 tpd_flag = 0;
			 
		 set_current_state(TASK_RUNNING);
		 

		  if (tpd_touchinfo(&cinfo, &pinfo)) 
		  {
//		    printk("point_num = %d, cinfo.x = %d, cinfo.y = %d\n",point_num, cinfo.x[0], cinfo.y[0]);
			TPD_DEBUG_SET_TIME;
		  
            if(point_num >0) 
			{
                tpd_down(cinfo.x[0], cinfo.y[0], 1);
                if(point_num>1)
             	{
			 	tpd_down(cinfo.x[1], cinfo.y[1], 1);
			    if(point_num >2) 
			    	{
					tpd_down(cinfo.x[2], cinfo.y[2], 1);
					if(point_num >3)
						tpd_down(cinfo.x[3], cinfo.y[3], 1);
			    	}
		}
                input_sync(tpd->dev);
				//TPD_DEBUG("press --->\n");
				
            } 
			else  
            {
			    tpd_up(cinfo.x[0], cinfo.y[0], 0);
                //TPD_DEBUG("release --->\n"); 
                //input_mt_sync(tpd->dev);
                input_sync(tpd->dev);
            }
        }

 }while(!kthread_should_stop());
 
	 return 0;
 }
 
 static int tpd_detect (struct i2c_client *client, int kind, struct i2c_board_info *info) 
 {
	 strcpy(info->type, TPD_DEVICE);	
	  return 0;
 }
 
 static void tpd_eint_interrupt_handler(void)
 {
	 //TPD_DEBUG("TPD interrupt has been triggered\n");
	 tpd_flag = 1;
	 wake_up_interruptible(&waiter);

 }

#if TP_UPGRADE

static int write_reg(u8 addr, u8 para)
{
	char buf[3];
	int ret = -1;

	buf[0] = addr;
	buf[1] = para;
	ret = i2c_master_send(i2c_client, buf, 2);
	if (ret < 0){
		pr_err("write reg failed! %#x ret: %d", buf[0], ret);
		return -1;
	}
	return 0;
}

static int read_reg(u8 addr, unsigned char *pdata)
{
	int ret;
	unsigned char buf[2];
	struct  i2c_msg msgs[2];

	buf[0] = addr;               //register address

	i2c_master_send(i2c_client, &buf[0], 1);
	ret = i2c_master_recv(i2c_client, &buf[0], 1);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);

	*pdata = buf[0];
	return ret;
}

u8 cmd_write(u8 btcmd,u8 btPara1,u8 btPara2,u8 btPara3,u8 num)
{
	u8 write_cmd[4] = {0};
	i2c_client->addr = i2c_client->addr & I2C_MASK_FLAG;
	write_cmd[0] = btcmd;
	write_cmd[1] = btPara1;
	write_cmd[2] = btPara2;
	write_cmd[3] = btPara3;
	return i2c_master_send(i2c_client, write_cmd, num);
}


int  fts_ctpm_fw_upgrade(unsigned char* pbt_buf, long unsigned int dw_lenth)
{
	unsigned char reg_val[3] = {0};
	int i = 0;

	int  packet_number;
	int  j;
	int  temp;
	int  lenght;
	unsigned char  packet_buf[FTS_PACKET_LENGTH + 6];
	unsigned char  auc_i2c_write_buf[10];
	unsigned char bt_ecc;
	int      i_ret;

	/*********Step 1:Reset  CTPM *****/
	/*write 0xaa to register 0xfc*/
	write_reg(0xfc,0xaa);
	mdelay(50);
	/*write 0x55 to register 0xfc*/
	write_reg(0xfc,0x55);
	printk("Step 1: Reset CTPM test\n");

	mdelay(30);   

	/*********Step 2:Enter upgrade mode *****/
	auc_i2c_write_buf[0] = 0x55;
	auc_i2c_write_buf[1] = 0xaa;
	do
	{
		i ++;
	//	i_ret = ft5306_i2c_txdata(auc_i2c_write_buf, 2);
		i_ret = i2c_master_send(i2c_client, auc_i2c_write_buf, 2);
		mdelay(5);
	}while(i_ret <= 0 && i < 5 );
	printk("Step 2: Enter upgrade mode\n");
	
	/*********Step 3:check READ-ID***********************/        
	cmd_write(0x90,0x00,0x00,0x00,4);
	i2c_client->addr = i2c_client->addr & I2C_MASK_FLAG;
	i2c_master_recv(i2c_client, &reg_val, 2);
	printk("Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
	//byte_read(reg_val,2);
	if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
	{
		printk("Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
	}
	else
	{
		return 2;
		//i_is_new_protocol = 1;
	}

	/*********Step 4:erase app*******************************/
	cmd_write(0x61,0x00,0x00,0x00,1);

	mdelay(1500);
	//cmd_write(0x63, 0x00, 0x00, 0x00, 1);
	//mdelay(100);
	printk("Step 4: erase. \n");

	/*********Step 5:write firmware(FW) to ctpm flash*********/
	i2c_client->addr = i2c_client->addr & I2C_MASK_FLAG;
	bt_ecc = 0;
	printk("Step 5: start upgrade. \n");
	dw_lenth = dw_lenth - 8;
	printk("Packet length = %d\n", dw_lenth);
	packet_number = (dw_lenth) / FTS_PACKET_LENGTH;
	packet_buf[0] = 0xbf;
	packet_buf[1] = 0x00;
	for (j=0;j<packet_number;j++)
	{
		temp = j * FTS_PACKET_LENGTH;
		packet_buf[2] = (u8)(temp>>8);
		packet_buf[3] = (u8)temp;
		lenght = FTS_PACKET_LENGTH;
		packet_buf[4] = (u8)(lenght>>8);
		packet_buf[5] = (u8)lenght;

		for (i=0;i<FTS_PACKET_LENGTH;i++)
		{
			packet_buf[6+i] = pbt_buf[j*FTS_PACKET_LENGTH + i];
			bt_ecc ^= packet_buf[6+i];
		}
		i2c_master_send(i2c_client, packet_buf, FTS_PACKET_LENGTH + 6);
		mdelay(50);
		if ((j * FTS_PACKET_LENGTH % 1024) == 0)
		{
			printk("[FT520X] upgrade the 0x%x th byte.\n", ((unsigned int)j) * FTS_PACKET_LENGTH);
		}
	}

	if ((dw_lenth) % FTS_PACKET_LENGTH > 0)
	{
		temp = packet_number * FTS_PACKET_LENGTH;
		packet_buf[2] = (u8)(temp>>8);
		packet_buf[3] = (u8)temp;

		temp = (dw_lenth) % FTS_PACKET_LENGTH;
		packet_buf[4] = (u8)(temp>>8);
		packet_buf[5] = (u8)temp;

		for (i=0;i<temp;i++)
		{
			packet_buf[6+i] = pbt_buf[ packet_number*FTS_PACKET_LENGTH + i]; 
			bt_ecc ^= packet_buf[6+i];
		}
		i2c_master_send(i2c_client, packet_buf, temp+6);
		mdelay(30);
	}

	//send the last six byte
	for (i = 0; i<6; i++)
	{
		packet_buf[0] = 0xbf;
		packet_buf[1] = 0x00;
		temp = 0x6ffa + i;
		packet_buf[2] = (u8)(temp>>8);
		packet_buf[3] = (u8)temp;
		temp =1;
		packet_buf[4] = (u8)(temp>>8);
		packet_buf[5] = (u8)temp;
		packet_buf[6] = pbt_buf[ dw_lenth + i]; 
		bt_ecc ^= packet_buf[6];

		i2c_master_send(i2c_client, packet_buf, 7);    

		mdelay(40);
	}

	/*********Step 6: read out checksum***********************/
	/*send the operation head*/
	cmd_write(0xcc,0x00,0x00,0x00,1);
	i2c_client->addr = i2c_client->addr & I2C_MASK_FLAG;
	i2c_master_recv(i2c_client, &reg_val, 1);
	printk("[FT520X] Step 6:  ecc read 0x%x, new firmware 0x%x. \n", reg_val[0], bt_ecc);
	if(reg_val[0] != bt_ecc)
	{
		printk("5 check sum error!!\n");
		return 5;
	}

	/*********Step 7: reset the new FW***********************/
	cmd_write(0x07,0x00,0x00,0x00,1);
	printk("[FT520X] Step 7: reset the new FW. \n");
	
	/*********Step 8: calibration TP ***********************/
	mdelay(300);          //—” ±100ms

	return 0;
}

int fts_ctpm_auto_clb(void)
{
    unsigned char uc_temp[1];
    unsigned char i ;

    printk("[FTS] start auto CLB.\n");
    msleep(200);
    write_reg(0, 0x40);  
    mdelay(100);   //make sure already enter factory mode
    write_reg(2, 0x4);  //write command to start calibration
    mdelay(300);
    for(i=0;i<100;i++)
    {
        read_reg(0,uc_temp);
        if ( ((uc_temp[0]&0x70)>>4) == 0x0)  //return to normal mode, calibration finish
        {
            break;
        }
        mdelay(200);
        printk("[FTS] waiting calibration %d\n",i);
        
    }
    printk("[FTS] calibration OK.\n");
    
    msleep(300);
    write_reg(0, 0x40);  //goto factory mode
    mdelay(100);   //make sure already enter factory mode
    write_reg(2, 0x5);  //store CLB result
    mdelay(300);
    write_reg(0, 0x0); //return to normal mode 
    msleep(300);
    printk("[FTS] store CLB result OK.\n");
    return 0;
}

int fts_ctpm_fw_upgrade_with_i_file(unsigned char *buf, long unsigned int  length)
{
	unsigned char*     pbt_buf = NULL;
	int i_ret;
	//=========FW upgrade========================*/
	pbt_buf = buf;
	/*call the upgrade function*/
	i_ret =  fts_ctpm_fw_upgrade(pbt_buf, length);
	if (i_ret != 0)
	{
		printk("[FTS] upgrade failed i_ret = %d.\n", i_ret);
	}
 	  else
	{
  		printk("[FTS] upgrade successfully.\n");
   		fts_ctpm_auto_clb();  //start auto CLB
	 }

   	return i_ret;
}

unsigned char fts_ctpm_get_upg_ver(unsigned char* buff, unsigned long int length)
{
//	unsigned int ui_sz;
//	ui_sz = sizeof(length);
	if (length > 2)
	{
		return buff[length - 2];
	}
	else
	{
		//TBD, error handling?
		return 0xff; //default value
	}
}


//return factory ID
unsigned char fts_ctpm_get_panel_factory_setting(void)
{
    unsigned char uc_i2c_addr;             //I2C slave address (8 bit address)
    unsigned char uc_io_voltage;           //IO Voltage 0---3.3v;	1----1.8v
    unsigned char uc_panel_factory_id;     //TP panel factory ID

    unsigned char buf[128];
    unsigned char reg_val[2] = {0};
    unsigned char  auc_i2c_write_buf[10];
    unsigned char  packet_buf[128 + 6];
    int i = 0;
    int      i_ret;

    uc_i2c_addr = 0x70;
    uc_io_voltage = 0x0;
    uc_panel_factory_id = 0x5a;

    /*********Step 1:Reset  CTPM *****/
    /*write 0xaa to register 0xfc*/
    write_reg(0xfc,0xaa);
    mdelay(50);
     /*write 0x55 to register 0xfc*/
    write_reg(0xfc,0x55);
    printk("[FTS] Step 1: Reset CTPM test\n");
   
    mdelay(30);   

    /*********Step 2:Enter upgrade mode *****/
    auc_i2c_write_buf[0] = 0x55;
    auc_i2c_write_buf[1] = 0xaa;
    do
    {
		i ++;
	//	i_ret = ft5306_i2c_txdata(auc_i2c_write_buf, 2);
		i_ret = i2c_master_send(i2c_client, auc_i2c_write_buf, 2);
		mdelay(5);
	}while(i_ret <= 0 && i < 5 );
	printk("[FT520X] Step 2: Enter upgrade mode\n");
	
    /*********Step 3:check READ-ID***********************/        
    cmd_write(0x90,0x00,0x00,0x00,4);
	i2c_client->addr = i2c_client->addr & I2C_MASK_FLAG;
	i2c_master_recv(i2c_client, &reg_val, 2);
	printk("[FT520X] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
	//byte_read(reg_val,2);
	if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
	{
		printk("[FT520X] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
	}
	else
	{
		return 2;
		//i_is_new_protocol = 1;
	}

    //cmd_write(0xcd,0x0,0x00,0x00,1);
    //byte_read(reg_val,1);
    //printk("bootloader version = 0x%x\n", reg_val[0]);

    /* --------- read current project setting  ---------- */
    //set read start address
    buf[0] = 0x3;
    buf[1] = 0x0;
    buf[2] = 0x78;
    buf[3] = 0x0;
  //  i2c_master_send(i2c_client, &buf[0], 4);
    cmd_write(0x3,0x0,0x78,0x0,4);
    i2c_client->addr = i2c_client->addr & I2C_MASK_FLAG;
    i2c_master_recv(i2c_client, &buf, 8);

    printk("[FTS] old setting: uc_i2c_addr = 0x%x, uc_io_voltage = %d, uc_panel_factory_id = 0x%x\n",
        buf[0],  buf[2], buf[4]);
   
    /********* reset the new FW***********************/
    cmd_write(0x07,0x00,0x00,0x00,1);

    msleep(200);

    return buf[4];
    
}

#endif

 
 static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id)
 {	 
	int retval = TPD_OK;
	char data;
	u8 report_rate=0;
	int err=0;
	int reset_count = 0;
	unsigned char i;
#if TP_UPGRADE
	unsigned char bufff[8] = {0}, buff[8] = {0};
#endif

reset_proc:   
	i2c_client = client;

	printk("\nxxxxxxxxxxxxxxxxx%sxxxxxxxxxxxxxxxxxx\n", __FUNCTION__);

//WK pin high
	mt_set_gpio_mode(GPIO53, 0);
	mt_set_gpio_dir(GPIO53, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO53, GPIO_OUT_ONE);

//RST pin low
	mt_set_gpio_mode(GPIO54, 0);
	mt_set_gpio_dir(GPIO54, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO54, GPIO_OUT_ZERO);

//power on, need confirm with SA
	hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_3000, "TP");

	mdelay(1);
//RST pin to high
	mt_set_gpio_out(GPIO54, GPIO_OUT_ONE);

//INT pin
	mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
	mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);

	mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
	mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
	mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1); 
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);

//200ms is needed
	mdelay(200);
 
	if((i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 1, &data))< 0)
	{
		TPD_DMESG("I2C transfer error, line: %d\n", __LINE__);
#ifdef TPD_RESET_ISSUE_WORKAROUND
        if ( reset_count < TPD_MAX_RESET_COUNT )
        {
            reset_count++;
            goto reset_proc;
        }
#endif
		   return -1; 
	}

	//set report rate 80Hz
	report_rate = 0x8; 
	if((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
	{
	    if((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
	    {
		   TPD_DMESG("I2C read report rate error, line: %d\n", __LINE__);
	    }
		   
	}


//updating
#if TP_UPGRADE

	for(i=0; i<5; i++){
		buff[0] = fts_ctpm_get_panel_factory_setting();
		if((buff[0]==0x53)||(buff[0]==0x85))
			break;
	}
	read_reg(0xa6, bufff);		//firmware ID(0x??)
	printk("xxxxx%s's firmware version is 0x%2x xxxxxxxxxxxx\n", buff[0]==0x53?version[0] : version[1], bufff[0]);


	if(buff[0] == 0x53)				//for mud
	{
		if((bufff[0] == 0xa6)||(fts_ctpm_get_upg_ver(CTPM_FW_MUD, sizeof(CTPM_FW_MUD)) > bufff[0])){
			printk("Different version, we need version 0x%2x, while now is 0x%2x\n", 
								fts_ctpm_get_upg_ver(CTPM_FW_MUD, sizeof(CTPM_FW_MUD)), bufff[0]);
			printk("updating MUD!!!!\n");
			fts_ctpm_fw_upgrade_with_i_file(CTPM_FW_MUD, sizeof(CTPM_FW_MUD));
			if(read_reg(0xa6,bufff)<0){
			printk(KERN_ERR"tp upgrade error!\n");
			}
		printk("xxxxxxxxxMUD: after upgrade : new firmware is %2xxxxxxxxxxx\n", bufff[0]);
		}
	}else if(buff[0] == 0x85){		//for junda
		if((bufff[0] == 0xa6)||(fts_ctpm_get_upg_ver(CTPM_FW_JUNDA, sizeof(CTPM_FW_JUNDA)) > bufff[0])){
			printk("Different version, we need version 0x%2x, while now is 0x%2x\n", 
								fts_ctpm_get_upg_ver(CTPM_FW_JUNDA, sizeof(CTPM_FW_JUNDA)), bufff[0]);
			printk("updating Junda!!!!\n");
			fts_ctpm_fw_upgrade_with_i_file(CTPM_FW_JUNDA, sizeof(CTPM_FW_JUNDA));
			if(read_reg(0xa6,bufff)<0){
			printk(KERN_ERR"tp upgrade error!\n");
			}
		printk("xxxxxxxxxJunda: after upgrade : new firmware is %2xxxxxxxxxxx\n", bufff[0]);
		}
	}
#endif

	tpd_load_status = 1;

	#ifdef VELOCITY_CUSTOM_FT5206
	if((err = misc_register(&tpd_misc_device)))
	{
		printk("mtk_tpd: tpd_misc_device register failed\n");
		
	}
	#endif

	thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
	 if (IS_ERR(thread))
		 { 
		  retval = PTR_ERR(thread);
		  TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", retval);
		}

	TPD_DMESG("ft5206 Touch Panel Device Probe %s\n", (retval < TPD_OK) ? "FAIL" : "PASS");
   return 0;
   
 }

 static int __devexit tpd_remove(struct i2c_client *client)
 
 {
   
	 TPD_DEBUG("TPD removed\n");
 
   return 0;
 }
 
 
 static int tpd_local_init(void)
 {

 
  TPD_DMESG("Focaltech FT5206 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);
 
 
   if(i2c_add_driver(&tpd_i2c_driver)!=0)
   	{
  		TPD_DMESG("ft5206 unable to add i2c driver.\n");
      	return -1;
    }
    if(tpd_load_status == 0) 
    {
    	TPD_DMESG("ft5206 add error touch panel driver.\n");
    	i2c_del_driver(&tpd_i2c_driver);
    	return -1;
    }
	
#ifdef TPD_HAVE_BUTTON     
    tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif   
  
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))    
    TPD_DO_WARP = 1;
    memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
    memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif 

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
    memcpy(tpd_calmat, tpd_def_calmat_local, 8*4);
    memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);	
#endif  
		TPD_DMESG("end %s, %d\n", __FUNCTION__, __LINE__);  
		tpd_type_cap = 1;
    return 0; 
 }

 static int tpd_resume(struct i2c_client *client)
 {
	printk("xxxxxxxxxTPD wake upxxxxxxxxxxxx\n");

//	hwPowerOn(MT65XX_POWER_LDO_VGP2, VOL_3000, "TP");

//rst pin
	mt_set_gpio_mode(GPIO54, 0);
	mt_set_gpio_dir(GPIO54, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO54, GPIO_OUT_ONE);
	mt_set_gpio_out(GPIO54, GPIO_OUT_ZERO);
	mdelay(5);
        mt_set_gpio_out(GPIO54, GPIO_OUT_ONE);
	mdelay(200);

#if 0
#ifdef TPD_CLOSE_POWER_IN_SLEEP	
	hwPowerOn(TPD_POWER_SOURCE,VOL_3300,"TP"); 
#else
#ifdef MT6573
	mt_set_gpio_mode(GPIO_CTP_EN_PIN, GPIO_CTP_EN_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_EN_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_EN_PIN, GPIO_OUT_ONE);
#endif	
	msleep(100);

	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);  
	msleep(1);  
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#endif
#endif
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);  

	return 0;
 }
 
 static int tpd_suspend(struct i2c_client *client, pm_message_t message)
 {
	printk("xxxxxxxxxxxx %s xxxxxxxxxxxxxx\n", __FUNCTION__);
	unsigned char data = 0x03;
	mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
	i2c_smbus_write_i2c_block_data(i2c_client, 0xA5, 1, &data);  //TP enter sleep mode
//	hwPowerDown(MT65XX_POWER_LDO_VGP2, "TP");

	return 0;
 } 


 static struct tpd_driver_t tpd_device_driver = {
		 .tpd_device_name = "FT5206",
		 .tpd_local_init = tpd_local_init,
		 .suspend = tpd_suspend,
		 .resume = tpd_resume,
#ifdef TPD_HAVE_BUTTON
		 .tpd_have_button = 1,
#else
		 .tpd_have_button = 0,
#endif		
 };
 /* called when loaded into kernel */
 static int __init tpd_driver_init(void) {
	 printk("MediaTek FT5206 touch panel driver init\n");
	   i2c_register_board_info(0, &ft5206_i2c_tpd, 1);
		 if(tpd_driver_add(&tpd_device_driver) < 0)
			 TPD_DMESG("add FT5206 driver failed\n");
	 return 0;
 }
 
 /* should never be called */
 static void __exit tpd_driver_exit(void) {
	 TPD_DMESG("MediaTek FT5206 touch panel driver exit\n");
	 //input_unregister_device(tpd->dev);
	 tpd_driver_remove(&tpd_device_driver);
 }
 
 module_init(tpd_driver_init);
 module_exit(tpd_driver_exit);


