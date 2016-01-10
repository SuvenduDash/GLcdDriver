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
