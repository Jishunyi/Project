/******************************************************************************/
/**\file user.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-18 15:03:00
 ******************************************************************************/
 
 /******************************************************************************
 * Include files
 ******************************************************************************/
 #include "user.h"
 
 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
stc_uart_datatompu_t DataToMPU = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; //控制显示屏的指令 //已解析


uint8_t DataToMPUBuffer[DataToMPULen]; 				//控制显示屏的指令 //待发送
uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//显示屏回传的指令	//已接收

uint8_t DataFromMPUFlag =0 ; 		//显示屏回传指令的标志位 1为接收完成

 /*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//系统初始化
void System_Init(void)
{
		//通讯端口初始化
		System_PortInit();
		//看门狗初始化
		System_WdtInit();
		//LED 端口初始化
    System_LEDInit();
}

//通讯端口初始化
void System_PortInit(void)
{
		//Uart1
		App_Uart1Init(UART1Baud);
	
		//CAN
}

//发送指令到显示屏
void SendToMPU(void)
{
		memcpy((void *)DataToMPUBuffer, (void *)&DataToMPU, DataToMPULen); //装载待发送数据
		App_UartSend(M0P_UART1, DataToMPUBuffer, DataToMPULen); //UART1发送
}

//解析显示屏回传数据
void ParseFromMPU(void)
{
		if(DataFromMPUFlag == 1)	//接收到数据
		{		
				App_UartSend(M0P_UART1, DataFromMPUBuffer, DataFromMPULen); //UART1发送
				DataFromMPUFlag = 0;
		}			
}

//看门狗初始化
void System_WdtInit(void) 
{
		App_WdtInit();	//看门狗参数配置
		Wdt_Start(); 		//启动看门狗
}

//LED初始化
void System_LEDInit(void)
{
		App_LEDInit();	//LED参数配置
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
