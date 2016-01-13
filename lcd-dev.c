/*
 * lcd subsystem, dev interface
 * Author: Suvendu Kumar Dash
*/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/lcd-dev.h>
#include <mach/gpio.h>
#include "fproomap_emif.h"

#define DA850_LCD_RESET_PIN	GPIO_TO_PIN(0,1)
#define DA850_LCD_MODE_PIN	GPIO_TO_PIN(7,12)
#define DA850_LCD_FS_PIN        GPIO_TO_PIN(7,13)

unsigned int Gpio_init(void)
{
  int status;
  printk("Inside Gpio_init\n");
  status = gpio_request(DA850_LCD_RESET_PIN, "lcd_reset\n");
  if(status <0)
      printk("\n Error in gpio request for lcd_reset");
  status = gpio_request(DA850_LCD_MODE_PIN, "lcd_mode\n");
  if(status<0)
	   printk("\n Error in gpio request for lcd_mode\n");
  status = gpio_request(DA850_LCD_FS_PIN, "lcd_fs\n");
  if(status<0)
	   printk("\n Error in gpio request for lcd_fs\n");
  gpio_direction_output(DA850_LCD_RESET_PIN, 0);
  gpio_direction_output(DA850_LCD_MODE_PIN,  0);
  gpio_direction_output(DA850_LCD_FS_PIN, 0);
  printk("After Gpio_direction\n");
  LCD_writeByte(LCD_INST_REG, 0x00);
  return(0);
	
}
unsigned int LCD_Test(void)
{
	unsigned char low_byte1;
	unsigned char x, y;
	int i=1;
	//write text home address=0000h
	LCD_writeByte(LCD_DATA_REG, 0x00);
	LCD_writeByte(LCD_DATA_REG, 0x00);
	LCD_writeByte(LCD_INST_REG , 0x40);
	//write text area address
	low_byte1	=128 /8;
	LCD_writeByte(LCD_DATA_REG, low_byte1);
	LCD_writeByte(LCD_DATA_REG, 0x00);
	LCD_writeByte(LCD_INST_REG , 0x41);
	//set display mode Display mode set (Graphic only enable)
	LCD_writeByte(LCD_INST_REG , 0x80);
	LCD_writeByte(LCD_INST_REG , 0x94);
	
	
	//set character on graphics off
	LCD_writeByte(LCD_INST_REG , 0x94);
	LCD_writeByte(LCD_DATA_REG, 0x00);
	LCD_writeByte(LCD_DATA_REG, 0x00);
	//set address pointer
	LCD_writeByte(LCD_INST_REG , 0x24);
	//set auto write
	LCD_writeByte(LCD_INST_REG , 0xB0);
    //lcd_write(LCD_DATA_REG, 0x21);
	for (y=0; y<64/8;y++)
	{
		for (x=0; x<128/8;x++)
		{
	
		    LCD_writeByte(LCD_DATA_REG, i);
	
			if(i<128)
			{
				i++;
			}
		    else
			{
		    	i=33;
		    }
	
		}
	}
	
	//set auto write reset
	LCD_writeByte(LCD_INST_REG , 0xB2);
        return(0);
	
}

unsigned int Lcd_initialize(void)
{
        printk("Inside Lcd_init before Gpio_init()\n");
  	Gpio_init();
        printk("Aftre Lcd_init \n");
	/*LCD reset*/
	gpio_set_value(DA850_LCD_RESET_PIN, OUTPUT_LOW);
        printk("before Emif_bus delay\n");
	udelay(1000);
        printk("after Emif_bus_delay\n");
	gpio_set_value(DA850_LCD_RESET_PIN, OUTPUT_HIGH);
	udelay(1000);
        printk("after Emif_bus_delay_gpio_set_value\n");
	/*Mode*/
	gpio_set_value(DA850_LCD_MODE_PIN, OUTPUT_HIGH);
	/*Font Select*/
	gpio_set_value(DA850_LCD_FS_PIN,   OUTPUT_LOW);
        printk("Before Lcd_test\n");
        LCD_Test();
        /*while(1)
        {
          gpio_set_value(DA850_LCD_MODE_PIN, OUTPUT_LOW);
          mdelay(1000);
          gpio_set_value(DA850_LCD_MODE_PIN, OUTPUT_HIGH);
          mdelay(1000);
        }*/
        printk("Aftre Lcd_test\n");
        return(0);
        
}
unsigned int LCD_writeString(uint8_t* Buf, uint16_t bufferLen, uint32_t charDelay)
{
	uint16_t bufferIndex;

	if(bufferLen<=LCD_BUF_LENGTH)
	{
		for (bufferIndex = NO_ZERO; bufferIndex < bufferLen+1; bufferIndex++)
		{
			LCD_writeByte(LCD_DATA_REG, Buf[bufferIndex] - 0x20);
		}
		return (ERR_NO_ERROR);
	}

	else
	{
		for (bufferIndex = 0; bufferIndex < bufferLen+1; bufferIndex++)
		{
			LCD_writeByte(LCD_DATA_REG, Buf[bufferIndex]);
		}
	}
 return(0);
}
unsigned int LCD_TextHome(void)
{
	LCD_writeByte(LCD_DATA_REG,0x40);
	LCD_writeByte(LCD_DATA_REG,0x05);
        return(0);
}

unsigned int LCD_TextArea(void)
{
	LCD_writeByte(LCD_DATA_REG,0x15);
	LCD_writeByte(LCD_DATA_REG,0x00);
        return(0);
}

unsigned int LCD_GRAPH_Home(void)
{
	LCD_writeByte(LCD_DATA_REG,0x00);
	LCD_writeByte(LCD_DATA_REG,0x00);
        return(0);

}

unsigned int LCD_GRAPH_Area(void)
{
	LCD_writeByte(LCD_DATA_REG,0x15);
	LCD_writeByte(LCD_DATA_REG,0x00);
        return(0);
}

unsigned int LCD_font_Change_16(void)
{
	gpio_set_value(DA850_LCD_FS_PIN,   OUTPUT_LOW);
        return(0);
}

unsigned int LCD_font_Change_21(void)
{
	gpio_set_value(DA850_LCD_FS_PIN,   OUTPUT_HIGH);
        return(0);
}
