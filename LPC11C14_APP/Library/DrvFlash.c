/*****************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: DrvFlash.c
	Author: Zhang zhiwei Version: V0 Date: 6. March. 2015
	Description: Flash驱动功能模块
	Version: V0
	Function List: 

	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/
//* Private include files -----------------------------------------------------
//* @{引用头文件声明区域(仅限本文件内使用)
#include "DrvFlash.h"
#include "LPC11xx.h"
//**
// * @}

//* Private Macros and Data type -----------------------------------------------
//* @{私有宏和数据定义区域(仅限本文件内使用的全局变量和宏)
/*command declaration*/
#define IAP_PREPARE 50	/*IAP准备命令*/
#define IAP_COPY	51	/*IAP复制命令*/
#define IAP_ERASURE	52	/*IAP擦除命令*/
#define IAP_CHECK	53	/*IAP检测命令*/
#define IAP_READID	54	/*IAP读ID命令*/
#define IAP_READBOOT 55	/*IAP读BOOT命令*/
#define IAP_COMPARE	56	/*IAP比较命令*/
#define IAP_RESULT	57	/*IAP复位命令*/
#define IAP_READUID	58	/*IAP读UID命令*/
/*status code*/
#define CMD_SUCCESS                                0	/*操作成功*/
#define INVALID_COMMAND                            1	/*无效命令*/
#define SRC_ADDR_ERROR                             2 	/*源地址错误*/
#define DST_ADDR_ERROR                             3	/*目的地址错误*/
#define SRC_ADDR_NOT_MAPPED                        4	/*源地址无映射*/
#define DST_ADDR_NOT_MAPPED                        5	/*目的地址无映射*/
#define COUNT_ERROR                                6	/*读写个数错误*/
#define INVALID_SECTOR                             7	/*无效扇区*/
#define SECTOR_NOT_BLANK                           8	/*扇区不为空*/
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION    9	/*扇区写操作未准备*/
#define COMPARE_ERROR                              10	/*比较错误*/
#define BUSY                                       11	/*FLASH忙*/
/*IAP entery Adr*/
#define IAP_LOCATION 0x1FFF1FF1
/*函数指针类型重定义为IAP*/
typedef void (*IAP)(unsigned long Cmd[],unsigned long Res[]);
/*IAP操作频率*/
#define  IAP_FCCLK            (uSystemFrequency/1000)

IAP Iap_Entry = (IAP)IAP_LOCATION;
unsigned long gParamIn[5];	/*IAP输入参数*/
unsigned long gParamOut[5];	/*IAP输出参数*/

unsigned long* SectorPrepare(const unsigned long Sec1,const unsigned long Sec2)
{
	gParamIn[0] = IAP_PREPARE;
	gParamIn[1] = Sec1;
	gParamIn[2] = Sec2;
	Iap_Entry(gParamIn,gParamOut);	
	return gParamOut;
}

unsigned long* RamToFalsh(const unsigned long Dst,const unsigned long Src,const unsigned long No)
{
	gParamIn[0] = IAP_COPY;
	gParamIn[1] = Dst;
	gParamIn[2] = Src;
	gParamIn[3] = No;
	gParamIn[4] = IAP_FCCLK;
	Iap_Entry(gParamIn,gParamOut);
	return gParamOut;
}
unsigned long  blankChk (uint8_t sec1, uint8_t sec2)
{  
    gParamIn[0] = IAP_CHECK;                                          /* 设置命令字                   */
    gParamIn[1] = sec1;                                                  /* 设置参数                     */
    gParamIn[2] = sec2;
    (*Iap_Entry)(gParamIn, gParamOut);                                    /* 调用IAP服务程序              */

    return (gParamOut[0]);                                               /* 返回状态码                   */
}





unsigned long* SectorErasure(const unsigned long Sec1,const unsigned long Sec2)
{
	gParamIn[0] = IAP_ERASURE;
	gParamIn[1] = Sec1;
	gParamIn[2] = Sec2;
	gParamIn[3] = IAP_FCCLK;
	Iap_Entry(gParamIn,gParamOut);
	return gParamOut;
}

#define VTOR_OFFSET         (SECTOR2)   //APP的异常中断向量表位置
void ReAllocateNVIC(void)  //将异常中断向量表从Flash映射到RAM上
{
    uint32_t *src, *dst;
    int32_t size;
    __disable_irq();
    src  = (uint32_t *)VTOR_OFFSET;
    dst  = (uint32_t *)0x10000000;  //RAM基地址
    size = 192;
		/* copy vector table */
    while (size > 0)
    {
        *dst++ = *src++;
        size -= 4;
    }
     LPC_SYSCON->SYSMEMREMAP = 0x1;    /* remap to internal RAM */
    __enable_irq();
}

