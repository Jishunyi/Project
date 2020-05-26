/***************************************************************
          main.c
******************************************************************/

#include "user.h"

//---------------------------------------------------------------------------------------------------------

int main(void)
{
	SystemInit(EXT_XAL_FREQUENCY, MAIN_FREQUENCY);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<GPIO);					  //Datasheet��˵��GPIO��AHBʱ��Ĭ���Ǵ򿪵ģ��˴�������Բ���
	
	StartSysTick();  		//1ms��ִ��Callback_TimerArrived��Ȼ������ѭ������Callback_TimerArrived
	IOUART_Open(9600, VERIFY_NONE);    //��ʼ��ģ�⴮��
	TimerInit();
	
	ConrtollerPortInit();
	
	SetConnState(E_SYNC);
	SendPageIndexToMPU(1);
	
	while(1) //�ȴ�ͬ��
	{
		if(gSysTickCount % 100 == 5)
		{
			//���ð汾��
			T_WRITE_DATAREG tmpData;
			tmpData.mADR_H = (unsigned char)(E_ID_VERSION>>8);
			tmpData.mADR_L = (unsigned char)(E_ID_VERSION);
			memcpy(tmpData.mDATA, gVersion, strlen((char*)gVersion));
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), strlen((char*)gVersion)+2);
		}
			
		IOUART_REC();
		LoopForLedBlink();
		LoopForMPU();
		LoopForFrameToMPU();//uart�������ݵ�MPU
		LoopForFrameFromMPU();//��������MPU������
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
