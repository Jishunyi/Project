/******************************************************************************/
/**\file wdt_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-18 15:03:00
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
 //WDT �жϷ������
void Wdt_IRQHandler(void)
{
		if(Wdt_GetIrqStatus())
    {
        Wdt_IrqClr();       ///<��� wdt �жϱ��
        
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

//���Ź���ʼ��
void App_WdtInit(void)
{
    //����WDT����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
    //WDT ��ʼ��
		Wdt_Init(WdtResetEn, WdtT820ms); 				//��ʱδι���򴥷���λ
		//Wdt_Init(WdtIntEn, WdtT51ms); 						//��ʱδι���򴥷��ж�
		//EnableNvic(WDT_IRQn, IrqLevel3, TRUE); 		//����NVIC�ж�
}
 
 /******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
