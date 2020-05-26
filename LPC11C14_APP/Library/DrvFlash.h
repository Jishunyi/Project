/*******************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: DrvFlash.h
	Author: zhang zhiwei Version: V0 Date: 6. March. 2015
	Description: 
      �ṩFlash������д�ȹ��ܲ����Լ�IAP��ز�������
	Others: 
	Function List: 
        1-SectorPrepare()   ׼������д����������
        2-RamToFalsh()      ����RAM���ݵ�FLASH
        3-SectorErasure()   ��������
        4-SectorCheck()     ��������
        5-IapReadID()       ������ID
        6-IapReadBoot()     ��Boot����汾��
        7-IapCompare()      IAP�Ƚ�
        8-IapResult()       �ص���ISP
        9-IapReadUID()      IAP��UID
	History: 
	1.
		Date:
		Author:
		Modification:
	2. 
******************************************************************************/
#ifndef _DRVFLASH_H_
#define _DRVFLASH_H_
//* Public include files ------------------------------------------------------
//* @{����ͷ�ļ�

#include "LPC11xx.h"

extern uint32 uSystemFrequency;

#define SECTOR0	0x00000000
#define SECTOR1	0x00001000
#define SECTOR2	0x00002000
#define SECTOR3	0x00003000
#define SECTOR4 0x00004000
#define SECTOR5 0x00005000
#define SECTOR6 0x00006000
#define SECTOR7	0x00007000

#define D_PARAMETER_ADDR		0x000007c00
#define D_PARAMETER_SECTOR	7

#define FlashData(addr)   (*(unsigned int *)( addr))

unsigned long* SectorPrepare(const unsigned long Sec1,const unsigned long Sec2);
unsigned long* RamToFalsh(const unsigned long Dst,const unsigned long Src,const unsigned long No);
unsigned long* SectorErasure(const unsigned long Sec1,const unsigned long Sec2);
unsigned long  blankChk (uint8_t sec1, uint8_t sec2);
void ReAllocateNVIC(void);

//**
// * @}	

#endif //_DRVFLASH_H_ 
