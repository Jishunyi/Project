/*******************************************************************************
	Copyright (C), 2010-2015, BST Electric Co., Ltd.
	File name: DrvIOUART.h
	Author: Yong Hao Version: V0 Date: 14. July. 2015
	Description: 
    DrvIOUART.c��ͷ�ļ��������˿ɹ������ļ�ʹ�õĺ����ͱ���

	Others: 
	Function List: 

	History: 
	1.
		Date:
		Author:
		Modification:
	2. 
******************************************************************************/

#ifndef _IOUART_H_
#define _IOUART_H_

//* Public include files ------------------------------------------------------
//* @{����ͷ�ļ�

//#include "DebugMsg.h"
#include "stdint.h"

//**
// * @}

//* Public Macros/Configuration and Data type --------------------------------
//* @{������/���ú����ݶ�������(Դ�ļ��пɹ��ⲿ�ļ�ʹ�õĺ�/���ú�����)

/*********************************************************************************************************
    ģ�⴮�ڳ�ʼ��ʱ��У�鷽ʽ
*********************************************************************************************************/
#define     VERIFY_NONE         0                                       /*  ��У��                      */
#define     VERIFY_ODD          1                                       /*  ��У��                      */
#define     VERIFY_EVEN         2                                       /*  żУ��                      */
#define     VERIFY_ONE          3                                       /*  1У��                       */
#define     VERIFY_ZERO         4                                       /*  0У��                       */
#define     VERIFY_ERR          0x100                                   /*  У�����λ��Bit9λ          */

/********************************************************************************************************
    ģ�⴮�������Ϣ�ṹ��
*********************************************************************************************************/
typedef struct {
    /* 
     *  ģ�⴮�ڲ����ʣ�֧�����б�׼�����ʣ�������Խ�ߣ�CPUռ����Խ�ߣ������ʣ�С�ڵ���19200bps�����ڵ���1200bps
     */
    unsigned long       baudrate;       
                                                
    /* 
     *  ģ�⴮��У�鷽ʽ��֧��5��У�鷽ʽ���ֱ�Ϊ��0����У�飬1����У�飬żУ�飬3��1У�飬4��0У��   
     */
    unsigned char       verify;  
    
    /* 
     *  ģ�⴮�ڷ��͡����ջ�����
     */
    unsigned short     *tx_buf;                                         /*  ���ͻ�����                  */
    unsigned short      tx_buf_len;                                     /*  ���ͻ�������С              */
    unsigned short     *rx_buf;                                         /*  ���ջ�����                  */
    unsigned short      rx_buf_len;                                     /*  ���ջ�������С              */
} IOUARTCONFIG;

#define BUF_REC_SIZE 256		
extern unsigned char g_RecvBuff[BUF_REC_SIZE]; 
extern unsigned char g_RecvIn;   				
extern unsigned char g_RecvOut;				

//**
// * @}	

//* Public function declaration -----------------------------------------------
//* @{����������������(�ɹ��ⲿ�ļ�ʹ��)

/*********************************************************************************************************
    Ӳ��ռ����Դ�б����º�����ע�ᵽ�ж�������
*********************************************************************************************************/
extern void IOTimer_IRQHandler (void);                                  /*  ģ��UART��ʱ���ж�          */
extern void IOEINT0_IRQHandler(void);                                   /*  ģ��UART TXD�жϷ�����    */

/*********************************************************************************************************
    ��ʱ��Ӳ����ֲ��Ҫʵ�ֵĽӿ�
*********************************************************************************************************/
extern void IOTimer_INT_CLR(void);                                      /*  �����ʱ���ж�              */
extern void IOTimer_OPEN(void);                                         /*  �򿪶�ʱ����������          */
extern void IOTimer_CLOSE(void);                                        /*  �رն�ʱ���������ܣ��رպ�*/
                                                                        /*  ���뽫������TC����          */
extern void IOTimerInit(uint32_t IOTimer_clk, uint32_t bode);           /*  ��ʱ����ʼ�������£�        */
                                                                        /*  1�����㶨ʱ���ڣ������¹�ʽ */
                                                                        /*  ��ʱ���� = IOTimer_clk/bode */
                                                                        /*  2���رռ�������             */
                                                                        /*  3��ʹ�ü�ע���жϷ�����   */

/*********************************************************************************************************
    ͨ��I/O��Ӳ����ֲ��Ҫʵ�ֵĽӿ�
*********************************************************************************************************/
extern void IOTX0_OUT(char x);                                          /*  xΪ1��TXD����ߣ���������� */
extern char IORX0_IN(void);                                             /*  ����1����ʾRXDΪ�ߣ�0Ϊ��   */
extern void IORX0_OPEN(void);                                           /*  ��RXD�½��ز����жϣ��� */
                                                                        /*  ǰ������������жϱ�־      */
