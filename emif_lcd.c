/******************************TASK HEADER*********************************************
 * DRIVER NAME          :       $Name:emif_lcd $
 * FUNCTION                     :   Driver code for interfacing EMIF controller for WG128X64 Display
 * CHIP SELECT USED     :       cs4
 * STARTING ADDRESS     :       0x65000000
 * DATA BUS WIDTH       :   8 bit width
 * AUTHOR                       : Suvendu Kumar Dash
 ***************************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/lcd-dev.h>
#include <asm/uaccess.h>
#include "fproomap_emif.h"

#define DRV_NAME                        "emif_lcd"
#define DEV_NAME                        "G_LCD"
#define DRV_VERSION                     "0.01"

static dev_t lcd_devt;
struct cdev *lcddev ;
struct class *lcd_class;
struct device *mydevice;
static const struct file_operations lcd_dev_fops = {
        .owner          = THIS_MODULE,
        .unlocked_ioctl = lcd_dev_ioctl,
        .open           = lcd_dev_open,
        .write          = lcd_dev_write,
};

static int __init wg128x64_probe(struct platform_device *pdev)
{
        struct resource *res, *mem;
        int err;
        printk("Before Platform_get_resource\n");
        res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        printk("After platform_get_resources\n");
        if (!res) {
                pr_debug("%s: Lcd resource data missing\n", pdev->name);
                printk("Lcd resource data missing\n");
                return -ENOENT;
        }
        printk("Before request_mem_region\n");
        mem = request_mem_region(res->start, resource_size(res), pdev->name);
        printk("After request_mem_region\n");
        if (!mem) {
                pr_debug("%s: LCD registers are not free\n",
                                 pdev->name);
                printk("Lcd registers are not free\n");
                return -EBUSY;
        }
        printk("before emif_init\n");
        emif_init(res->start);
        printk("after emif_init\n");
        /*Create a struct class structure*/
#if 1
        lcd_class = class_create(THIS_MODULE, DEV_NAME);
        printk("after class_creae\n");
        if (IS_ERR(lcd_class)) {
                printk(KERN_ERR "%s: couldn't create class\n", __FILE__);
                printk("Couldn't create class\n");
                return PTR_ERR(lcd_class);
 }
        /*allocate a cdev structure*/
        printk("before cdev_allc\n");
        lcddev = cdev_alloc();
        printk("after cdev_allc\n");
        err = alloc_chrdev_region(&lcd_devt, 0, 1, DEV_NAME);
        printk("after alloc_chrdev_region\n");
        if (err < 0)
        {
                printk(KERN_ERR "%s: failed to allocate char dev region\n",
                           __FILE__);
                printk("failed to allocate char dev region\n");
         }
         /*create a class device and register with sysfs*/
         printk("before device_create\n");
         mydevice=device_create(lcd_class,NULL, lcd_devt,NULL,DEV_NAME);
         printk("after device_create\n");
         if(mydevice == NULL)
         {
                printk("before class_destroy\n");
                class_destroy(lcd_class);
                printk("after class_destroy\n");
                unregister_chrdev_region(lcd_devt,1);
                printk("after unregister\n");

         }
         printk("before cdev_init\n");
         cdev_init(lcddev,&lcd_dev_fops);
         printk("after cdev_init\n");
         lcddev->owner = THIS_MODULE;
         printk("before cdev_add");
         cdev_add(lcddev, lcd_devt, 1);
         printk("after cdev_add\n");
