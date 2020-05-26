/******************************************************************************
 * @file:    system_LPC11xx.c
 * @purpose: CMSIS Cortex-M0 Device Peripheral Access Layer Source File
 *           for the NXP LPC11xx Device Series 
 *			 
 * @version: V0.1.0	(It refer to nxp official program with the same name, version v1.1)
 * @date:    12. Feb. 2011
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2008 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3 
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
#include <stdint.h>
#include "LPC11xx.h"
#include "system_LPC11xx.h"

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *------------------------------------------------------------------------------------------*/
#define IRC_OSC     (12000000UL)        /* Internal RC oscillator frequency   				*/
#define MAX_OSC	    (100000000UL)		/* The max oscillator frequency this chip can use   */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32 uClockSource = IRC_OSC;
uint32 uSystemFrequency = IRC_OSC; /*!< System Clock Frequency (Core Clock)  */
uint32 uSystemAHBFrequency = IRC_OSC;

int32 Main_PLL_Setup (int32 bPLL_M, int32 oscillator)
{
  uint32 regVal;
  
  if(uClockSource*bPLL_M > MAX_OSC)	
		bPLL_M = MAX_OSC / uClockSource;
  bPLL_M = bPLL_M - 1;
  if(bPLL_M < 0)	
		bPLL_M = 0;

  //LPC_SYSCON->SYSPLLCLKSEL = SYSTEM_OSCILLATOR;   //Select system OSC
  LPC_SYSCON->SYSPLLCLKSEL = oscillator;
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;                 /* Update clock source */
  LPC_SYSCON->SYSPLLCLKUEN = 0x00;                 /* toggle Update register once */
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;
  while ( !(LPC_SYSCON->SYSPLLCLKUEN & 0x01) ); /* Wait until updated */

  regVal = LPC_SYSCON->SYSPLLCTRL;
  regVal &= ~0x1FF;
  LPC_SYSCON->SYSPLLCTRL = (regVal | (MAIN_PLL_P_Val<<5) | bPLL_M);
  /* Enable main system PLL, main system PLL bit 7 in PDRUNCFG. */
  LPC_SYSCON->PDRUNCFG &= ~(0x1<<SYSPLL_POWERDOWN);
  while ( !(LPC_SYSCON->SYSPLLSTAT & 0x01) );	/* Wait until it's locked */

  LPC_SYSCON->MAINCLKSEL = PLL_CLOCK_OUT;		/* Select PLL clock output */
  LPC_SYSCON->MAINCLKUEN = 0x01;		/* Update MCLK clock source */
  LPC_SYSCON->MAINCLKUEN = 0x00;		/* Toggle update register once */
  LPC_SYSCON->MAINCLKUEN = 0x01;
  while ( !(LPC_SYSCON->MAINCLKUEN & 0x01) );	/* Wait until updated */

  return bPLL_M;
}

void SystemInit (uint32 ulOcx, uint32 uTargetfrequency)
{
  uint32 i;
	int nRealPLL_M = 0;
	int nAHB_DIV = 1;
	int8 bClockSource = TRUE;
	int8 bPLLFlag = TRUE;	
	int8 nPLL_M;
	int32 oscillator;

	if(ulOcx != 0)//外部时钟
	{
		nPLL_M = (int8)(uTargetfrequency/ulOcx);
		uClockSource = ulOcx;
		oscillator = SYSTEM_OSCILLATOR;
	}
	else if(ulOcx == 0)//内部时钟
	{
		nPLL_M = (int8)(uTargetfrequency/IRC_OSC);
		uClockSource = IRC_OSC;
		oscillator = IRC_OSCILLATOR;
	}
		
	if( nAHB_DIV <= 0 ) 
		nAHB_DIV = 1; 
	//LPC_SYSCON->SYSMEMREMAP = 0x2;		//remap to internal flash ,如果使用IAP方案，必须映射到RAM中
	if ( bClockSource == OSC_CLOCK )           // Clock Setup 
	{
  		// bit 0 default is crystal bypass, 
  		// bit1 0=0~20Mhz crystal input, 1=15~50Mhz crystal input.
		if(ulOcx <= 20000000UL)
			LPC_SYSCON->SYSOSCCTRL = 0x00;
		else if((ulOcx <= 50000000UL)&&(ulOcx > 20000000UL))
		{
			LPC_SYSCON->SYSOSCCTRL = 0x01;
		}
  		// main system OSC run is cleared, bit 5 in PDRUNCFG register 
		LPC_SYSCON->PDRUNCFG &= ~(0x1<<SYSOSC_POWERDOWN);
		// Wait 200us for OSC to be stablized, no status 
		//indication, dummy wait. 
		for ( i = 0; i < 0x100; i++ );
		//uClockSource = ulOcx;

		if ( bPLLFlag == TRUE )
			nRealPLL_M = Main_PLL_Setup(nPLL_M,oscillator);  
	}
	
	LPC_SYSCON->SYSAHBCLKDIV = nAHB_DIV;	// SYS AHB clock, typical is 1 or 2 or 4 

	if( bPLLFlag == TRUE )
		uSystemFrequency = uClockSource * (nRealPLL_M+1);
	else
		uSystemFrequency = uClockSource;
	uSystemAHBFrequency = (uint32)(uSystemFrequency/nAHB_DIV);

	// System clock to the IOCON needs to be enabled or
	// most of the I/O related peripherals won't work. 
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<IOCON);
	
	{
		volatile uint32_t i;
		for(i = 0; i < 10000; i++); /* 等待系统时钟稳定 */
	}
	__disable_irq();
	
	
	#ifdef __JTAG_DISABLED
	LPC_IOCON->R_PIO1_0  &= ~0x07;				//先清掉为初始复位状态 
	LPC_IOCON->R_PIO1_0  |=  0x01;				//禁止TMS 
	LPC_IOCON->R_PIO1_1  &= ~0x07;				//先清掉为初始复位状态 
	LPC_IOCON->R_PIO1_1  |=  0x01;				//禁止TDO 再设置所需要的模式 
	LPC_IOCON->R_PIO1_2  &= ~0x07;				//先清掉为初始复位状态 
	LPC_IOCON->R_PIO1_2  |=  0x01;				//禁止TRST 再设置所需要的模式 
	LPC_IOCON->R_PIO0_11 &= ~0x07;     
	LPC_IOCON->R_PIO0_11 |=  0x01;				//禁止TDI 设置为普通GPIO 
#endif
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

