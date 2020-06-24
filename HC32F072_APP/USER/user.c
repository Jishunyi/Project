/******************************************************************************/
/**\file user.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-23 15:03:00
 ******************************************************************************/
 
 /******************************************************************************
 * Include files
 ******************************************************************************/
 #include "user.h"
 
 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
//CAN
stc_can_datafromctr DataFromCtr; 							//控制柜传入的指令 							//已解析
uint32_t IDFromCtrBuffer; 										//控制柜传入的指令 ID						//已接收
uint8_t DataFromCtrBuffer[DataFromCtrLen]; 		//控制柜传入的指令 数据					//已接收

uint8_t DataToCtrBuffer[DataToCtrLen]; 				//回传到控制柜的指令数据					//待发送
uint8_t DataFromCANFlag = FALSE ; 						//回传到控制柜的指令的标志位

//UART
stc_uart_datatompu_t DataToMPU = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0x10, 0x00}; 		//控制显示屏的指令 //已解析
uint8_t DataToMPUBuffer[DataToMPULen]; 				//控制显示屏的指令 							//待发送
uint8_t DataFromMPUBuffer[DataFromMPULen]; 		//显示屏回传的指令	数据					//已接收
uint8_t DataFromMPUFlag =FALSE ; 							//显示屏回传指令的标志位

//解析模式
uint32_t ParseModeAuto_i = 0;									//自动演示计数

 /*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//系统初始化
void System_Init(void)
{		
		//系统时钟设置
		//System_ClkInit();		//系统时钟切换为外部时钟后 CAN收发正常 UART收发错乱
		//通讯端口初始化
		System_PortInit();
		//看门狗初始化
		System_WdtInit();
		//LED 端口初始化
    System_LEDInit();
}

//系统时钟设置
void System_ClkInit(void)
{
	AHB_Clock_PortCfg();		/*初始化时钟AHB_CLK输出引脚*/
	App_ClkInit();
	Rch4MHzToXth12MHz();			/*时钟源切换(RCH->XTH)*/
	Xth12MHzToPll48MHz();		/*xth作为时钟源，经过PLL倍频48MHz*/

	//使能HCLK从PA01输出 测试HCLK用
	Gpio_SfHClkOutputCfg(GpioSfHclkOutEnable, GpioSfHclkOutDiv1);
}

//端口配置 设置AHB时钟输出口
void AHB_Clock_PortCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 开启GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    

    ///<========================== 时钟输出端口模式配置 ===========================
    ///< 端口方向配置->输出
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvH;
    ///< 端口上下拉配置->无上下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< GPIO IO PA01初始化
    Gpio_Init(GpioPortA, GpioPin1, &stcGpioCfg);    
    ///< 配置PA01复用功能为HCLK输出
    Gpio_SetAfMode(GpioPortA, GpioPin1, GpioAf6);
}

//时钟初始化配置 启动已经默认使用内部4MHzRCH，这里无需再次配置
void App_ClkInit(void)
{
    stc_sysctrl_clk_cfg_t stcCfg;
    
    ///< 开启FLASH外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);
    
    ///<========================== 时钟初始化配置 ===================================
    ///< 因要使用的时钟源HCLK小于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    ///< 时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    ///< 选择内部RCH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    ///< HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    ///< PCLK 为HCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    ///< 系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
}

//将时钟从RCH4MHz切换至XTH12MHz
void Rch4MHzToXth12MHz(void)
{    
	/*使能Flash的时钟*/
	Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);
	
	//======================== 将时钟从RCH4MHz切换至XTH8MHz ==============================    
    //因要使用的时钟源HCLK将小于24M：此处设置FLASH 读等待周期为1 cycle(默认值也为1 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    //切换时钟前（根据外部高速晶振）设置XTH频率范围,配置晶振参数，使能目标时钟，此处为8MHz
    Sysctrl_SetXTHFreq(SysctrlXthFreq6_12MHz);
    Sysctrl_XTHDriverCfg(SysctrlXtalDriver3);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
    delay1ms(10);
    
    //时钟切换
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
	
    //根据需要选择是否关闭原时钟（此处不关闭）
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, FALSE);
}

//PLL倍频
void Xth12MHzToPll48MHz (void)
{
    stc_sysctrl_pll_cfg_t stcPLLCfg;
    DDL_ZERO_STRUCT(stcPLLCfg);	
	
	//======================== 将时钟从XTH8MHz切换至PLL48MHz ==============================    
    //倍频后的时钟源HCLK大于48M：此处设置FLASH 读等待周期为2 cycle(前面已经配置，此处无需重复配置)
    Flash_WaitCycle(FlashWaitCycle1);
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq6_12MHz;    //XTH 12MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL 输出48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllXthXtal;          //输入时钟源选择XTH
    stcPLLCfg.enPllMul    = SysctrlPllMul4;             //12MHz x 4 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
	
    //时钟切换
    Sysctrl_SysClkSwitch(SysctrlClkPLL);   
	
	//时钟分频设置
	Sysctrl_SetHCLKDiv(SysctrlHclkDiv1);
	Sysctrl_SetPCLKDiv(SysctrlPclkDiv1);
}

//通讯端口初始化
void System_PortInit(void)
{
		//Uart1
		App_Uart1Init(UART1Baud);
	
		//CAN
		App_CANInit(CANBaud);				//CAN初始化设置		//传入波特率
}

//看门狗初始化
void System_WdtInit(void) 
{
		App_WdtInit(WdtT1s64);	//看门狗参数配置
		Wdt_Start(); 		//启动看门狗
}

//LED初始化
void System_LEDInit(void)
{
		App_LEDInit();	//LED参数配置
}

void ParseFromCtr(uint32_t Mode)	//解析控制柜传入数据
{	
		if(Mode == ParseModeNoml)	//常规模式
		{
				if(DataFromCANFlag == TRUE) //接收到数据
				{
					App_CANSend(IDFromCtrBuffer,DataFromCtrBuffer,DataToCtrLen);	//CAN发送数据
					DataFromCANFlag = FALSE;
				}
		}
		else if(Mode == ParseModeAuto)	//自动演示模式
		{
				if(ParseModeAuto_i < 16)
				{
						
						ParseModeAuto_i++;
				}
		
		}
}

//发送指令到显示屏
void SendToMPU(void)
{
		memcpy((void *)DataToMPUBuffer, (void *)&DataToMPU, DataToMPULen); //装载待发送数据
		App_UartSend(M0P_UART1, DataToMPUBuffer, DataToMPULen); //UART1发送
}

//解析显示屏回传数据
void ParseFromMPU(void)
{
		if(DataFromMPUFlag == TRUE)	//接收到数据
		{		
				App_UartSend(M0P_UART1, DataFromMPUBuffer, DataFromMPULen); //UART1发送
				DataFromMPUFlag = FALSE;
		}			
}

void SendToCtr(void)	//回传数据到控制柜
{

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
