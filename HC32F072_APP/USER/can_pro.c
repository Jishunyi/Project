/******************************************************************************/
/**\file can_pro.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-18 15:03:00
 ******************************************************************************/

#include "can_pro.h"

/******************************************************************************
 * variable definitions                                      									*
 ******************************************************************************/
stc_can_rxframe_t       stcRxFrame;
stc_can_txframe_t       stcTxFrame;
uint8_t                 u8RxFlag = FALSE;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 //CAN�жϺ���
void Can_IRQHandler(void)
{
    if(TRUE == CAN_IrqFlgGet(CanRxIrqFlg))
    {
        CAN_IrqFlgClr(CanRxIrqFlg);
        CAN_IrqCmd(CanRxIrqEn, FALSE);

        CAN_Receive(&stcRxFrame);

        u8RxFlag = TRUE;
    }
}

//< ϵͳʱ�ӳ�ʼ��(8MHz for CanClk)
void App_SysClkInit(void)
{
    ///< �л�ʱ��ǰ�������ⲿ���پ�������XTHƵ�ʷ�Χ,���þ��������ʹ��Ŀ��ʱ�ӣ��˴�Ϊ8MHz
    Sysctrl_SetXTHFreq(SysctrlXthFreq6_12MHz);
    Sysctrl_XTHDriverCfg(SysctrlXtalDriver3);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
    delay1ms(10);

    ///< ʱ���л�
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
}

//< CAN GPIO ����
void App_CanGpioInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    ///< �˿ڷ�������->����
    stcGpioCfg.enDir = GpioDirIn;
    ///< �˿�������������->����������
    stcGpioCfg.enDrv = GpioDrvL;
    ///< �˿�����������->��
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< �˿ڿ�©�������->��©����ر�
    stcGpioCfg.enOD = GpioOdDisable;
    ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;

    Gpio_Init(EVB_CAN_RX_PORT, EVB_CAN_RX_PIN, &stcGpioCfg);
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(EVB_CAN_TX_PORT, EVB_CAN_TX_PIN, &stcGpioCfg);
    Gpio_Init(EVB_CAN_STB_PORT, EVB_CAN_STB_PIN, &stcGpioCfg);

    ///<CAN RX\TX���ù�������
    Gpio_SetAfMode(EVB_CAN_RX_PORT, EVB_CAN_RX_PIN, GpioAf1);
    Gpio_SetAfMode(EVB_CAN_TX_PORT, EVB_CAN_TX_PIN, GpioAf1);

    ///<STB ��-PHY��Ч
    Gpio_ClrIO(EVB_CAN_STB_PORT, EVB_CAN_STB_PIN);
}

//< CAN ��ʼ������
void App_CanInit(void)
{
    stc_can_init_config_t   stcCanInitCfg;
    stc_can_filter_t        stcFilter;


    Sysctrl_SetPeripheralGate(SysctrlPeripheralCan, TRUE);

    //<<CAN ����������
    stcCanInitCfg.stcCanBt.PRESC = 1-1;
    stcCanInitCfg.stcCanBt.SEG_1 = 5-2;
    stcCanInitCfg.stcCanBt.SEG_2 = 3-1;
    stcCanInitCfg.stcCanBt.SJW   = 3-1;

    stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 10;
    stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 16-1;

    stcCanInitCfg.enCanRxBufAll  = CanRxNormal;
    stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;
    stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;

    CAN_Init(&stcCanInitCfg);

    //<<CAN �˲�������
    stcFilter.enAcfFormat = CanAllFrames;
    stcFilter.enFilterSel = CanFilterSel1;
    stcFilter.u32CODE     = 0x00000352;
    stcFilter.u32MASK     = 0x1FFFFFFF;
    CAN_FilterConfig(&stcFilter, TRUE);


    //<<Can Irq Enable
    CAN_IrqCmd(CanRxIrqEn, TRUE);

    EnableNvic(CAN_IRQn, IrqLevel0, TRUE);

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
