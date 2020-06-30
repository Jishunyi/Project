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
#define DataToCtrLen 			8 					//�ش������ƹ��ָ���
#define DataFromCtrLen 		8 					//���ƹ����ָ���

#define DataToMPULen 			11 					//������ʾ����ָ���
#define DataFromMPULen 		11 					//��ʾ���ش���ָ���

#define CANSendMax 				64 					//CAN��������������
#define CANReceiveMax 		64 					//CAN��������������

#define UartSendMax 			64 					//UART��������������
#define UartReceiveMax 		64 					//UART��������������

#define CANBaud 					20000				//CAN������
#define UART1Baud 				9600				//UART1������

#define ParseModeNoml 		0						//����ģʽ
#define ParseModeAuto 		1						//�Զ���ʾģʽ

 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
typedef struct stc_uart_datatompu	//������ʾ��ָ��
{
    uint8_t	STX;							//֡ͷ	1byte
    uint8_t DATA[8];     			//����	7byte
    uint8_t CRC;     					//ָ��	1byte
    uint8_t ETX;       				//֡β	1byte
} stc_uart_datatompu_t;

typedef struct stc_can_datafromctr	//���ƹ���ָ��
{
    uint16_t	id;					//ID		2byte
    uint8_t data[8];     	//����	8byte
} stc_can_datafromctr;

/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void System_Init(void); 						//ϵͳ��ʼ��
void AHB_Clock_PortCfg(void);				//�˿����� ����AHBʱ�������
void App_ClkInit(void);							//ʱ�ӳ�ʼ������ �����Ѿ�Ĭ��ʹ���ڲ�4MHzRCH�����������ٴ�����
void Rch4MHzToXth12MHz(void);				//��ʱ�Ӵ�RCH4MHz�л���XTH12MHz
void Xth12MHzToPll48MHz (void);			//PLL��Ƶ

void System_ClkInit(void);					//ʱ��Դ��ʼ��
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
