/******************************************************************************/
/**\file wdt_pro.h
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-30 15:26:42
 ******************************************************************************/
 
#ifndef  __WDT_PRO_H_
#define  __WDT_PRO_H_

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "wdt.h"
#include "gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/


 /******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/


/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void App_WdtInit(en_wdt_time_t Time); //���Ź���ʼ��

#endif
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
