#include "DrvSysClk.h"

volatile uint32_t 		 gSysTickCount = 0;				 //1ms时基计数变量

void TIMER16_0_IRQHandler(void)
{
	/* 清除中断标志 */
	if(LPC_TMR16B0->IR & (0x1<<0))
	{  
		LPC_TMR16B0->IR = 0x1<<0;			/* clear interrupt flag */
		gSysTickCount++;
		Callback_SysTick();
	}
}

//JINWENJIE
uint32_t GetElapsedTime(uint32_t pLastTick)
{
	//if(gSysTickCount > pLastTick)
	{
		return gSysTickCount - pLastTick;
	}
	//return (0xffffffffffffffff + gSysTickCount - pLastTick);
}

void StartSysTick(void /*unsigned int pTimeout*/) //us
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<CT16B0);

	LPC_TMR16B0->PR = uSystemFrequency/1000000 - 1;  //1us

	LPC_TMR16B0->MCR = 3;			/* Interrupt and Reset on MR0 */
	
	LPC_TMR16B0->MR0 = 1000;//pTimeout; 
	LPC_TMR16B0->TCR = 1;     /*开启定时器*/

	/* Enable the TIMER0 Interrupt */
	NVIC_EnableIRQ(TIMER_16_0_IRQn);
	NVIC_SetPriority(TIMER_16_0_IRQn,1);	
}

void StopSysTick(void)
{
	NVIC_DisableIRQ(TIMER_16_0_IRQn);
	LPC_TMR16B0->TCR = 2;
}

void Delay(unsigned int time)
{
	/* 由于编译选项中的优化等级为-O1，因此cnt必须加volatile定义 */
	volatile unsigned int cnt;
	while (time--)
	{
		for (cnt = 0; cnt < 100; cnt++)
		{
			;
		}
	}
}

/**************************  Timer  *********************************************************/

TIMEOUT_PARA *gTimeoutList = 0;

void SetTimeout(int pID, uint32_t pTimeout) //ms
{
	if(!gTimeoutList)
	{
		gTimeoutList = (TIMEOUT_PARA*)malloc((unsigned int)(sizeof(TIMEOUT_PARA)));
		gTimeoutList->next = 0;
		gTimeoutList->mID = pID;
		gTimeoutList->mValue = gSysTickCount+pTimeout;
		return;
	}
	else
	{
		TIMEOUT_PARA *tmpParentTimeoutNode = gTimeoutList;
		TIMEOUT_PARA *tmpTimeoutNode = tmpParentTimeoutNode;
		while(tmpTimeoutNode)
		{
			tmpParentTimeoutNode = tmpTimeoutNode;
			if(tmpTimeoutNode->mID == pID)
			{
				tmpTimeoutNode->mValue = gSysTickCount+pTimeout;
				return;
			}
			tmpTimeoutNode = tmpTimeoutNode->next;
		}
		if(tmpParentTimeoutNode)
		{
			tmpTimeoutNode = (TIMEOUT_PARA*)malloc(sizeof(TIMEOUT_PARA));
			tmpTimeoutNode->next = 0;
			tmpTimeoutNode->mID = pID;
			tmpTimeoutNode->mValue = gSysTickCount+pTimeout;
			tmpParentTimeoutNode->next = tmpTimeoutNode;
			return;
		}
	}
}

uint32_t IsTimeout(int pID)  //返回1表示已经超时了
{
	TIMEOUT_PARA *tmpTimeoutNode = gTimeoutList;
	while(tmpTimeoutNode)
	{
		if(tmpTimeoutNode->mID == pID)
		{
			if(gSysTickCount >= tmpTimeoutNode->mValue)
			 return 1;
			return 0;
		}
		tmpTimeoutNode = tmpTimeoutNode->next;
	}
	return 0;
}


