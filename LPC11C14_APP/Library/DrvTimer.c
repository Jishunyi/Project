#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "DrvTimer.h"

void StartTimer(uint8 pNum, uint32 pTimeout) //us
{
	if( pNum == 0 )
	{
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<CT32B0);
		LPC_TMR32B0->MR0 = pTimeout;     
		LPC_TMR32B0->PR = uSystemFrequency/1000000 - 1;  /* 预分频计数器 */
		LPC_TMR32B0->MCR = 3;			/* Interrupt and Reset on MR0 */
		LPC_TMR32B0->TCR = 1;     /* 开始计时 */

		/* Enable the TIMER0 Interrupt */
		NVIC_EnableIRQ(TIMER_32_0_IRQn);
		NVIC_SetPriority(TIMER_32_0_IRQn, 1);
	}
	else if( pNum == 1 )
	{
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<CT32B1);

		LPC_TMR32B1->MR0 = pTimeout;
		LPC_TMR32B1->PR = uSystemFrequency/1000000 - 1;   /* 预分频计数器 */

		LPC_TMR32B1->MCR = 3;			/* Interrupt and Reset on MR0 */
		LPC_TMR32B1->TCR = 1;     /* 开始计时 */

		/* Enable the TIMER1 Interrupt */
		NVIC_EnableIRQ(TIMER_32_1_IRQn);
		NVIC_SetPriority(TIMER_32_1_IRQn, 1);
	}
}

void StopTimer(uint8 pNum)
{
	if( pNum == 0 )
	{
		NVIC_DisableIRQ(TIMER_32_0_IRQn);
		LPC_TMR32B0->TCR = 2;
	}
	else if( pNum == 1 )
	{
		NVIC_DisableIRQ(TIMER_32_1_IRQn);
		LPC_TMR32B1->TCR = 2;
	}
}

void TIMER32_0_IRQHandler(void)
{
  if(LPC_TMR32B0->IR & (0x1<<0))
  {  
		LPC_TMR32B0->IR = 0x1<<0;				/* clear interrupt flag */
		Callback_Timer(0);
	}
}

void TIMER32_1_IRQHandler(void)
{
	if(LPC_TMR32B1->IR & (0x1<<0))
	{
		LPC_TMR32B1->IR = 0x1<<0;				/* clear interrupt flag */
		Callback_Timer(1);
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/

