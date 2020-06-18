/******************************************************************************/
/**\file uart_pro.h
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-16 08:51:37
 ******************************************************************************/
 
#ifndef  __UART_PRO_H_
#define  __UART_PRO_H_

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "sysctrl.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
#define     T1_PORT                 (3)
#define     T1_PIN                  (3)

/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void App_Uart1PortInit(void); //UART1引脚配置
void App_Uart1Cfg(void); //UART1参数配置
void App_Uart1Init(void); //UART1初始化
void App_UartSend(M0P_UART_TypeDef* UARTx, const uint8_t * DataSendBuffer, const uint8_t DataSendBufferLen); //串口发送数据
void App_ReceiveDataSet(const uint8_t DataReceiveBufferLen); //串口接收数据配置

#endif
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
