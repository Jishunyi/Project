/*****************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: DrvFlash.c
	Author: Zhang zhiwei Version: V0 Date: 6. March. 2015
	Description: Flash��������ģ��
	Version: V0
	Function List: 

	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/
//* Private include files -----------------------------------------------------
//* @{����ͷ�ļ���������(���ޱ��ļ���ʹ��)
#include "DrvFlash.h"
#include "LPC11xx.h"
//**
// * @}

//* Private Macros and Data type -----------------------------------------------
//* @{˽�к�����ݶ�������(���ޱ��ļ���ʹ�õ�ȫ�ֱ����ͺ�)
/*command declaration*/
#define IAP_PREPARE 50	/*IAP׼������*/
#define IAP_COPY	51	/*IAP��������*/
#define IAP_ERASURE	52	/*IAP��������*/
#define IAP_CHECK	53	/*IAP�������*/
#define IAP_READID	54	/*IAP��ID����*/
#define IAP_READBOOT 55	/*IAP��BOOT����*/
#define IAP_COMPARE	56	/*IAP�Ƚ�����*/
#define IAP_RESULT	57	/*IAP��λ����*/
#define IAP_READUID	58	/*IAP��UID����*/
/*status code*/
#define CMD_SUCCESS                                0	/*�����ɹ�*/
#define INVALID_COMMAND                            1	/*��Ч����*/
#define SRC_ADDR_ERROR                             2 	/*Դ��ַ����*/
#define DST_ADDR_ERROR                             3	/*Ŀ�ĵ�ַ����*/
#define SRC_ADDR_NOT_MAPPED                        4	/*Դ��ַ��ӳ��*/
#define DST_ADDR_NOT_MAPPED                        5	/*Ŀ�ĵ�ַ��ӳ��*/
#define COUNT_ERROR                                6	/*��д��������*/
#define INVALID_SECTOR                             7	/*��Ч����*/
#define SECTOR_NOT_BLANK                           8	/*������Ϊ��*/
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION    9	/*����д����δ׼��*/
#define COMPARE_ERROR                              10	/*�Ƚϴ���*/
#define BUSY                                       11	/*FLASHæ*/
/*IAP entery Adr*/
#define IAP_LOCATION 0x1FFF1FF1
/*����ָ�������ض���ΪIAP*/
typedef void (*IAP)(unsigned long Cmd[],unsigned long Res[]);
/*IAP����Ƶ��*/
#define  IAP_FCCLK            (uSystemFrequency/1000)

IAP Iap_Entry = (IAP)IAP_LOCATION;
unsigned long gParamIn[5];	/*IAP�������*/
unsigned long gParamOut[5];	/*IAP�������*/

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
    gParamIn[0] = IAP_CHECK;                                          /* ����������                   */
    gParamIn[1] = sec1;                                                  /* ���ò���                     */
    gParamIn[2] = sec2;
    (*Iap_Entry)(gParamIn, gParamOut);                                    /* ����IAP�������              */

    return (gParamOut[0]);                                               /* ����״̬��                   */
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

#define VTOR_OFFSET         (SECTOR2)   //APP���쳣�ж�������λ��
void ReAllocateNVIC(void)  //���쳣�ж��������Flashӳ�䵽RAM��
{
    uint32_t *src, *dst;
    int32_t size;
    __disable_irq();
    src  = (uint32_t *)VTOR_OFFSET;
    dst  = (uint32_t *)0x10000000;  //RAM����ַ
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

