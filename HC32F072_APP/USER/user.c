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
stc_uart_datafromctr DataFromCtr = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; 	//���ƹ����ָ�� //�ѽ���
stc_uart_datatompu_t DataToMPU = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; 		//������ʾ����ָ�� //�ѽ���

uint8_t DataFromCtrBuffer[DataFromCtrLen]; 		//���ƹ����ָ���		//�ѽ���
uint8_t DataToCtrBuffer[DataToCtrLen]; 				//�ش������ƹ��ָ��� //������

uint8_t DataToMPUBuffer[DataToMPULen]; 				//������ʾ����ָ�� 			//������
uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//��ʾ���ش���ָ��				//�ѽ���

uint8_t DataFromMPUFlag =0 ; 									//��ʾ���ش�ָ��ı�־λ 1Ϊ�������

uint32_t ParseModeAuto_i = 0;									//�Զ���ʾ����

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
		App_SysClkInit();						//ϵͳʱ�ӳ�ʼ��(12MHz for CanClk)
		App_CanGpioInit();					//CAN GPIO ����
		App_CanInit(CANBaud);				//CAN��ʼ������		//���벨����
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


extern stc_can_rxframe_t       stcRxFrame;
extern stc_can_txframe_t       stcTxFrame;
extern uint8_t                 u8RxFlag;
void ParseFromCtr(uint32_t Mode)	//�������ƹ�������
{	
		uint8_t u8Idx = 0;
		if(Mode == ParseModeNoml)	//����ģʽ
		{
				while(1) //�ش����յ�����
				{
						if(TRUE == u8RxFlag)
						{
								u8RxFlag = FALSE;

								if(1 == stcRxFrame.Cst.Control_f.RTR)
								{
										continue;
								}

								//<<Can Tx
								stcTxFrame.StdID         = stcRxFrame.StdID;
								stcTxFrame.Control_f.DLC = stcRxFrame.Cst.Control_f.DLC;
								stcTxFrame.Control_f.IDE = stcRxFrame.Cst.Control_f.IDE;
								stcTxFrame.Control_f.RTR = stcRxFrame.Cst.Control_f.RTR;

								for(u8Idx=0; u8Idx<stcRxFrame.Cst.Control_f.DLC; u8Idx++)
								{
										stcTxFrame.Data[u8Idx] = stcRxFrame.Data[u8Idx];
								}

								CAN_SetFrame(&stcTxFrame);
								CAN_TransmitCmd(CanPTBTxCmd);

								CAN_IrqCmd(CanRxIrqEn, TRUE);
						}
				}
		}
		else if(Mode == ParseModeAuto)	//�Զ���ʾģʽ
		{
				if(ParseModeAuto_i < 16)
				{
						
						ParseModeAuto_i++;
				}
		
		}
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

void SendToCtr(void)	//�ش����ݵ����ƹ�
{

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
