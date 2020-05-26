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
//* @{����ͷ�ļ���������(���ޱ��ļ���ʹ��)

#include "IOUART.h"
#include "LPC11xx.h"
#include "stdlib.h"
#include "string.h"

//**
// * @}

//* Private Macros and Data type -----------------------------------------------
//* @{˽�к�����ݶ�������(���ޱ��ļ���ʹ�õ�ȫ�ֱ����ͺ�)

/********************************************************************************************************
    ģ�⴮��0   PIO3.1 <-> TXD  PIO3.0 <-> RXD
*********************************************************************************************************/
#define IOTX0_FUNC_SEL() LPC_IOCON->PIO3_1 = 0x10; /*  ����I/�ڣ��ͺ����          */
#define IORX0_FUNC_SEL() LPC_IOCON->PIO3_0 = 0x10; /*  ����I/�ڣ��ͺ����          */

#define IOTX0_PORT LPC_GPIO3
#define IOTX0 (1 << 1)

#define IORX0_PORT LPC_GPIO3
#define IORX0 (1 << 0)
#define IORX0_INT EINT3_IRQn

IOUARTCONFIG config[1];

/********************************************************************************************************
    ģ��UART FIFO�������
*********************************************************************************************************/
unsigned short iou0_tx_buf[12 + 64]; /* ģ�⴮��0����FIFO��64�����  */
unsigned short iou0_rx_buf[12 + 64]; /* ģ�⴮��0����FIFO��64�����  */

/********************************************************************************************************
	����FIFO����
*********************************************************************************************************/
unsigned char g_RecvBuff[BUF_REC_SIZE]; /* ����FIFO */
unsigned char g_RecvIn;					/* FIFO��ָ�� */
unsigned char g_RecvOut;				/* FIFO��ָ�� */

//**
// * @}

//* Private function declaration ------------------------------------------------
//* @{˽�к�����������(���ޱ��ļ���ʹ��)

//**
// * @}

/*******************************************************************************
	Function:			IOUART_Prepare
	Description:		��ʼ��ģ��UART��������Ϣ
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
	Description:		ģ��UART�ķ������ݺ���
	Input:				data--Ҫ���͵����ݵ�ַ	size--���ݴ�С
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
	Description:		ģ��UART�Ľ������ݺ���
	Input:				size--���ݴ�С
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

/***********************************�������ú���*****************************************/

/*******************************************************************************
	Function:			IOTX0_OUT
	Description:		������������ߵ͵�ƽ
	Input:				x:Ϊ�棬ģ��UART TXD��������ߵ�ƽ����������͵�ƽ
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
	Description:		����ģ��UART RXD���ŵ�ƽ״̬
	Input:				None
	Output:				None
	Return:				����ֵΪchar�ͣ�Ϊ1��ʾ�ⲿ����Ϊ�ߣ��߱���Ϊ1����Ϊ0��ʾ�ⲿ����Ϊ�͵�ƽ
	Others:				None
********************************************************************************/
char IORX0_IN(void)
{
	return (IORX0_PORT->MASKED_ACCESS[IORX0] > 0);
}

/*******************************************************************************
	Function:			IORX0_OPEN
	Description:		��ģ��UART RXD���ŵ��½��ش������ܣ����жϹ���֮ǰ������������жϱ�־
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
	Description:		�ر�ģ��UART RXD���ŵ��½��ش�������
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
	Description:		GPIO ��ʼ������
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOPort0Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 6); /*  ��IO�˿�����                */

	IOTX0_FUNC_SEL(); /*  ����I/�ڣ��ͺ����          */
	IORX0_FUNC_SEL(); /*  ����I/�ڣ��ͺ����          */

	IOTX0_PORT->DIR |= IOTX0;  /* ����TXΪ���                 */
	IORX0_PORT->DIR &= ~IORX0; /* ����RXΪ����                 */

	IORX0_PORT->IS &= ~IORX0;  /* RXΪ�����ж�                 */
	IORX0_PORT->IEV &= ~IORX0; /* �½����ж�                   */

	IOTX0_OUT(1);
	IORX0_OPEN();

	NVIC_SetPriority(IORX0_INT, 0);
	NVIC_EnableIRQ(IORX0_INT);
}

/*******************************************************************************
	Function:			PIOINT3_IRQHandler
	Description:		ע���жϺ���IOEINT3_IRQHandler()
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void PIOINT3_IRQHandler(void)
{
	IOEINT0_IRQHandler();
}

/***********************************��ʱ�����ú���*****************************************/

/*******************************************************************************
	Function:			IOTimer_INT_CLR
	Description:		�����ʱ���жϱ�־
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
	Description:		�򿪶�ʱ����������
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
	Description:		�رն�ʱ���������ܣ��رն�ʱ���󣬱��뽫������TC����
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
	Description:		�����ʷ�������ʱ����ʼ�����ж�������СΪ3λ������
	Input:				ucSysClock��    ��ʱ��ʱ��
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void IOTimerInit(uint32_t IOTimer_clk, uint32_t bode)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8); /* �򿪶�ʱ��ģ��               */

	LPC_TMR16B1->TCR = 0; /* �رն�ʱ��                   */
	LPC_TMR16B1->IR = 0;
	LPC_TMR16B1->PR = 0;				   /* ���÷�Ƶϵ��                 */
	LPC_TMR16B1->MCR = 3;				   /* ����MR0ƥ���λTC�������ж�*/
	LPC_TMR16B1->MR0 = IOTimer_clk / bode; /* �����ж�ʱ��                 */
	IOTimer_CLOSE();

	/* 16λ��ʱ���������ֵ65535    */
	NVIC_EnableIRQ(TIMER_16_1_IRQn); /* �����жϲ�ʹ��               */
	NVIC_SetPriority(TIMER_16_1_IRQn, 2);
}

/*******************************************************************************
	Function:			TIMER16_1_IRQHandler
	Description:		�����ʷ�������ʱ���жϺ���
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void TIMER16_1_IRQHandler(void)
{
	IOTimer_IRQHandler();
}

//ѭ���ж�ģ�⴮���Ƿ���յ�MPU���������ݣ�������յ��������յ������ݽ��ж�֡����һ��֡����ParseFrameFromMPU����
void IOUART_REC(void)
{
	unsigned char len = IOUARTRxFifoLength(0);
	if (len != 0) /*����FIFO������*/
	{
		RecvDataFromMPU(IOUARTCharGetNonBlocking(0));
	}
}
