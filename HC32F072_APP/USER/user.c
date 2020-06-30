/******************************************************************************/
/**\file user.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-30 15:26:42
 ******************************************************************************/
 
 /******************************************************************************
 * Include files
 ******************************************************************************/
 #include "user.h"
 
 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
//CAN
stc_can_datafromctr DataFromCtr; 							//���ƹ����ָ�� 							//�ѽ���
uint32_t IDFromCtrBuffer; 										//���ƹ����ָ�� ID						//�ѽ���
uint8_t DataFromCtrBuffer[DataFromCtrLen]; 		//���ƹ����ָ�� ����					//�ѽ���

uint8_t DataToCtrBuffer[DataToCtrLen]; 				//�ش������ƹ��ָ������					//������
uint8_t DataFromCANFlag = FALSE ; 						//�ش������ƹ��ָ��ı�־λ

//UART
stc_uart_datatompu_t DataToMPU = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x81}; 		//������ʾ����ָ�� //�ѽ���
uint8_t DataToMPUBuffer[DataToMPULen]; 				//������ʾ����ָ�� 							//������
uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//��ʾ���ش���ָ��	����					//�ѽ���
uint8_t DataFromMPUFlag =FALSE ; 							//��ʾ���ش�ָ��ı�־λ

//�Զ���ʾģʽ
uint32_t ParseModeAuto_unit 	= '0';									//�Զ���ʾ ��λ
uint32_t ParseModeAuto_ten 		= '0';									//�Զ���ʾ ʮλ
uint32_t ParseModeAuto_arrow 	= 	1;									//�Զ���ʾ ��ͷ

 /*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//ϵͳ��ʼ��
void System_Init(void)
{		
		//ϵͳʱ������
		//System_ClkInit();		//ϵͳʱ���л�Ϊ�ⲿʱ�Ӻ� CAN�շ����� UART�շ�����
		//ͨѶ�˿ڳ�ʼ��
		System_PortInit();
		//���Ź���ʼ��
		System_WdtInit();
		//LED �˿ڳ�ʼ��
    System_LEDInit();
}

//ϵͳʱ������
void System_ClkInit(void)
{
	AHB_Clock_PortCfg();		/*��ʼ��ʱ��AHB_CLK�������*/
	App_ClkInit();
	Rch4MHzToXth12MHz();			/*ʱ��Դ�л�(RCH->XTH)*/
	Xth12MHzToPll48MHz();		/*xth��Ϊʱ��Դ������PLL��Ƶ48MHz*/

	//ʹ��HCLK��PA01��� ����HCLK��
	Gpio_SfHClkOutputCfg(GpioSfHclkOutEnable, GpioSfHclkOutDiv1);
}

//�˿����� ����AHBʱ�������
void AHB_Clock_PortCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< ����GPIO����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    

    ///<========================== ʱ������˿�ģʽ���� ===========================
    ///< �˿ڷ�������->���
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�������������->����������
    stcGpioCfg.enDrv = GpioDrvH;
    ///< �˿�����������->��������
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< �˿ڿ�©�������->��©����ر�
    stcGpioCfg.enOD = GpioOdDisable;
    ///< GPIO IO PA01��ʼ��
    Gpio_Init(GpioPortA, GpioPin1, &stcGpioCfg);    
    ///< ����PA01���ù���ΪHCLK���
    Gpio_SetAfMode(GpioPortA, GpioPin1, GpioAf6);
}

//ʱ�ӳ�ʼ������ �����Ѿ�Ĭ��ʹ���ڲ�4MHzRCH�����������ٴ�����
void App_ClkInit(void)
{
    stc_sysctrl_clk_cfg_t stcCfg;
    
    ///< ����FLASH����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);
    
    ///<========================== ʱ�ӳ�ʼ������ ===================================
    ///< ��Ҫʹ�õ�ʱ��ԴHCLKС��24M���˴�����FLASH ���ȴ�����Ϊ0 cycle(Ĭ��ֵҲΪ0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    ///< ʱ�ӳ�ʼ��ǰ����������Ҫʹ�õ�ʱ��Դ���˴�����RCHΪ4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    ///< ѡ���ڲ�RCH��ΪHCLKʱ��Դ;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    ///< HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    ///< PCLK ΪHCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    ///< ϵͳʱ�ӳ�ʼ��
    Sysctrl_ClkInit(&stcCfg);
}

//��ʱ�Ӵ�RCH4MHz�л���XTH12MHz
void Rch4MHzToXth12MHz(void)
{    
	/*ʹ��Flash��ʱ��*/
	Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);
	
	//======================== ��ʱ�Ӵ�RCH4MHz�л���XTH8MHz ==============================    
    //��Ҫʹ�õ�ʱ��ԴHCLK��С��24M���˴�����FLASH ���ȴ�����Ϊ1 cycle(Ĭ��ֵҲΪ1 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    //�л�ʱ��ǰ�������ⲿ���پ�������XTHƵ�ʷ�Χ,���þ��������ʹ��Ŀ��ʱ�ӣ��˴�Ϊ8MHz
    Sysctrl_SetXTHFreq(SysctrlXthFreq6_12MHz);
    Sysctrl_XTHDriverCfg(SysctrlXtalDriver3);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
    delay1ms(10);
    
    //ʱ���л�
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
	
    //������Ҫѡ���Ƿ�ر�ԭʱ�ӣ��˴����رգ�
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, FALSE);
}

