/******************************************************************************/
/**\file wdt_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-23 15:03:00
 ******************************************************************************/
 
 /******************************************************************************
 * Include files
 ******************************************************************************/
 #include "wdt_pro.h"
 
 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t u32CountWdt;
uint8_t u8Flag=0xFF;

 /*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 //WDT 中断服务程序
void Wdt_IRQHandler(void)
{
		if(Wdt_GetIrqStatus())
    {
        Wdt_IrqClr();       ///<清除 wdt 中断标记
        
        u8Flag = ~u8Flag;
        if(u8Flag)
        {
            Gpio_WriteOutputIO(EVB_LEDG_PORT, EVB_LEDG_PIN, TRUE);
        }
        else
        {
            Gpio_WriteOutputIO(EVB_LEDG_PORT, EVB_LEDG_PIN, FALSE);
        }
				u32CountWdt++;
    }
}

//看门狗初始化
void App_WdtInit(en_wdt_time_t Time)
{
    //开启WDT外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
    //WDT 初始化
		Wdt_Init(WdtResetEn, Time); 				//超时未喂狗则触发复位
		//Wdt_Init(WdtIntEn, WdtT51ms); 						//超时未喂狗则触发中断
		//EnableNvic(WDT_IRQn, IrqLevel3, TRUE); 		//开启NVIC中断
}
 
 /******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
