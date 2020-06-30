/******************************************************************************/
/**\file can_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-30 15:26:42
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "can_pro.h"

/******************************************************************************
 * variable definitions                                      									*
 ******************************************************************************/
extern uint32_t IDFromCtrBuffer; 						//���ƹ����ָ�� ID						//�ѽ���
extern uint8_t 	DataFromCtrBuffer[]; 				//���ƹ����ָ�� ����					//�ѽ���
extern uint8_t 	DataFromCANFlag; 						//����CAN�����ָ��ı�־λ

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 //CAN�жϺ���
void Can_IRQHandler(void)
{		
		uint8_t i = 0;
		stc_can_rxframe_t stcRxFrame;
    if(TRUE == CAN_IrqFlgGet(CanRxIrqFlg))
    {		
				//��������
        CAN_IrqFlgClr(CanRxIrqFlg);
        CAN_IrqCmd(CanRxIrqEn, FALSE);	//�ر�CAN�����ж�
        CAN_Receive(&stcRxFrame);				//����CAN����
				
				//��������
				IDFromCtrBuffer = stcRxFrame.StdID;
				for(i=0; i<DataFromCtrLen; i++)
				{
						DataFromCtrBuffer[i] = stcRxFrame.Data[i];
				}
        DataFromCANFlag = TRUE;					//�������
    }
}

//CAN��ʼ������		//���벨����
void App_CANInit(uint32_t Baud)
{
	//IO������
	{	
		stc_gpio_cfg_t stcGpioCfg;
		Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

		//�˿ڷ�������->����
		stcGpioCfg.enDir = GpioDirIn;
		//�˿�������������->����������
		stcGpioCfg.enDrv = GpioDrvL;
		//�˿�����������->��
		stcGpioCfg.enPu = GpioPuDisable;
		stcGpioCfg.enPd = GpioPdDisable;
		//�˿ڿ�©�������->��©����ر�
		stcGpioCfg.enOD = GpioOdDisable;
		//�˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
		stcGpioCfg.enCtrlMode = GpioAHB;

		Gpio_Init(EVB_CAN_RX_PORT, EVB_CAN_RX_PIN, &stcGpioCfg);
		stcGpioCfg.enDir = GpioDirOut;
		Gpio_Init(EVB_CAN_TX_PORT, EVB_CAN_TX_PIN, &stcGpioCfg);
		//Gpio_Init(EVB_CAN_STB_PORT, EVB_CAN_STB_PIN, &stcGpioCfg);

		//CAN RX\TX���ù�������
		Gpio_SetAfMode(EVB_CAN_RX_PORT, EVB_CAN_RX_PIN, GpioAf3);
		Gpio_SetAfMode(EVB_CAN_TX_PORT, EVB_CAN_TX_PIN, GpioAf5);

		//STB ��-PHY��Ч
		//Gpio_ClrIO(EVB_CAN_STB_PORT, EVB_CAN_STB_PIN);
	}
	
	//CAN_Init()
	{	
		stc_can_init_config_t   stcCanInitCfg;
		stc_can_filter_t        stcFilter;

		Sysctrl_SetPeripheralGate(SysctrlPeripheralCan, TRUE);

		stcCanInitCfg.stcCanBt.SEG_1 = 5-2;
		stcCanInitCfg.stcCanBt.SEG_2 = 3-1;
		stcCanInitCfg.stcCanBt.PRESC = ((4000000/Baud)/(stcCanInitCfg.stcCanBt.SEG_1+stcCanInitCfg.stcCanBt.SEG_2+3)-1); //��Ƶ��Ϊ48M������48000000 //оƬ�ڲ�ʱ��Ϊ4M
		stcCanInitCfg.stcCanBt.SJW   = 3-1;

		stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 10;
		stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 16-1;

		stcCanInitCfg.enCanRxBufAll  = CanRxNormal;					//����ģʽ
		stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;		//���½��յ������ݲ����洢
		stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;			//FIFOģʽ

		//CAN��ʼ��
		CAN_Init(&stcCanInitCfg);

		//CAN �˲�������
		stcFilter.enAcfFormat = CanAllFrames;				//����֡���� ��׼֡ / ��չ֡
		stcFilter.enFilterSel = CanFilterSel1;			//ɸѡ����1ʹ��
		stcFilter.u32CODE     = 0x00000352;
		stcFilter.u32MASK     = 0x1FFFFFFF;
		CAN_FilterConfig(&stcFilter, TRUE);

		//Can Irq Enable
		CAN_IrqCmd(CanRxIrqEn, TRUE);
		EnableNvic(CAN_IRQn, IrqLevel0, TRUE);
	}
}

//CAN��������
void App_CANSend(uint32_t ID, const uint8_t *DataSendBuffer, const uint8_t DataSendBufferLen)
{
		uint8_t i = 0;
		stc_can_txframe_t stcTxFrame;
		stcTxFrame.StdID         = ID;
		stcTxFrame.Control_f.DLC = DataSendBufferLen;
		stcTxFrame.Control_f.IDE = 0;
		stcTxFrame.Control_f.RTR = 0;

		for(i=0; i<DataSendBufferLen; i++)
		{
				stcTxFrame.Data[i] = DataSendBuffer[i];
		}

		CAN_SetFrame(&stcTxFrame);
		CAN_TransmitCmd(CanPTBTxCmd);

		CAN_IrqCmd(CanRxIrqEn, TRUE);           //ʹ��CAN�����ж�
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
