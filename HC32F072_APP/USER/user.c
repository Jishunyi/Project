/******************************************************************************/
/**\file user.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-18 15:03:00
 ******************************************************************************/
 
 /******************************************************************************
 * Include files
 ******************************************************************************/
 #include "user.h"
 
 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
stc_uart_datatompu_t DataToMPU = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; //������ʾ����ָ�� //�ѽ���


uint8_t DataToMPUBuffer[DataToMPULen]; 				//������ʾ����ָ�� //������
uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//��ʾ���ش���ָ��	//�ѽ���

uint8_t DataFromMPUFlag =0 ; 		//��ʾ���ش�ָ��ı�־λ 1Ϊ�������

 /*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//ϵͳ��ʼ��
void System_Init(void)
{
		//ͨѶ�˿ڳ�ʼ��
		System_PortInit();
		//���Ź���ʼ��
		System_WdtInit();
		//LED �˿ڳ�ʼ��
    System_LEDInit();
}

//ͨѶ�˿ڳ�ʼ��
void System_PortInit(void)
{
		//Uart1
		App_Uart1Init(UART1Baud);
	
		//CAN
}

//����ָ���ʾ��
void SendToMPU(void)
{
		memcpy((void *)DataToMPUBuffer, (void *)&DataToMPU, DataToMPULen); //װ�ش���������
		App_UartSend(M0P_UART1, DataToMPUBuffer, DataToMPULen); //UART1����
}

//������ʾ���ش�����
void ParseFromMPU(void)
{
		if(DataFromMPUFlag == 1)	//���յ�����
		{		
				App_UartSend(M0P_UART1, DataFromMPUBuffer, DataFromMPULen); //UART1����
				DataFromMPUFlag = 0;
		}			
}

//���Ź���ʼ��
void System_WdtInit(void) 
{
		App_WdtInit();	//���Ź���������
		Wdt_Start(); 		//�������Ź�
}

//LED��ʼ��
void System_LEDInit(void)
{
		App_LEDInit();	//LED��������
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
