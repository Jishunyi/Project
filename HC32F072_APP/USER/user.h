/******************************************************************************/
/**\file user.h
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-18 15:03:00
 ******************************************************************************/
 
#ifndef  __USER_H_
#define  __USER_H_

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "uart_pro.h"
#include "wdt_pro.h"
#include "gpio_pro.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
#define DataToMPULen 		8 		//控制显示屏的指令长度
#define DataFromMPULen 	8 		//显示屏回传的指令长度

#define UartSendMax 		64 		//UART发送数据最大个数
#define UartReceiveMax 	64 		//UART接收数据最大个数

#define UART1Baud 			9600	//UART1波特率

 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
typedef struct stc_uart_datatompu
{
    uint8_t	start[2];			//帧头	2byte
    uint8_t len;     			//长度	1byte
    uint8_t cmd;     			//指令	1byte
    uint8_t adrsH;       	//地址H	1byte
		uint8_t adrsL;       	//地址L	1byte
		uint8_t data[2];      //数据	2byte
} stc_uart_datatompu_t;

/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void System_Init(void); 						//系统初始化
void System_PortInit(void); 				//通讯端口初始化
void System_WdtInit(void); 					//看门狗初始化
void System_LEDInit(void); 					//LED初始化

void SendToMPU(void); 							//发送数据到显示屏
void ParseFromMPU(void); 						//解析显示屏回传数据

#endif
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
