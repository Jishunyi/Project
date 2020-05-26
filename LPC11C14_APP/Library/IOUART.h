/*******************************************************************************
	Copyright (C), 2010-2015, BST Electric Co., Ltd.
	File name: DrvIOUART.h
	Author: Yong Hao Version: V0 Date: 14. July. 2015
	Description: 
    DrvIOUART.c的头文件，声明了可供其他文件使用的函数和变量

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
//* @{公共头文件

//#include "DebugMsg.h"
#include "stdint.h"

//**
// * @}

//* Public Macros/Configuration and Data type --------------------------------
//* @{公共宏/配置和数据定义区域(源文件中可供外部文件使用的宏/配置和数据)

/*********************************************************************************************************
    模拟串口初始化时的校验方式
*********************************************************************************************************/
#define     VERIFY_NONE         0                                       /*  无校验                      */
#define     VERIFY_ODD          1                                       /*  奇校验                      */
#define     VERIFY_EVEN         2                                       /*  偶校验                      */
#define     VERIFY_ONE          3                                       /*  1校验                       */
#define     VERIFY_ZERO         4                                       /*  0校验                       */
#define     VERIFY_ERR          0x100                                   /*  校验错误位，Bit9位          */

/********************************************************************************************************
    模拟串口相关信息结构体
*********************************************************************************************************/
typedef struct {
    /* 
     *  模拟串口波特率，支持所有标准波特率，波特率越高，CPU占用率越高，波特率：小于等于19200bps，大于等于1200bps
     */
    unsigned long       baudrate;       
                                                
    /* 
     *  模拟串口校验方式，支持5种校验方式，分别为，0：无校验，1：奇校验，偶校验，3：1校验，4：0校验   
     */
    unsigned char       verify;  
    
    /* 
     *  模拟串口发送、接收缓冲区
     */
    unsigned short     *tx_buf;                                         /*  发送缓冲区                  */
    unsigned short      tx_buf_len;                                     /*  发送缓冲区大小              */
    unsigned short     *rx_buf;                                         /*  接收缓冲区                  */
    unsigned short      rx_buf_len;                                     /*  接收缓冲区大小              */
} IOUARTCONFIG;

#define BUF_REC_SIZE 256		
extern unsigned char g_RecvBuff[BUF_REC_SIZE]; 
extern unsigned char g_RecvIn;   				
extern unsigned char g_RecvOut;				

//**
// * @}	

//* Public function declaration -----------------------------------------------
//* @{公共函数声明区域(可供外部文件使用)

/*********************************************************************************************************
    硬件占用资源列表，以下函数需注册到中断向量表
*********************************************************************************************************/
extern void IOTimer_IRQHandler (void);                                  /*  模拟UART定时器中断          */
extern void IOEINT0_IRQHandler(void);                                   /*  模拟UART TXD中断服务函数    */

/*********************************************************************************************************
    定时器硬件移植需要实现的接口
*********************************************************************************************************/
extern void IOTimer_INT_CLR(void);                                      /*  清除定时器中断              */
extern void IOTimer_OPEN(void);                                         /*  打开定时器计数功能          */
extern void IOTimer_CLOSE(void);                                        /*  关闭定时器计数功能，关闭后，*/
                                                                        /*  必须将计数器TC清零          */
extern void IOTimerInit(uint32_t IOTimer_clk, uint32_t bode);           /*  定时器初始化，如下：        */
                                                                        /*  1、计算定时周期，如以下公式 */
                                                                        /*  定时周期 = IOTimer_clk/bode */
                                                                        /*  2、关闭计数功能             */
                                                                        /*  3、使用及注册中断服务函数   */

/*********************************************************************************************************
    通用I/O口硬件移植需要实现的接口
*********************************************************************************************************/
extern void IOTX0_OUT(char x);                                          /*  x为1：TXD输出高，否则输出低 */
extern char IORX0_IN(void);                                             /*  返回1，表示RXD为高，0为低   */
extern void IORX0_OPEN(void);                                           /*  打开RXD下降沿捕获中断，打开 */
                                                                        /*  前，必须清除该中断标志      */
