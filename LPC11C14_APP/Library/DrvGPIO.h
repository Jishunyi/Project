/****************************************************************************
 *   $Id:: gpio.h                       $
 *   Project: NXP LPC11xx software example
 *
 *   Description:
 *     This file contains definition and prototype for GPIO.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#ifndef __GPIO_H 
#define __GPIO_H

#include "lpc11xx.h"
#include "system_LPC11xx.h"
#include "type.h"
#include "string.h"	
#include "stdio.h" 
#include "stdlib.h"
#include "DrvSysClk.h"
					
#define GPIO_INPORT		0
#define GPIO_OUTPORT	1

#define PORT0			0
#define PORT1			1
#define PORT2			2
#define PORT3			3

static LPC_GPIO_TypeDef (* const LPC_GPIO[4]) = { LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3 };

void GPIOSetValue( uint32 portNum, uint32 bitPosi, uint32 bitVal );
void GPIOSetDir( uint32 portNum, uint32 bitPosi, uint32 dir );
uint32 GPIOGetValue( uint32 portNum, uint32 bitPosi );

//-----------------  LED  ------------------------------------------------------------------------------

#define LED_INIT	LPC_IOCON->PIO1_10 &= ~(0x07);	LPC_GPIO1->DIR |= (1 << 10)
#define LED_ON		LPC_GPIO1->DATA &= ~(1 << 10)
#define LED_OFF   LPC_GPIO1->DATA |= (1 << 10)

void SetLedBinkFrequency(unsigned int pInterval);
void LoopForLedBlink(void);

#endif /* end __GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
