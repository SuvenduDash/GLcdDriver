/******************************TASK HEADER**********************************
 * DRIVER NAME		:	$Name:EMIF_A $
 * FUNCTION			:   Driver code for interfacing EMIF controller for LCD&LED
 * CHIP SELECT USED	: 	cs4
 * STARTING ADDRESS	: 	0x64000000
 * DATA BUS WIDTH	:   8 bit width
  * AUTHOR			:	$Author: Suvendu Kumar Dash $
 ****************************************************************************/
#if 0
/*-----------------------------------------------------------------------------------------
 *  						LCD Data/Command With EMIF								      *
 * EMA_7	Not Used																	  *
 *  _C/D_______________________________________________________________________           *
 * |	 |		|		|															      *
 * |EMA_1|	Description										                              *
 * |_____|_____	|_______|_______________________________________________________	      *
 *           								                      				          *									|	      *
 * |0	 |With this combination decoder selects the CS of LCD and 	     				  *
 * |	 |Data register of LCD is selected (0x60)					      				  *
 * |	 															      				  *
 * |1	 |With this combination decoder selects the CS of LCD and	      				  *
 * |      Command register of LCD is selected(0x61)              	      				  *
 * |	 															     				  *
 * _____________________________________________________________________________	      *
 *  																					  *
 *----------------------------------------------------------------------------------------*/
/******************************************************************************
*						EMIF-A WORKING METHODOLOGY
*		ptr is pointing to 0x60000000
*		*ptr = anydata;
*		ie.on address bus a0 to a21 all are zero
*		anydata will be on databus.
*
*		*(ptr+1) = anydata
*		ptr+1 is pointing 0x60000001
*		i.e. on address bus  a0 is 1 and all others are zero
*		anydata will be on databus.
*
*		*(ptr+32) = anydata;
*		ptr+32 is pointing 0x60000020
*		i.e. on address bus  a5 is 1 and all others are zero
*		anydata will be on databus.
*
*		*(ptr+64) = anydata;
*		ptr+64 is pointing 0x60000040
*		i.e. on address bus  a6 is 1 and all others are zero
*		anydata will be on databus.
*
*		*(ptr+128) = anydata;
*		ptr+128 is pointing 0x60000080
*		i.e. on address bus  a7 is 1 and all others are zero
*		anydata will be on databus.
**********************************************************************************/
#endif

#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include "fproomap_emif.h"

struct chipio
{
	unsigned		io_base;			// Physical Address
	unsigned		*io_data;			// Virtual Address or Logical Process Address of Physical Address
	unsigned	    *io_emifa;			// Virtual Address of EMIF-A Controller's Control Register
	unsigned	 	cs;					// Chip Select number of EMIF-A
} chipio;
struct chipio	*cio ;
int emif_init(unsigned base_address)
{
	unsigned int	emifa_ancr ;
	cio = (struct chipio*)kmalloc(sizeof(chipio), GFP_KERNEL);
	cio->io_base = base_address;
	cio->cs = OMAPL1xx_EMIFA_CS4;
	/*Map in the device Address for Chip Select 2*/
	cio->io_data = (unsigned int *)ioremap(cio->io_base,OMAPL1xx_EMIFA_CS4_SIZE);

	if (!cio->io_data)
	{
		printk("\n Returning Null address in emif_init");
		return (-1);
	}
#if 0
*************************************************************************************
*        VALUE CONFIGURED FOR CENCFG REGISTER ASSIGNED IN OMAPL1xx_EMIFA_AnCR_FASCIA_VAL
* bit    	31		Select strobe          	- 0 		(normal)
* bit    	30		Extended               	- 0 		(disabled)
* bit 		29-26	Write setup width   	- 0001  	(10 ns)
* bit 		25-20 	Write strobe width    	- 000011	(40ns)
* bit 		19-17	Write hold width   		- 001 		(10ns)
* bit 		16-13	Read setup width   		- 0001 		(10ns)
* bit  		12-7  	Read strobe width    	- 000101 	(60ns)
* bit   	6-4  	Read hold with         	- 001 		(10ns)
* bit   	3-2  	Turn around            	- 11
* bit   	1-0  	Async size             	- 00
**************************************************************************************
#endif	
}
/******************************************************************************
* FUNC NAME:	LCD_writeByte
* ACTION   :	writing data or Command to the LCD
* RETURNS  : 	ERR_NO_ERROR
* AUTHOR   : 	Suvendu Kumar Dash
* REVISION :	0.01
******************************************************************************/
unsigned int LCD_writeByte(unsigned int registerDest, volatile unsigned int dataToWrite)
{
       volatile	char *cmd_dat = NULL;
       volatile	unsigned int	set_val;
	dataToWrite&=	MASK_VAL;
	set_val=dataToWrite;
	if (registerDest == LCD_INST_REG)
	{
		//EMA_1 = 1(c/d=1)
		cmd_dat	=( char*)(cio->io_data + LCD_COMMAND_OFFSET);
	}
	else if (registerDest == LCD_DATA_REG)
	{
		//EMA_1 = 0(c/d=0)
		 cmd_dat=( char*)(cio->io_data + LCD_DATA_OFFSET);
	}
	//Emif_busdelay(); 
         udelay(500);
	*cmd_dat=set_val;
	//Emif_busdelay();
         udelay(500);
	return (ERR_NO_ERROR);
}
void Emif_busdelay(void)
{
	int i;
	for(i=0;i<200;i++)
	{

	}
}
