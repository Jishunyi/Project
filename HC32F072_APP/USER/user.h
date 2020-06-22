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
#include "can_pro.h"
#include "uart_pro.h"
#include "wdt_pro.h"
#include "gpio_pro.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
#define DataToCtrLen 		8 		//�ش������ƹ��ָ���
#define DataFromCtrLen 	8 		//���ƹ����ָ���

#define DataToMPULen 		8 		//������ʾ����ָ���
#define DataFromMPULen 	8 		//��ʾ���ش���ָ���

#define UartSendMax 		64 		//UART��������������
#define UartReceiveMax 	64 		//UART��������������

#define CANBaud 				1000	//CAN������	K
#define UART1Baud 			9600	//UART1������

#define ParseModeNoml 	0			//����ģʽ
#define ParseModeAuto 	1			//�Զ���ʾģʽ

 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
typedef struct stc_uart_datatompu	//������ʾ��ָ��
{
    uint8_t	start[2];			//֡ͷ	2byte
    uint8_t len;     			//����	1byte
    uint8_t cmd;     			//ָ��	1byte
    uint8_t adrsH;       	//��ַH	1byte
		uint8_t adrsL;       	//��ַL	1byte
		uint8_t data[2];      //����	2byte
} stc_uart_datatompu_t;

typedef struct stc_uart_datafromctr	//���ƹ���ָ��
{
    uint16_t	id;					//ID		2byte
    uint8_t data[8];     	//����	8byte
} stc_uart_datafromctr;

/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void System_Init(void); 						//ϵͳ��ʼ��
void System_PortInit(void); 				//ͨѶ�˿ڳ�ʼ��
void System_WdtInit(void); 					//���Ź���ʼ��
void System_LEDInit(void); 					//LED��ʼ��

void ParseFromCtr(uint32_t Mode); 	//�������ƹ�������
void SendToMPU(void); 							//�������ݵ���ʾ��
void ParseFromMPU(void); 						//������ʾ���ش�����
void SendToCtr(void); 							//�ش����ݵ����ƹ�

#endif
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
