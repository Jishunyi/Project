#ifndef _DRVSYSCLK_H_
#define _DRVSYSCLK_H_

#include "LPC11xx.h"
#include "system_LPC11xx.h"

#define EXT_XAL_FREQUENCY                   (uint32)12000000
#define MAIN_FREQUENCY                      (uint32)48000000

extern volatile uint32_t 		 gSysTickCount;

void StartSysTick(void /*unsigned int pTimeout*/); //us
void StopSysTick(void);

void Callback_SysTick(void);

void Delay(unsigned int time);

/****************************************************************/

typedef struct _TIMEOUT_PARA
{	
	struct _TIMEOUT_PARA *next;
	uint32_t   mID;
	uint32_t   mValue;
} TIMEOUT_PARA;

void SetTimeout(int pID, uint32_t pTimeout);
uint32_t IsTimeout(int pID);

uint32_t GetElapsedTime(uint32_t pLastTick);

#endif //_DRVSYSCLK_H_ 
