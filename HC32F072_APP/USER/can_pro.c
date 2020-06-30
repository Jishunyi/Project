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
extern uint32_t IDFromCtrBuffer; 						//控制柜传入的指令 ID						//已接收
extern uint8_t 	DataFromCtrBuffer[]; 				//控制柜传入的指令 数据					//已接收
extern uint8_t 	DataFromCANFlag; 						//接收CAN传入的指令的标志位

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 //CAN中断函数
void Can_IRQHandler(void)
{		
		uint8_t i = 0;
		stc_can_rxframe_t stcRxFrame;
    if(TRUE == CAN_IrqFlgGet(CanRxIrqFlg))
    {		
				//接收数据
        CAN_IrqFlgClr(CanRxIrqFlg);
        CAN_IrqCmd(CanRxIrqEn, FALSE);	//关闭CAN接收中断
        CAN_Receive(&stcRxFrame);				//接收CAN数据
				
				//拷贝数据
				IDFromCtrBuffer = stcRxFrame.StdID;
				for(i=0; i<DataFromCtrLen; i++)
				{
						DataFromCtrBuffer[i] = stcRxFrame.Data[i];
				}
        DataFromCANFlag = TRUE;					//接收完成
    }
}

//CAN初始化设置		//传入波特率
void App_CANInit(uint32_t Baud)
{
	//IO口配置
	{	
		stc_gpio_cfg_t stcGpioCfg;
		Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

		//端口方向配置->输入
		stcGpioCfg.enDir = GpioDirIn;
		//端口驱动能力配置->高驱动能力
		stcGpioCfg.enDrv = GpioDrvL;
		//端口上下拉配置->无
		stcGpioCfg.enPu = GpioPuDisable;
		stcGpioCfg.enPd = GpioPdDisable;
		//端口开漏输出配置->开漏输出关闭
		stcGpioCfg.enOD = GpioOdDisable;
		//端口输入/输出值寄存器总线控制模式配置->AHB
		stcGpioCfg.enCtrlMode = GpioAHB;

		Gpio_Init(EVB_CAN_RX_PORT, EVB_CAN_RX_PIN, &stcGpioCfg);
		stcGpioCfg.enDir = GpioDirOut;
		Gpio_Init(EVB_CAN_TX_PORT, EVB_CAN_TX_PIN, &stcGpioCfg);
		//Gpio_Init(EVB_CAN_STB_PORT, EVB_CAN_STB_PIN, &stcGpioCfg);

		//CAN RX\TX复用功能配置
		Gpio_SetAfMode(EVB_CAN_RX_PORT, EVB_CAN_RX_PIN, GpioAf3);
		Gpio_SetAfMode(EVB_CAN_TX_PORT, EVB_CAN_TX_PIN, GpioAf5);

		//STB 低-PHY有效
		//Gpio_ClrIO(EVB_CAN_STB_PORT, EVB_CAN_STB_PIN);
	}
	
	//CAN_Init()
	{	
		stc_can_init_config_t   stcCanInitCfg;
		stc_can_filter_t        stcFilter;

		Sysctrl_SetPeripheralGate(SysctrlPeripheralCan, TRUE);

		stcCanInitCfg.stcCanBt.SEG_1 = 5-2;
		stcCanInitCfg.stcCanBt.SEG_2 = 3-1;
		stcCanInitCfg.stcCanBt.PRESC = ((4000000/Baud)/(stcCanInitCfg.stcCanBt.SEG_1+stcCanInitCfg.stcCanBt.SEG_2+3)-1); //倍频后为48M所以是48000000 //芯片内部时钟为4M
		stcCanInitCfg.stcCanBt.SJW   = 3-1;

		stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 10;
		stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 16-1;

		stcCanInitCfg.enCanRxBufAll  = CanRxNormal;					//正常模式
		stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;		//最新接收到的数据不被存储
		stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;			//FIFO模式

		//CAN初始化
		CAN_Init(&stcCanInitCfg);

		//CAN 滤波器配置
		stcFilter.enAcfFormat = CanAllFrames;				//接收帧类型 标准帧 / 拓展帧
		stcFilter.enFilterSel = CanFilterSel1;			//筛选器组1使能
		stcFilter.u32CODE     = 0x00000352;
		stcFilter.u32MASK     = 0x1FFFFFFF;
		CAN_FilterConfig(&stcFilter, TRUE);

		//Can Irq Enable
		CAN_IrqCmd(CanRxIrqEn, TRUE);
		EnableNvic(CAN_IRQn, IrqLevel0, TRUE);
	}
}

//CAN发送数据
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

		CAN_IrqCmd(CanRxIrqEn, TRUE);           //使能CAN接收中断
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
