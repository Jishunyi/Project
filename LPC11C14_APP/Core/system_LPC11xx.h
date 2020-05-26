/******************************************************************************
 * @file:    system_LPC11xx.h
 * @purpose: CMSIS Cortex-M0 Device Peripheral Access Layer Header File
 *           for the NXP LPC11xx Device Series 
 * @version: V1.1
 * @date:    29. Jan. 2011
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2008 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M0 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef __SYSTEM_LPC11xx_H
#define __SYSTEM_LPC11xx_H

/* Vector Table Base ---------------------------------------------------------*/
#define NVIC_VectTab_RAM             (0x10000000)
#define NVIC_VectTab_FLASH           (0x00000000)

#include "type.h"

extern uint32 uClockSource;

#define IRC_CLOCK					0
#define OSC_CLOCK					1	

//PDRUNCFG register
#define IRCOUT_POWERDOWN	  0
#define IRC_POWERDOWN		  1
#define FLASH_POWERDOWN		  2
#define BOD_POWERDOWN		  3
#define ADC_POWERDOWN		  4
#define SYSOSC_POWERDOWN	  5
#define WDTOSC_POWERDOWN	  6
#define SYSPLL_POWERDOWN	  7


//SYSPLLCLKSEL register//MAINCLKSEL register
#define IRC_OSCILLATOR		  0x00000000
#define SYSTEM_OSCILLATOR     0x00000001
//MAINCLKSEL register
#define WDT_OSCILLATOR		  0x00000002
#define PLL_CLOCK_OUT		  0x00000003

#define MAIN_PLL_M_Val        0x00000001
#define MAIN_PLL_P_Val        0x00000002

//SYSAHBCLKCTRL register
#define SYS					  0
//#define ROM					  1
#define RAM					  2
#define FLASHREG			  3
#define FLASHARRAY			  4
#define I2C					  5
#define GPIO				  6
#define CT16B0				  7
#define CT16B1				  8
#define CT32B0				  9
#define CT32B1				  10
#define SSP0				  11
#define UART				  12
#define ADC					  13
#define WDT  				  15
#define IOCON				  16
//#define CAN 				  17
#define SSP1				  18

//#define __JTAG_DISABLED

#define JTAG_TDI_PIOPORT		0
#define JTAG_TDI_PIOBIT			11

#define JTAG_TMS_PIOPORT		1
#define JTAG_TMS_PIOBIT			0

#define JTAG_TDO_PIOPORT		1
#define JTAG_TDO_PIOBIT			1

#define JTAG_nTRST_PIOPORT		1
#define JTAG_nTRST_PIOBIT		2

extern void SystemInit (uint32 ulOcx, uint32 uTargetfrequency);

extern uint32 uSystemFrequency;
extern uint32 uSystemAHBFrequency;

#endif
