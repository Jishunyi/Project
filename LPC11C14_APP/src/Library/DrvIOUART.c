/*****************************************************************************
	Copyright (C), 2010-2015, BST Electric Co., Ltd.
	File name: DrvIOUART.c
	Author: Yong Hao Version: V0 Date: 14. July. 2015
	Description: GPIO simulated UART driver 
	Version: V0
	Function List: 

	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/

//* Private include files -----------------------------------------------------
//* @{引用头文件声明区域(仅限本文件内使用)

#include "IOUART.h"
#include "LPC11xx.h"
#include "stdlib.h"
#include "string.h"

//**
// * @}

//* Private Macros and Data type -----------------------------------------------
//* @{私有宏和数据定义区域(仅限本文件内使用的全局变量和宏)

/********************************************************************************************************
    模拟串口0   PIO3.1 <-> TXD  PIO3.0 <-> RXD
*********************************************************************************************************/
#define IOTX0_FUNC_SEL() LPC_IOCON->PIO3_1 = 0x10; /*  上拉I/口，滞后禁能          */
#define IORX0_FUNC_SEL() LPC_IOCON->PIO3_0 = 0x10; /*  上拉I/口，滞后禁能          */

#define IOTX0_PORT LPC_GPIO3
#define IOTX0 (1 << 1)

#define IORX0_PORT LPC_GPIO3
#define IORX0 (1 << 0)
#define IORX0_INT EINT3_IRQn

IOUARTCONFIG config[1];

/********************************************************************************************************
    模拟UART FIFO深度配置
*********************************************************************************************************/
unsigned short iou0_tx_buf[12 + 64]; /* 模拟串口0发送FIFO，64个深度  */
unsigned short iou0_rx_buf[12 + 64]; /* 模拟串口0接收FIFO，64个深度  */

/********************************************************************************************************
	接收FIFO定义
*********************************************************************************************************/
unsigned char g_RecvBuff[BUF_REC_SIZE]; /* 接收FIFO */
unsigned char g_RecvIn;					/* FIFO入指针 */
unsigned char g_RecvOut;				/* FIFO出指针 */

//**
// * @}

//* Private function declaration ------------------------------------------------
//* @{私有函数声明区域(仅限本文件内使用)

//**
// * @}

/*******************************************************************************
	Function:			IOUART_Prepare
	Description:		初始化模拟UART的配置信息
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
extern uint32 uSystemFrequency;
void IOUART_Prepare(void)
{
	config[0].baudrate = 9600;
	config[0].verify = VERIFY_NONE;
	config[0].tx_buf = iou0_tx_buf;
	config[0].rx_buf = iou0_rx_buf;
	config[0].tx_buf_len = sizeof(iou0_tx_buf);
	config[0].rx_buf_len = sizeof(iou0_rx_buf);

	IOUARTInit(uSystemFrequency, config);
}

void IOUART_Open(unsigned int pBaud, unsigned int pVerify /*VERIFY_NONE*/)
{
	config[0].baudrate = pBaud;
	config[0].verify = pVerify;
	config[0].tx_buf = iou0_tx_buf;
	config[0].rx_buf = iou0_rx_buf;
	config[0].tx_buf_len = sizeof(iou0_tx_buf);
	config[0].rx_buf_len = sizeof(iou0_rx_buf);

	IOUARTInit(uSystemFrequency, config);
}

/*******************************************************************************
	Function:			IOUART_Send
	Description:		模拟UART的发送数据函数
	Input:				data--要发送的数据地址	size--数据大小
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOUART_Send(unsigned char *data, unsigned char size)
{
	while (size--)
	{
		IOUARTCharPut(0, *data++);
	}
}

/*******************************************************************************
	Function:			IOUART_Recv
	Description:		模拟UART的接收数据函数
	Input:				size--数据大小
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOUART_Recv(unsigned char size)
{
	if (IOUARTRxFifoLength(0) >= size)
	{
		while (size--)
		{
			g_RecvBuff[g_RecvIn++ & 0xFF] = IOUARTCharGetNonBlocking(0);
		}
	}
}

/***********************************引脚配置函数*****************************************/

