/******************************************************************************/
/**\file can_pro.h
 * @Author       : Shunyi
 * @Date         : 2020-06-11 08:33:55
 * @LastEditors  : Shunyi
 * @LastEditTime : 2020-06-30 15:26:42
 ******************************************************************************/
 
#ifndef  __CAN_PRO_H_
#define  __CAN_PRO_H_

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "sysctrl.h"
#include "can.h"
#include "gpio.h"

#include "user.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/*****************************************************************************
 * Function implementation
 ******************************************************************************/
void App_CANInit(uint32_t Mode); //CAN初始化设置		//传入波特率
void App_CANSend(uint32_t ID, const uint8_t *DataSendBuffer, const uint8_t DataSendBufferLen); //CAN发送数据

#endif
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
