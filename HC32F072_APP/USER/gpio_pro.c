/******************************************************************************/
/**\file gpio_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-30 15:26:42
 ******************************************************************************/
 
 /******************************************************************************
 * Include files
 ******************************************************************************/
 #include "gpio_pro.h"
 
 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/


 /*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void App_LEDInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    ///< 开启GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); 
    
    ///< LED 初始化
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_WriteOutputIO(EVB_LEDG_PORT,EVB_LEDG_PIN,TRUE); //板载绿色LED //关闭状态
    Gpio_Init(EVB_LEDG_PORT,EVB_LEDG_PIN,&stcGpioCfg);
}
 
 /******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
