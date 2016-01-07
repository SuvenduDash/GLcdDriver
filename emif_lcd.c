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