extern void IORX0_CLOSE(void);                                          /*  �ر�RXD�½��ز����ж�       */
extern void IOPort0Init(void);                                          /*  ��ʼ��TXD��RXD���ţ����£�  */
                                                                        /*  1����ʼ��TXD��RXDΪGPIO���� */
                                                                        /*  2��TXDΪ�����RXDΪ����     */
                                                                        /*  3��RXDΪ�½��ز����ж�      */
                                                                        /*  4������IOTX0_OUT(1)         */
                                                                        /*  5������IORX0_OPEN(1)        */
                                                                        /*  6��ʹ�ü�ע���жϷ�����   */

/*********************************************************************************************************
    ģ�⴮��API��������
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:       IOUARTInit
** Descriptions:        IOUART ��ʼ�� ����
** input parameters:    timer_clk:  ��ǰ��ʱ������ʱ��
**                      config[1]�� ģ�⴮�����ò���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern uint32_t IOUARTInit(uint32_t timer_clk, IOUARTCONFIG config[1]);

/*********************************************************************************************************
** Function name:       IOUARTTxState
** Descriptions:        ��ȡ��ǰ����FIFO״̬��������Ϊ1ʱ����ʾû�����ݷ��ͣ�Ϊ0ʱ����ʾ��ǰ�������ڷ��ͣ�
**                      ����485����
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      1����ʾ��ǰ�����ѷ�����ϣ���������ֹͣλ��0����ʾ�������ڷ�����
*********************************************************************************************************/
extern uint8_t IOUARTTxState(uint32_t ulID);

/*********************************************************************************************************
** Function name:       IOUARTTxFifoLength
** Descriptions:        ��ȡ��ǰ����FIFO��������ȣ����ֽ�Ϊ��λ
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      ��ǰ����FIFO���ֽ����
*********************************************************************************************************/
extern uint32_t IOUARTTxFifoLength(uint32_t ulID);

/*********************************************************************************************************
** Function name:       IOUARTRxFifoLength
** Descriptions:        ��ȡ��ǰ����FIFO��������ȣ����ֽ�Ϊ��λ
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      ��ǰ����FIFO���ֽ����
*********************************************************************************************************/
extern uint32_t IOUARTRxFifoLength(uint32_t ulID);

/*********************************************************************************************************
** Function name:       IOUARTSpaceAvail
** Descriptions:        �ж�ģ�⴮���Ƿ���FIFO��FIFO���Ϊ64�ֽڣ��Ƿ���ʣ��
** input parameters:    ulID:    ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern unsigned char IOUARTSpaceAvail(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharsAvail
** Descriptions:        �жϽ���FIFO�Ƿ�������
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern unsigned char IOUARTCharsAvail(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharGet
** Descriptions:        �Ӵ���FIFO��ȡһ���ֽڵ����ݣ��������紮��FIFOû����������ѭ���ȴ�
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0 
** output parameters:   ��
** Returned value:      ���ؽ��յ������ݣ���8λ��ʾ���յ������ݣ�Bit8��ʾ��żУ��λ״̬��1��ʾ��żУ�����
**                      0��ʾ��żУ����ȷ��
*********************************************************************************************************/
extern unsigned long IOUARTCharGet(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharGetNonBlocking
** Descriptions:        �Ӵ���FIFO��ȡһ���ֽڵ����ݣ����������紮��FIFOû�����������˳�������-1
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      �÷���ֵ��IOUARTCharGet�ķ���ֵ������ͬ����Ωһ����������FIFOû�����ݵ�����£�
**                      �÷���ֵ����-1����0xFFFFFFFF
*********************************************************************************************************/
extern long IOUARTCharGetNonBlocking(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharPut
** Descriptions:        ͨ��ģ�⴮�ڷ������ݣ���������FIFOû��ʣ��ռ䣬��ȴ�ֱ����ʣ��ռ䡣
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void IOUARTCharPut(unsigned long ulID, unsigned char ulData);

/*********************************************************************************************************
** Function name:       IOUARTCharPutNonBlocking
** Descriptions:        ͨ��ģ�⴮�ڷ������ݣ�����������FIFOû��ʣ��ռ��������˳�������0��
** input parameters:    ulID:   ģ�⴮�ڱ�ţ��̶�Ϊ0
** output parameters:   ��
** Returned value:      0����ʾ���ݷ���ʧ�ܣ�ʧ�ܵ�ԭ������У�ulID��������FIFOû��ʣ��ռ䣬
                        1����ʾ���ݳɹ�д�뷢��FIFO���ú����˳�ʱ���ݲ�û�з�����ϣ�IOUARTCharPut����ͬ��
*********************************************************************************************************/
extern long IOUARTCharPutNonBlocking(unsigned long ulID, unsigned char ulData);

void IOUART_Prepare(void);

void IOUART_Open(unsigned int pBaud, unsigned int pVerify);
void IOUART_Send(unsigned char *data, unsigned char size);
void IOUART_Recv(unsigned char size);

void IOUART_REC(void);
void RecvDataFromMPU(unsigned char pData);

//**
// * @}	

#endif //_DRVIOUART_H_ 
