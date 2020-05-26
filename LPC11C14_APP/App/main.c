/***************************************************************
          main.c
******************************************************************/

#include "user.h"

//---------------------------------------------------------------------------------------------------------

int main(void)
{
	SystemInit(EXT_XAL_FREQUENCY, MAIN_FREQUENCY);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<GPIO);					  //Datasheet中说明GPIO的AHB时钟默认是打开的，此处代码可以不加
	
	StartSysTick();  		//1ms后执行Callback_TimerArrived，然后周期循环进入Callback_TimerArrived
	IOUART_Open(9600, VERIFY_NONE);    //初始化模拟串口
	TimerInit();
	
	ConrtollerPortInit();
	
	SetConnState(E_SYNC);
	SendPageIndexToMPU(1);
	
	while(1) //等待同步
	{
		if(gSysTickCount % 100 == 5)
		{
			//设置版本号
			T_WRITE_DATAREG tmpData;
			tmpData.mADR_H = (unsigned char)(E_ID_VERSION>>8);
			tmpData.mADR_L = (unsigned char)(E_ID_VERSION);
			memcpy(tmpData.mDATA, gVersion, strlen((char*)gVersion));
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), strlen((char*)gVersion)+2);
		}
			
		IOUART_REC();
		LoopForLedBlink();
		LoopForMPU();
		LoopForFrameToMPU();//uart发送数据到MPU
		LoopForFrameFromMPU();//解析来自MPU的数据
		LoopForFrameFromController();
		if(IsConnectedWithMPU())
		{
			
			break	;
		}
	}
	gSendRightNow = 1;
	
	while(1)
	{
		IOUART_REC();
		LoopForLedBlink();
		LoopForMPU();
		LoopForFrameToMPU();
		LoopForFrameFromMPU();
		LoopForFrameFromController();
	}
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
