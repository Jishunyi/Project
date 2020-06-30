/******************************************************************************/
/**\file user.h
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-30 15:26:42
 ******************************************************************************/
 
#ifndef  __USER_H_
#define  __USER_H_

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "flash.h"
#include "can_pro.h"
#include "uart_pro.h"
#include "wdt_pro.h"
#include "gpio_pro.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
#define DataToCtrLen 			8 					//回传到控制柜的指令长度
#define DataFromCtrLen 		8 					//控制柜传入的指令长度

#define DataToMPULen 			11 					//控制显示屏的指令长度
#define DataFromMPULen 		11 					//显示屏回传的指令长度

#define CANSendMax 				64 					//CAN发送数据最大个数
#define CANReceiveMax 		64 					//CAN接收数据最大个数

#define UartSendMax 			64 					//UART发送数据最大个数
#define UartReceiveMax 		64 					//UART接收数据最大个数

#define CANBaud 					20000				//CAN波特率
#define UART1Baud 				9600				//UART1波特率

#define ParseModeNoml 		0						//常规模式
#define ParseModeAuto 		1						//自动演示模式

 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
typedef struct stc_uart_datatompu	//控制显示屏指令
{
    uint8_t	STX;							//帧头	1byte
    uint8_t DATA[8];     			//长度	7byte
    uint8_t CRC;     					//指令	1byte
    uint8_t ETX;       				//帧尾	1byte
} stc_uart_datatompu_t;

typedef struct stc_can_datafromctr	//控制柜传入指令
{
    uint16_t	id;					//ID		2byte
    uint8_t data[8];     	//数据	8byte
} stc_can_datafromctr;

/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void System_Init(void); 						//系统初始化
void AHB_Clock_PortCfg(void);				//端口配置 设置AHB时钟输出口
void App_ClkInit(void);							//时钟初始化配置 启动已经默认使用内部4MHzRCH，这里无需再次配置
void Rch4MHzToXth12MHz(void);				//将时钟从RCH4MHz切换至XTH12MHz
void Xth12MHzToPll48MHz (void);			//PLL倍频

void System_ClkInit(void);					//时钟源初始化
void System_PortInit(void); 				//通讯端口初始化
void System_WdtInit(void); 					//看门狗初始化
void System_LEDInit(void); 					//LED初始化

void ParseFromCtr(uint32_t Mode); 	//解析控制柜传入数据
void SendToMPU(void); 							//发送数据到显示屏
void ParseFromMPU(void); 						//解析显示屏回传数据
void SendToCtr(void); 							//回传数据到控制柜



#endif
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
