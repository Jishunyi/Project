#include "DrvGPIO.h"

/*****************************************************************************
** Function name:		GPIOSetValue
**
** Descriptions:		Set/clear a bitvalue in a specific bit position
**						in GPIO portX(X is the port number.)
**
** parameters:			port num, bit position, bit value
** Returned value:		None
**
*****************************************************************************/
void GPIOSetValue( uint32 portNum, uint32 bitPosi, uint32 bitVal )
{
	LPC_GPIO[portNum]->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
}

/*****************************************************************************
** Function name:		GPIOGetValue
**
** Descriptions:		Get a bitvalue in a specific bit position
**						in GPIO portX(X is the port number.)
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
uint32 GPIOGetValue( uint32 portNum, uint32 bitPosi )
{
	return (uint8)(((LPC_GPIO[portNum]->DATA)&(1<<bitPosi))>>bitPosi);//���ص������Ƶ����һλ��BIT0
}

/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			port num, bit position, direction (1 out, 0 input)
** Returned value:		None
**
*****************************************************************************/
void GPIOSetDir( uint32 portNum, uint32 bitPosi, uint32 dir )
{
#ifndef __JTAG_DISABLED	
	if(((portNum == JTAG_TDI_PIOPORT)&&(bitPosi == JTAG_TDI_PIOBIT))
	||((portNum == JTAG_TMS_PIOPORT)&&(bitPosi == JTAG_TMS_PIOBIT))
	||((portNum == JTAG_TDO_PIOPORT)&&(bitPosi == JTAG_TDO_PIOBIT))
	||((portNum == JTAG_nTRST_PIOPORT)&&(bitPosi == JTAG_nTRST_PIOBIT)))
	{
		if((portNum == JTAG_TDI_PIOPORT)&&(bitPosi == JTAG_TDI_PIOBIT))
		{
			LPC_IOCON->R_PIO0_11 &= ~0x07;
			LPC_IOCON->R_PIO0_11 |=  0x01;				//��ֹTDI ����Ϊ��ͨGPIO 
		}
		else if((portNum == JTAG_TMS_PIOPORT)&&(bitPosi == JTAG_TMS_PIOBIT))
		{
			LPC_IOCON->R_PIO1_0  &= ~0x07;
			LPC_IOCON->R_PIO1_0  |=  0x01;				//��ֹTMS ����Ϊ��ͨGPIO 
		}
		else if((portNum == JTAG_TDO_PIOPORT)&&(bitPosi == JTAG_TDO_PIOBIT))
		{
			LPC_IOCON->R_PIO1_1  &= ~0x07;
			LPC_IOCON->R_PIO1_1  |=  0x01;				//��ֹTDO ����Ϊ��ͨGPIO 
		}
		else if((portNum == JTAG_nTRST_PIOPORT)&&(bitPosi == JTAG_nTRST_PIOBIT))
		{		 
			LPC_IOCON->R_PIO1_2  &= ~0x07;
			LPC_IOCON->R_PIO1_2  |=  0x01;				//��ֹnTRST ����Ϊ��ͨGPIO 
		}
		else
		{
			return;
		}
	}
#endif
 	if(dir)
	{
		LPC_GPIO[portNum]->DIR |= 1<<bitPosi;
	}
 	else
	{
		LPC_GPIO[portNum]->DIR &= ~(1<<bitPosi);
	}
}

//-----------------  LED  ------------------------------------------------------------------------------

volatile uint32_t		 gLEDInited = 0;
volatile uint32_t    gLEDInterval = 0;  //��˸����ms
volatile uint32_t 	 gLEDTick = 0;					 //LED��˸�߼�����

void SetLedBinkFrequency(unsigned int pInterval)
{
	gLEDInterval = pInterval;
}

void LoopForLedBlink(void)
{
	if(gLEDInterval && (gLEDTick < gSysTickCount))  //���ݵ�ǰ����״̬gWorkMode���趨LED��˸��Ƶ��
	{
		static volatile int ledFlag = 0;
		if(gLEDInited == 0)
		{
			gLEDInited = 1;
			LED_INIT;
		}
		(ledFlag == 0) ? (LED_ON) : (LED_OFF);
		ledFlag = !ledFlag;
		gLEDTick = gSysTickCount + gLEDInterval;
	}
}

/******************************************************************************
**					 End Of File
******************************************************************************/