/*******************************************************************************
	Function:			IOTX0_OUT
	Description:		控制引脚输出高低电平
	Input:				x:为真，模拟UART TXD引脚输出高电平，否则，输出低电平
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOTX0_OUT(char x)
{
	IOTX0_PORT->MASKED_ACCESS[IOTX0] = x ? IOTX0 : 0;
}

/*******************************************************************************
	Function:			IORX0_IN
	Description:		返回模拟UART RXD引脚电平状态
	Input:				None
	Output:				None
	Return:				返回值为char型，为1表示外部引脚为高（高必须为1），为0表示外部引脚为低电平
	Others:				None
********************************************************************************/
char IORX0_IN(void)
{
	return (IORX0_PORT->MASKED_ACCESS[IORX0] > 0);
}

/*******************************************************************************
	Function:			IORX0_OPEN
	Description:		打开模拟UART RXD引脚的下降沿触发功能，打开中断功能之前，必须先清除中断标志
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IORX0_OPEN(void)
{
	IORX0_PORT->IC |= IORX0;
	IORX0_PORT->IE |= IORX0;
}

/*******************************************************************************
	Function:			IORX0_CLOSE
	Description:		关闭模拟UART RXD引脚的下降沿触发功能
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IORX0_CLOSE(void)
{
	IORX0_PORT->IE &= ~IORX0;
}

/*******************************************************************************
	Function:			IOPort0Init
	Description:		GPIO 初始化配置
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOPort0Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 6); /*  开IO端口外设                */

	IOTX0_FUNC_SEL(); /*  上拉I/口，滞后禁能          */
	IORX0_FUNC_SEL(); /*  上拉I/口，滞后禁能          */

	IOTX0_PORT->DIR |= IOTX0;  /* 设置TX为输出                 */
	IORX0_PORT->DIR &= ~IORX0; /* 设置RX为输入                 */

	IORX0_PORT->IS &= ~IORX0;  /* RX为边沿中断                 */
	IORX0_PORT->IEV &= ~IORX0; /* 下降沿中断                   */

	IOTX0_OUT(1);
	IORX0_OPEN();

	NVIC_SetPriority(IORX0_INT, 0);
	NVIC_EnableIRQ(IORX0_INT);
}

/*******************************************************************************
	Function:			PIOINT3_IRQHandler
	Description:		注册中断函数IOEINT3_IRQHandler()
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void PIOINT3_IRQHandler(void)
{
	IOEINT0_IRQHandler();
}

/***********************************定时器配置函数*****************************************/

/*******************************************************************************
	Function:			IOTimer_INT_CLR
	Description:		清除定时器中断标志
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOTimer_INT_CLR(void)
{
	LPC_TMR16B1->IR = 1;
}

/*******************************************************************************
	Function:			IOTimer_OPEN
	Description:		打开定时器计数功能
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOTimer_OPEN(void)
{
	LPC_TMR16B1->TCR = 1;
}

/*******************************************************************************
	Function:			IOTimer_CLOSE
	Description:		关闭定时器计数功能，关闭定时器后，必须将计数器TC清零
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOTimer_CLOSE(void)
{
	LPC_TMR16B1->TCR = 0;
	LPC_TMR16B1->TC = 0;
}

/*******************************************************************************
	Function:			IOTimerInit
	Description:		波特率发生器定时器初始化，中断速率最小为3位波特率
	Input:				ucSysClock：    定时器时钟
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOTimerInit(uint32_t IOTimer_clk, uint32_t bode)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8); /* 打开定时器模块               */

	LPC_TMR16B1->TCR = 0; /* 关闭定时器                   */
	LPC_TMR16B1->IR = 0;
	LPC_TMR16B1->PR = 0;				   /* 设置分频系数                 */
	LPC_TMR16B1->MCR = 3;				   /* 设置MR0匹配后复位TC并产生中断*/
	LPC_TMR16B1->MR0 = IOTimer_clk / bode; /* 设置中断时间                 */
	IOTimer_CLOSE();

	/* 16位定时器计数最大值65535    */
	NVIC_EnableIRQ(TIMER_16_1_IRQn); /* 设置中断并使能               */
	NVIC_SetPriority(TIMER_16_1_IRQn, 2);
}

/*******************************************************************************
	Function:			TIMER16_1_IRQHandler
	Description:		波特率发生器定时器中断函数
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void TIMER16_1_IRQHandler(void)
{
	IOTimer_IRQHandler();
}

//循环判断模拟串口是否接收到MPU发来的数据，如果接收到，将接收到的数据进行断帧，下一步帧处理ParseFrameFromMPU函数
void IOUART_REC(void)
{
	unsigned char len = IOUARTRxFifoLength(0);
	if (len != 0) /*接收FIFO有数据*/
	{
		RecvDataFromMPU(IOUARTCharGetNonBlocking(0));
	}
}