#endif
         /*Write the code here for boot up display*/
        // register_chrdev(89,DEV_NAME,&lcd_dev_fops);
         printk("before Lcd_init\n");
         Lcd_initialize();
 printk("after Lcd_init\n");
        return(0);
}
static long lcd_dev_open(struct inode *inode, struct file *file)
{
  printk("Device opened sucessfully\n");
  return(0);
}
ssize_t lcd_dev_write(struct file *filp,const char *buf,size_t count,loff_t *offset)
{
  printk("lcd_dev_write func\n");
  int ret;
  char msg[1024];
  ret = copy_from_user(msg, buf, count);
  msg[count] = '\0';
  LCD_writeByte( LCD_INST_REG,AutoModeWrite);
  {
       LCD_writeString(msg, count, DELAY_200_MS);
       //printf("...loop forever with LCD running...\r\n");
  }
  LCD_writeByte(LCD_INST_REG, AutoModeReset );
  printk("Exit lcd_dev_write\n");
  printk("ret value = %d\n",ret);
  return(ret);
}
static long lcd_dev_ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{
        switch(cmd)
        {
                case DEV_OR_MODE :
                        LCD_writeByte( LCD_INST_REG,H_OR_MODE);
                        //printk("OR_MODE case executed\n");
                        break;
                case DEV_XOR_MODE:
                        LCD_writeByte( LCD_INST_REG,H_XOR_MODE);
                        //printk("XOR_MODE case executed\n");
                        break;
                case DEV_AND_MODE:
                        LCD_writeByte( LCD_INST_REG,H_AND_MODE);
                        //printk("AND_MODE case executed\n");
                        break;
                case DEV_DIS_OFF:
 LCD_writeByte( LCD_INST_REG,H_DIS_OFF);//DISPLAY OFF
                        //printk("DIS_OFF case executed\n");
                        break;
                case DEV_CUR_ON_BLINK_OFF:
                        LCD_writeByte( LCD_INST_REG,H_CUR_ON_BLINK_OFF);
                        //printk("CUR_ON_BLINK_OFF case executed\n");
                        break;
                case DEV_CUR_ON_BLINK_ON:
                        LCD_writeByte( LCD_INST_REG,H_CUR_ON_BLINK_ON);
                        //printk("CUR_ON_BLINK_ON case executed\n");
                        break;
                case DEV_CUR_WITH_TXT_ON_GRA_OFF:
                        LCD_writeByte( LCD_INST_REG,H_CUR_WITH_TXT_ON_GRA_OFF);
                        //printk("CUR_WITH_TXT_ON_GRA_OFF case executed\n");
                        break;
                case DEV_TXT_ON_GRA_OFF:
                        LCD_writeByte( LCD_INST_REG,H_TXT_ON_GRA_OFF);
                        //printk("TXT_ON_GRA_OFF case executed\n");
                        break;
                case DEV_TXT_OFF_GRA_ON:
                        LCD_writeByte( LCD_INST_REG,H_TXT_OFF_GRA_ON);
                        //printk("TXT_OFF_GRA_ON case executed\n");
                        break;
                case DEV_TXT_ON_GRA_ON:
                        LCD_writeByte( LCD_INST_REG,H_TXT_ON_GRA_ON);
                        //printk("TXT_ON_GRA_ON case executed\n");
                        break;
                case DEV_SET_CUR_PTR:
                        LCD_writeByte( LCD_INST_REG,H_SET_CUR_PTR);
                        //printk("SET_CUR_PTR case executed\n");
                        break;
                case DEV_SET_ADD_PTR:
                        LCD_writeByte( LCD_INST_REG,H_SET_ADD_PTR);
                        //printk("SET_ADD_PTR case executed\n");
                        break;
                case DEV_TXT_HOME_ADD:
                        LCD_writeByte( LCD_INST_REG,H_TXT_HOME_ADD);
                        //printk("TXT_HOME_ADD case executed\n");
                        break;
                case DEV_TXT_AREA:
                        LCD_writeByte( LCD_INST_REG,H_TXT_AREA);
                        //printk("TXT_AREA case executed\n");
 break;
                case DEV_GRA_HOME_ADD:
                        LCD_GRAPH_Home();
                        //printk("GRA_HOME_ADD case executed\n");
                        break;
                case DEV_GRA_AREA:
                        LCD_GRAPH_Area();
                        //printk("GRA_AREA case executed\n");
                        break;
                case DEV_SET_HOME_ADDR:
                        LCD_TextHome();
                        //printk("LCD_TextHome case executed\n");
                        break;
                case DEV_SET_HOME_AREA:
                        LCD_TextArea();
                        //printk("LCD_TextArea case executed\n");
                        break;
                case DEV_AUTOMODEWRITE:
                        LCD_writeByte( LCD_INST_REG,H_AUTOMODEWRITE);
                        //printf("DEV_AUTOMODEWRITE case executed\n");
                        break;
                case DEV_AUTOMODERESET:
                        LCD_writeByte( LCD_INST_REG,H_AUTOMODERESET);
                        //printk("DEV_AUTOMODERESET case executed\n");
                        break;
                case DEV_CURSOR_OFF://T MUST CHECK NOW
                        LCD_writeByte( LCD_INST_REG,0X18);
                        LCD_writeByte( LCD_INST_REG,0X00);
                        //printk("DEV_CURSOR_OFF case executed\n");
                        break;
                case DEV_GRA_HOME_LENGTH:
                        LCD_writeByte( LCD_INST_REG,H_GRA_HOME_ADD);
                        //printk("DEV_GRA_HOME_LENGTH case executed\n");
                        break;
                case DEV_GRA_AREA_LENGTH:
                        LCD_writeByte( LCD_INST_REG,H_GRA_AREA);
                        //printk("DEV_GRA_AREA_LENGTH case executed\n");
                        break;
                case DEV_GRA_AREA_16:
                        LCD_writeByte(LCD_DATA_REG,0x10);
                        LCD_writeByte(LCD_DATA_REG,0x00);
						//printk("DEV_GRA_AREA_16 case executed\n");
                        break;
                case DEV_TXT_AREA_16:
                        LCD_writeByte(LCD_DATA_REG,0x10);
                        LCD_writeByte(LCD_DATA_REG,0x00);
                        //printk("DEV_TXT_AREA_16 case executed\n");
                        break;
                case DEV_FONT_CHANGE_16:
                        LCD_font_Change_16();
                        //printk("FONT_CHANGE_16 case executed\n");
                        break;
                case DEV_FONT_CHANGE_21:
                        LCD_font_Change_21();
                        printk("FONT_CHANGE_21 case executed\n");
                        break;
                case DEV_SET_HOME_ADDR_8_8:
                        LCD_writeByte(LCD_DATA_REG,0x00);
                        LCD_writeByte(LCD_DATA_REG,0x40);
                        //printk("DEV_SET_HOME_ADDR_8_8 case executed\n");
                        break;
                default:
                        return(ENOSYS);
        }
       return(0);

}
