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
stc_uart_datafromctr DataFromCtr = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; 	//控制柜传入的指令 //已解析
stc_uart_datatompu_t DataToMPU = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; 		//控制显示屏的指令 //已解析

uint8_t DataFromCtrBuffer[DataFromCtrLen]; 		//控制柜传入的指令长度		//已接收
uint8_t DataToCtrBuffer[DataToCtrLen]; 				//回传到控制柜的指令长度 //待发送

uint8_t DataToMPUBuffer[DataToMPULen]; 				//控制显示屏的指令 			//待发送
uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//显示屏回传的指令				//已接收

uint8_t DataFromMPUFlag =0 ; 									//显示屏回传指令的标志位 1为接收完成

uint32_t ParseModeAuto_i = 0;									//自动演示计数

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
		App_SysClkInit();						//系统时钟初始化(12MHz for CanClk)
		App_CanGpioInit();					//CAN GPIO 配置
		App_CanInit(CANBaud);				//CAN初始化设置		//传入波特率
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


extern stc_can_rxframe_t       stcRxFrame;
extern stc_can_txframe_t       stcTxFrame;
extern uint8_t                 u8RxFlag;
void ParseFromCtr(uint32_t Mode)	//解析控制柜传入数据
{	
		uint8_t u8Idx = 0;
		if(Mode == ParseModeNoml)	//常规模式
		{
				while(1) //回传接收到数据
				{
						if(TRUE == u8RxFlag)
						{
								u8RxFlag = FALSE;

								if(1 == stcRxFrame.Cst.Control_f.RTR)
								{
										continue;
								}

								//<<Can Tx
								stcTxFrame.StdID         = stcRxFrame.StdID;
								stcTxFrame.Control_f.DLC = stcRxFrame.Cst.Control_f.DLC;
								stcTxFrame.Control_f.IDE = stcRxFrame.Cst.Control_f.IDE;
								stcTxFrame.Control_f.RTR = stcRxFrame.Cst.Control_f.RTR;

								for(u8Idx=0; u8Idx<stcRxFrame.Cst.Control_f.DLC; u8Idx++)
								{
										stcTxFrame.Data[u8Idx] = stcRxFrame.Data[u8Idx];
								}

								CAN_SetFrame(&stcTxFrame);
								CAN_TransmitCmd(CanPTBTxCmd);

								CAN_IrqCmd(CanRxIrqEn, TRUE);
						}
				}
		}
		else if(Mode == ParseModeAuto)	//自动演示模式
		{
				if(ParseModeAuto_i < 16)
				{
						
						ParseModeAuto_i++;
				}
		
		}
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

void SendToCtr(void)	//回传数据到控制柜
{

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
