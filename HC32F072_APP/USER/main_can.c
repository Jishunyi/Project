/******************************************************************************/
/**\file main.c
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-16 08:51:37
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "can_pro.h"

/******************************************************************************
 * variable definitions                                      									*
 ******************************************************************************/
extern stc_can_rxframe_t       stcRxFrame;
extern stc_can_txframe_t       stcTxFrame;
extern uint8_t                 u8RxFlag;
 
/*******************************************************************************/
/**\function 	main()
 * \return 		int32_t return value, if needed
 * \note			Can中断接收 查询发送 返回接收到的数据
 ******************************************************************************/
int32_t main(void)
{
    uint8_t u8Idx = 0;

    //< 系统时钟初始化(8MHz for CanClk)
    App_SysClkInit();

    //< CAN GPIO 配置
    App_CanGpioInit();

    //< CAN 初始化配置
    App_CanInit();


    while(1)
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

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