extern void IORX0_CLOSE(void);                                          /*  关闭RXD下降沿捕获中断       */
extern void IOPort0Init(void);                                          /*  初始化TXD、RXD引脚，如下：  */
                                                                        /*  1、初始化TXD、RXD为GPIO功能 */
                                                                        /*  2、TXD为输出，RXD为输入     */
                                                                        /*  3、RXD为下降沿捕获中断      */
                                                                        /*  4、调用IOTX0_OUT(1)         */
                                                                        /*  5、调用IORX0_OPEN(1)        */
                                                                        /*  6、使用及注册中断服务函数   */

/*********************************************************************************************************
    模拟串口API函数声明
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:       IOUARTInit
** Descriptions:        IOUART 初始化 函数
** input parameters:    timer_clk:  当前定时器工作时钟
**                      config[1]： 模拟串口配置参数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern uint32_t IOUARTInit(uint32_t timer_clk, IOUARTCONFIG config[1]);

/*********************************************************************************************************
** Function name:       IOUARTTxState
** Descriptions:        获取当前发送FIFO状态，当返回为1时，表示没有数据发送，为0时，表示当前有数据在发送，
**                      用于485控制
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      1：表示当前数据已发送完毕，包括最后的停止位，0：表示还数据在发送中
*********************************************************************************************************/
extern uint8_t IOUARTTxState(uint32_t ulID);

/*********************************************************************************************************
** Function name:       IOUARTTxFifoLength
** Descriptions:        获取当前发送FIFO的数据深度，以字节为单位
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      当前发送FIFO的字节深度
*********************************************************************************************************/
extern uint32_t IOUARTTxFifoLength(uint32_t ulID);

/*********************************************************************************************************
** Function name:       IOUARTRxFifoLength
** Descriptions:        获取当前接收FIFO的数据深度，以字节为单位
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      当前接收FIFO的字节深度
*********************************************************************************************************/
extern uint32_t IOUARTRxFifoLength(uint32_t ulID);

/*********************************************************************************************************
** Function name:       IOUARTSpaceAvail
** Descriptions:        判断模拟串口是否发送FIFO（FIFO深度为64字节）是否还有剩余
** input parameters:    ulID:    模拟串口编号，固定为0
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern unsigned char IOUARTSpaceAvail(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharsAvail
** Descriptions:        判断接收FIFO是否有数据
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern unsigned char IOUARTCharsAvail(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharGet
** Descriptions:        从串口FIFO获取一个字节的数据，阻塞，如串口FIFO没数据则无限循环等待
** input parameters:    ulID:   模拟串口编号，固定为0 
** output parameters:   无
** Returned value:      返回接收到的数据，低8位表示接收到的数据，Bit8表示奇偶校验位状态，1表示奇偶校验错误，
**                      0表示奇偶校验正确。
*********************************************************************************************************/
extern unsigned long IOUARTCharGet(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharGetNonBlocking
** Descriptions:        从串口FIFO获取一个字节的数据，无阻塞，如串口FIFO没数据则立刻退出并返回-1
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      该返回值和IOUARTCharGet的返回值基本相同，其惟一的区别在于FIFO没有数据的情况下，
**                      该返回值等于-1，即0xFFFFFFFF
*********************************************************************************************************/
extern long IOUARTCharGetNonBlocking(unsigned long ulID);

/*********************************************************************************************************
** Function name:       IOUARTCharPut
** Descriptions:        通过模拟串口发送数据，阻塞，如FIFO没有剩余空间，则等待直到有剩余空间。
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void IOUARTCharPut(unsigned long ulID, unsigned char ulData);

/*********************************************************************************************************
** Function name:       IOUARTCharPutNonBlocking
** Descriptions:        通过模拟串口发送数据，无阻塞，如FIFO没有剩余空间则立刻退出并返回0。
** input parameters:    ulID:   模拟串口编号，固定为0
** output parameters:   无
** Returned value:      0：表示数据发送失败，失败的原因可能有：ulID参数错误，FIFO没有剩余空间，
                        1：表示数据成功写入发送FIFO，该函数退出时数据并没有发送完毕，IOUARTCharPut函数同理
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
