/******************************************************************************/
/**\file uart_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-23 15:03:00
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "uart_pro.h"

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t Uart1TxCnt = 0;												//发送中断计数
uint8_t Uart1RxCnt = 0;												//接收中断计数
uint8_t Uart1TxMax;														//发送数据个数
uint8_t Uart1RxMax = DataFromMPULen;					//接收数据个数
uint8_t Uart1TxData[UartSendMax];							//待发送数据
uint8_t Uart1RxData[UartReceiveMax];					//待接收数据

extern uint8_t DataFromMPUFlag; 		//显示屏回传指令的标志位 //1为接收到数据
extern uint8_t DataToMPUBuffer[DataToMPULen]; 				//控制显示屏的指令 //待发送
extern uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//显示屏回传的指令

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//UART1中断函数
void Uart1_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART1, UartRC))         								//UART1数据接收
    { 
				Uart_ClrStatus(M0P_UART1, UartRC);        								//清中断状态位			
				Uart1RxData[Uart1RxCnt] = Uart_ReceiveData(M0P_UART1);		//接收数据
			
				Uart1RxCnt++;
				DataFromMPUFlag = FALSE; //接收未完成
				
				if(Uart1RxCnt >= Uart1RxMax)															//接收结束
				{
						Uart1RxCnt = 0;
						DataFromMPUFlag = TRUE; //接收完成
						memcpy((void *)DataFromMPUBuffer, Uart1RxData, DataFromMPULen); //将接收到的数据取出
					
						Uart_DisableIrq(M0P_UART1, UartRxIrq);            		//禁止UART1接收功能
            Uart_EnableIrq(M0P_UART1, UartTxIrq);              		//使能UART1发送功能
				}
    }
    
    if(Uart_GetStatus(M0P_UART1, UartTC))         								//UART1数据发送
    {
				Uart_ClrStatus(M0P_UART1, UartTC);                        //清除中断状态标志位
				Uart1TxCnt++;
				if(Uart1TxCnt >= Uart1TxMax)                            	//发送结束
        {
						Uart1TxCnt = 0; 
            Uart_DisableIrq(M0P_UART1,UartTxIrq);            			//禁止UART1发送中断
            Uart_EnableIrq(M0P_UART1,UartRxIrq);              		//使能UART1接收中断
        }
				else                                       
        {
            Uart_SendDataIt(M0P_UART1, Uart1TxData[Uart1TxCnt]);  //继续发送数据字节 
        }
    }
}

//UART1引脚配置
void App_Uart1PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //使能GPIO模块时钟
    
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin2, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin2, GpioAf1);          //配置PA02 端口为URART1_TX
    
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin3, GpioAf1);          //配置PA03 端口为URART1_RX
}

//UART1参数配置 //传入波特率
void App_Uart1Cfg(uint32_t Baud)
{
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);

    //开启外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1, TRUE); //使能uart1模块时钟

    //UART1 Init
    stcCfg.enRunMode = UartMskMode1;                	//模式1 //模式2无数据 //模式3多字节数据有误
    stcCfg.enStopBit = UartMsk1bit;                 	//1bit停止位
    stcCfg.enMmdorCk = UartMskEven;                 	//偶检验
    stcCfg.stcBaud.u32Baud = Baud;                  	//波特率
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;      	//通道采样分频配置
    stcCfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq(); 	//获得外设时钟（PCLK）频率值
    Uart_Init(M0P_UART1, &stcCfg);                  	//串口初始化
	
    //UART1中断使能
    Uart_ClrStatus(M0P_UART1, UartRC);         				//清接收请求
		Uart_ClrStatus(M0P_UART1, UartTC);         				//清发送请求
    Uart_EnableIrq(M0P_UART1, UartRxIrq);      				//使能串口接收中断
		//Uart_EnableIrq(M0P_UART1, UartTxIrq);      				//使能串口发送中断
    EnableNvic(UART1_3_IRQn, IrqLevel3, TRUE); 				//系统中断使能
}

//UART1初始化 //传入波特率
void App_Uart1Init(uint32_t Baud)
{
		//UART1引脚配置
		App_Uart1PortInit();
		//UART1参数配置
		App_Uart1Cfg(Baud);
}

void App_UartSend(M0P_UART_TypeDef* UARTx, const uint8_t *DataSendBuffer, const uint8_t DataSendBufferLen) //UART发送数据
{
		//使能串口发送中断
		Uart_EnableIrq(UARTx, UartTxIrq);   
	
		if(UARTx == M0P_UART1) //UART1
		{
				//装载待发送数据
				Uart1TxCnt = 0;
				Uart1TxMax = DataSendBufferLen;
				memcpy((void *)Uart1TxData, DataSendBuffer, Uart1TxMax);
				//启动数据发送
				Uart_SendDataIt(UARTx, Uart1TxData[0]);			//发送第一个字节 后续数据在中断中发送
		}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