//PLL��Ƶ
void Xth12MHzToPll48MHz (void)
{
    stc_sysctrl_pll_cfg_t stcPLLCfg;
    DDL_ZERO_STRUCT(stcPLLCfg);	
	
	//======================== ��ʱ�Ӵ�XTH8MHz�л���PLL48MHz ==============================    
    //��Ƶ���ʱ��ԴHCLK����48M���˴�����FLASH ���ȴ�����Ϊ2 cycle(ǰ���Ѿ����ã��˴������ظ�����)
    Flash_WaitCycle(FlashWaitCycle1);
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq6_12MHz;    //XTH 12MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL ���48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllXthXtal;          //����ʱ��Դѡ��XTH
    stcPLLCfg.enPllMul    = SysctrlPllMul4;             //12MHz x 4 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
	
    //ʱ���л�
    Sysctrl_SysClkSwitch(SysctrlClkPLL);   
	
	//ʱ�ӷ�Ƶ����
	Sysctrl_SetHCLKDiv(SysctrlHclkDiv1);
	Sysctrl_SetPCLKDiv(SysctrlPclkDiv1);
}

//ͨѶ�˿ڳ�ʼ��
void System_PortInit(void)
{
		//Uart1
		App_Uart1Init(UART1Baud);
	
		//CAN
		App_CANInit(CANBaud);				//CAN��ʼ������		//���벨����
}

//���Ź���ʼ��
void System_WdtInit(void) 
{
		App_WdtInit(WdtT1s64);	//���Ź���������
		Wdt_Start(); 		//�������Ź�
}

//LED��ʼ��
void System_LEDInit(void)
{
		App_LEDInit();	//LED��������
}

void ParseFromCtr(uint32_t Mode)	//�������ƹ�������
{	
		if(Mode == ParseModeNoml)	//����ģʽ
		{
				if(DataFromCANFlag == TRUE) //���յ�����
				{
						if(IDFromCtrBuffer == 0x600)	//ID��ȷ
						{
								//¥�㣺
								DataToMPU.DATA[2] = DataFromCtrBuffer[0]; //ʮλ����λ��
								DataToMPU.DATA[1] = DataFromCtrBuffer[1]; //��λ����λ��
							
								//��ͷ��
								if((DataFromCtrBuffer[3]&0x40) == 0x40) //��
								{
										DataToMPU.DATA[5] = 1;
								}
								else if((DataFromCtrBuffer[3]&0x20) == 0x20) //��
								{
										DataToMPU.DATA[5] = 2; 
								}
						}
					DataFromCANFlag = FALSE;
				}
		}
		else if(Mode == ParseModeAuto)	//�Զ���ʾģʽ
		{
				delay1ms(1000);
			
				if((ParseModeAuto_ten <= '9') && (ParseModeAuto_unit <= '9') && (ParseModeAuto_arrow ==1)) //����
				{
						DataToMPU.DATA[2] = ParseModeAuto_ten; 	//ʮλ����λ��
						DataToMPU.DATA[1] = ParseModeAuto_unit; //��λ����λ��
					
						if(ParseModeAuto_unit < '9') //��λ����
						{
								ParseModeAuto_unit++;
						}
						else if(ParseModeAuto_unit == '9') //ʮλ����
						{
								ParseModeAuto_ten++;
								if(ParseModeAuto_ten == '9') //������ʾ����
								{
										ParseModeAuto_arrow = 2; 
								}
						}
				}
				else if((ParseModeAuto_ten >= '0') && (ParseModeAuto_unit >= '0') && (ParseModeAuto_arrow ==2)) //����
				{
						DataToMPU.DATA[2] = ParseModeAuto_ten; 	//ʮλ����λ��
						DataToMPU.DATA[1] = ParseModeAuto_unit; //��λ����λ��
						if(ParseModeAuto_unit > '0') //��λ�ݼ�
						{
								ParseModeAuto_unit--;
						}
						else if(ParseModeAuto_unit == '0') //ʮλ�ݼ�
						{
								ParseModeAuto_ten--;
								if(ParseModeAuto_ten == '0') //������ʾ����
								{
										ParseModeAuto_arrow = 1; 
								}
						}
				}
				else //��ʼ¥����Ϣ��Ϊ0-9
				{
						ParseModeAuto_unit 		= '0';									//�Զ���ʾ ��λ
						ParseModeAuto_ten 		= '0';									//�Զ���ʾ ʮλ
						ParseModeAuto_arrow 	= 	1;									//�Զ���ʾ ��ͷ
				}
		}
		//����У��λ
		DataToMPU.CRC =  (DataToMPU.DATA[0] ^ DataToMPU.DATA[1] ^ DataToMPU.DATA[2]^ DataToMPU.DATA[3] 
						^ DataToMPU.DATA[4]^ DataToMPU.DATA[5] ^ DataToMPU.DATA[6] ^ DataToMPU.DATA[7]) & 0x7f;
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
		if(DataFromMPUFlag == TRUE)	//���յ�����
		{		
				App_UartSend(M0P_UART1, DataFromMPUBuffer, DataFromMPULen); //UART1����
				DataFromMPUFlag = FALSE;
		}			
}

void SendToCtr(void)	//�ش����ݵ����ƹ�
{

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
