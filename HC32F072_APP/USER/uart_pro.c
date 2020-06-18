/******************************************************************************/
/**\file uart_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-16 08:51:37
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "gpio.h"
#include "uart.h"
#include "uart_pro.h"
#include "user.h"
#include "stdlib.h"

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t Uart1TxCnt = 0;												//�����жϼ���
uint8_t Uart1RxCnt = 0;												//�����жϼ���
uint8_t Uart1TxMax;														//�������ݸ���
uint8_t Uart1RxMax = DataFromMPULen;					//�������ݸ���
uint8_t Uart1TxData[UartSendMax];							//����������
uint8_t Uart1RxData[UartReceiveMax];					//����������

extern uint8_t DataFromMPUFlag; 		//��ʾ���ش�ָ��ı�־λ //1Ϊ���յ�����

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//UART1�жϺ���
void Uart1_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART1, UartRC))         								//UART1���ݽ���
    { 
				Uart_ClrStatus(M0P_UART1, UartRC);        								//���ж�״̬λ			
				Uart1RxData[Uart1RxCnt] = Uart_ReceiveData(M0P_UART1);	
			
				Uart1RxCnt++;
				DataFromMPUFlag = 0; //����δ���
				
				if(Uart1RxCnt >= Uart1RxMax)															//���ս���
				{
						Uart1RxCnt = 0;
						DataFromMPUFlag = 1; //�������
						Uart_DisableIrq(M0P_UART1, UartRxIrq);            		//��ֹUART1���չ���
            Uart_EnableIrq(M0P_UART1, UartTxIrq);              		//ʹ��UART1���͹���
				}
    }
    
    if(Uart_GetStatus(M0P_UART1, UartTC))         								//UART1���ݷ���
    {
				Uart_ClrStatus(M0P_UART1, UartTC);                        //����ж�״̬��־λ
				Uart1TxCnt++;
				if(Uart1TxCnt >= Uart1TxMax)                            	//���ͽ���
        {
						Uart1TxCnt = 0; 
            Uart_DisableIrq(M0P_UART1,UartTxIrq);            			//��ֹUART1�����ж�
            Uart_EnableIrq(M0P_UART1,UartRxIrq);              		//ʹ��UART1�����ж�
        }
				else                                       
        {
            Uart_SendDataIt(M0P_UART1, Uart1TxData[Uart1TxCnt]);  //�������������ֽ� 
        }
    }
}

//UART1��������
void App_Uart1PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //ʹ��GPIOģ��ʱ��
    
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin2, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin2, GpioAf1);          //����PA02 �˿�ΪURART1_TX
    
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin3, GpioAf1);          //����PA03 �˿�ΪURART1_RX
}

//UART1��������
void App_Uart1Cfg(void)
{
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);

    //��������ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1, TRUE); //ʹ��uart1ģ��ʱ��

    //UART1 Init
    stcCfg.enRunMode = UartMskMode1;                	//ģʽ1 //ģʽ2������ //ģʽ3���ֽ���������
    stcCfg.enStopBit = UartMsk1bit;                 	//1bitֹͣλ
    stcCfg.enMmdorCk = UartMskEven;                 	//ż����
    stcCfg.stcBaud.u32Baud = 9600;                  	//������9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;      	//ͨ��������Ƶ����
    stcCfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq(); 	//�������ʱ�ӣ�PCLK��Ƶ��ֵ
    Uart_Init(M0P_UART1, &stcCfg);                  	//���ڳ�ʼ��
	
    //UART1�ж�ʹ��
    Uart_ClrStatus(M0P_UART1, UartRC);         				//���������
		Uart_ClrStatus(M0P_UART1, UartTC);         				//�巢������
    Uart_EnableIrq(M0P_UART1, UartRxIrq);      				//ʹ�ܴ��ڽ����ж�
		//Uart_EnableIrq(M0P_UART1, UartTxIrq);      				//ʹ�ܴ��ڷ����ж�
    EnableNvic(UART1_3_IRQn, IrqLevel3, TRUE); 				//ϵͳ�ж�ʹ��
}

//UART1��ʼ��
void App_Uart1Init(void)
{
		//UART1��������
		App_Uart1PortInit();
		//UART1��������
		App_Uart1Cfg();
}

void App_UartSend(M0P_UART_TypeDef* UARTx, const uint8_t *DataSendBuffer, const uint8_t DataSendBufferLen) //���ڷ�������
{
		//ʹ�ܴ��ڷ����ж�
		Uart_EnableIrq(UARTx, UartTxIrq);   
	
		if(UARTx == M0P_UART1) //UART1
		{
				//װ�ش���������
				Uart1TxCnt = 0;
				Uart1TxMax = DataSendBufferLen;
				memcpy((void *)Uart1TxData, DataSendBuffer, Uart1TxMax);
			
				//�������ݷ���
				Uart_SendDataIt(UARTx, Uart1TxData[0]);			//���͵�һ���ֽ� �����������ж��з���
		}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/