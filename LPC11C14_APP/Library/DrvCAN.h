/*******************************************************************************
	Copyright (C), 2010-2015, BST Electric Co., Ltd.
	File name: DrvCAN.h
	Author: Yong Hao Version: V0 Date: 22. March. 2015
	Description: 
    DrvCAN.c的头文件，扩展了可供其他文件使用的全局变量的作用域，提供了
	可供其他文件使用的函数声明。
	Others: 
	Function List: 
	1. void InitCan(unsigned int BaudRate);
	2. void CAN_Send(uint8_t msg_no, uint8_t received_flag, uint32_t *msg_ptr);
	
	History: 
	1.
		Date:
		Author:
		Modification:
	2. 
******************************************************************************/

#ifndef _DRVCAN_H_
#define _DRVCAN_H_

//* Public include files ------------------------------------------------------
//* @{公共头文件

#include "DrvSysClk.h"
#include "DrvGPIO.h"

//**
// * @}	*/

//* Public Macros/Configuration and Data type --------------------------------
//* @{公共宏/配置和数据定义区域(源文件中可供外部文件使用的宏/配置和数据)

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define CANTX_PIN	GPIO_Pin_12
#define CANRX_PIN	GPIO_Pin_11
#define CAN_GPIO	GPIOA

#define INIT			0
#define START_E0		1
#define ONETOZERO		2
#define SEND_F0			3
#define RECEIVEE1		4
#define SEND_F1			5
#define RECEIVEE2		6
#define TECAHIN_OVER	7
#define RECEIVEE3		8

#define MAIN_SIDE	0x04
#define REAL_SIDE	0x08

#define SUCCEED	    1
#define FAIL		0

#define CAN_DEBUG				0

/* Only below flag is set, it uses the default setting 
in the CAN bittiming configuration value. In this case, 
CAN clock should be 8MHz CAN clock bitrate should be 500kbps. */
#define USE_DEFAULT_BIT_TIMING  0

/* Data structure for a CAN message */
typedef struct
{
	unsigned int    	ext;	/* 0--标准帧，1--扩展帧 */
	unsigned int			id;
	unsigned int 			dlc;
	unsigned short		data[4];
} Can_Message_Object;

#define DLC_MAX				8

#define RX_MSG_ID			0x600
#define RX_EXT_MSG_ID		0x00100000

#define TX_MSG_ID			0x200
#define TX_EXT_MSG_ID		0x00200000

/* BRP+1 = Fpclk/(CANBitRate * QUANTAValue)
   QUANTAValue = 1 + (Tseg1+1) + (Tseg2+1)
   QUANTA value varies based on the Fpclk and sample point
   e.g. (1) sample point is 87.5%, Fpclk is 48Mhz
   the QUANTA should be 16
        (2) sample point is 90%, Fpclk is 12.5Mhz
   the QUANTA should be 10 
   Fpclk = Fclk /APBDIV
   or
   BitRate = Fclk/(APBDIV * (BRP+1) * ((Tseg1+1)+(Tseg2+1)+1))
*/ 	
/*
 * Bit Timing Values for 24MHz clk frequency
 */
/*    LPC_CAN->BT = 0x0101;	*/	/* 1Mbps with 8M sysclk */
/*    LPC_CAN->BT = 0x4501;	*/	/* 1Mbps with 24M sysclk */
/*    LPC_CAN->BT = 0x5801;	*/	/* 500kbps with 16M sysclk */
/*    LPC_CAN->BT = 0x5803;	*/	/* 250kbps with 16M sysclk */

#define BITRATE100K8MHZ           0x00000113
#define BITRATE125K8MHZ           0x0000010F
#define BITRATE250K8MHZ           0x00000107
#define BITRATE500K8MHZ           0x00000103
#define BITRATE1000K8MHZ          0x00000101
#define BITRATE20K16MHZX           0x0000572E
#define BITRATE20K16MHZ           0x00005831
#define BITRATE25K16MHZ           0x00005827   /* BRP:39,SJW:0,TSEG1:8,TSEG2:5 */
#define BITRATE27_6K16MHZ         0x00001c23   /* 27.778k(BRP:35,SJW:0,TSEG1:12,TSEG2:1) */
#define BITRATE31_25K16MHZ		  0x0000581F   /* 31.25K???,???Artemis?????(BRP:31,SJW:0,TSEG1:8,TSEG2:5) */	
#define BITRATE33_3K16MHZ         0x0000581D   /* 33.33K???,???Artemis?????(BRP:29,SJW:0,TSEG1:8,TSEG2:5)*/
#define BITRATE50K16MHZ           0x00005813   /* BRP:19,SJW:0,TSEG1:8,TSEG2:5 */
#define BITRATE100K16MHZ          0x00005809   
#define BITRATE125K16MHZ          0x00005807
#define BITRATE250K16MHZ          0x00005803
#define BITRATE500K16MHZ          0x00005801

#define BITRATE100K24MHZ          0x00007E09
#define BITRATE125K24MHZ          0x0000450F
#define BITRATE250K24MHZ          0x00004507
#define BITRATE500K24MHZ          0x00004503
#define BITRATE1000K24MHZ         0x00004501


#define CAN_STATUS_INTERRUPT      0x8000    /* 0x0001-0x0020 are the # of the message 
										    object */
                                            /* 0x8000 is the status interrupt */

/* CAN Message interface register definitions */
/* bit field of IF command request n register */
#define IFCREQ_BUSY               0x8000    /* 1 is writing is progress, cleared when 
                                            RD/WR done */
/* CAN CTRL register */
#define CTRL_INIT		(1 << 0)
#define CTRL_IE			(1 << 1) 
#define CTRL_SIE		(1 << 2)
#define CTRL_EIE		(1 << 3) 
#define CTRL_DAR		(1 << 5)
#define CTRL_CCE		(1 << 6) 
#define CTRL_TEST		(1 << 7)
#define CTRL_RXIE		(1 << 10)
	
/* CAN Status register */
#define STAT_LEC		(0x7 << 0)
#define STAT_TXOK		(1 << 3) 
#define STAT_RXOK		(1 << 4)
#define STAT_EPASS		(1 << 5) 
#define STAT_EWARN		(1 << 6)
#define STAT_BOFF		(1 << 7)

/* bit field of IF command mask register */
#define	DATAB	(1 << 0)   /* 1 is transfer data byte 4-7 to message object, 0 is not */ 
#define	DATAA	(1 << 1)   /* 1 is transfer data byte 0-3 to message object, 0 is not */ 
#define	TREQ	(1 << 2)   /* 1 is set the TxRqst bit, 0 is not */
#define	INTPND	(1 << 3)       
#define	CTRLA	(1 << 4)   /* 1 is transfer the CTRL bit to the message object, 0 is not */
#define	ARB		(1 << 5)   /* 1 is transfer the ARB bits to the message object, 0 is not */
#define	MASK	(1 << 6)   /* 1 is transfer the MASK bit to the message object, 0 is not */
#define	WR		(1 << 7)   /* 0 is READ, 1 is WRITE */
#define RD      0x0000

/* bit field of IF mask 2 register */
#define	MASK_MXTD	(1 << 15)     /* 1 extended identifier bit is used in the RX filter unit, 0 is not */ 
#define	MASK_MDIR	(1 << 14)     /* 1 direction bit is used in the RX filter unit, 0 is not */

/* bit field of IF identifier 2 register */
#define	ID_MVAL		(1 << 15)     /* Message valid bit, 1 is valid in the MO handler, 0 is ignored */ 
#define	ID_MTD		(1 << 14)     /* 1 extended identifier bit is used in the RX filter unit, 0 is not */ 
#define	ID_DIR		(1 << 13)     /* 1 direction bit is used in the RX filter unit, 0 is not */

/* bit field of IF message control register */
#define	NEWD		(1 << 15)     /* 1 indicates new data is in the message buffer.  */ 
#define	MLST		(1 << 14)     /* 1 indicates a message loss. */ 
#define	INTP		(1 << 13)     /* 1 indicates message object is an interrupt source */
#define UMSK    	(1 << 12)     /* 1 is to use the mask for the receive filter mask. */
#define	TXIE		(1 << 11)     /* 1 is TX interrupt enabled */ 
#define	RXIE		(1 << 10)     /* 1 is RX interrupt enabled */
#define	ROEN		(1 << 9)      /* 1 is remote frame enabled */
#define TXRQ    	(1 << 8)      /* 1 is TxRqst enabled */
#define	EOB			(1 << 7)      /* End of buffer, always write to 1 */ 
//#define	DLC			0x000F        /* bit mask for DLC */ 

#define ID_STD_MASK		0x07F8		
#define ID_EXT_MASK		0x1FFFFFFF
#define DLC_MASK		0x0F

typedef struct
{
	unsigned int StdId;
	unsigned int ExtId;
	unsigned char IDE;
	unsigned char RTR;
	unsigned char DLC;
	unsigned char Data[8];
	unsigned char FMI;
	unsigned char flag;
} CanRxMsg;

void CAN_IRQHandler(void);
void CAN_MessageProcess(uint8_t msgno);
void InitRecvObject(unsigned char num, unsigned int RecvID, unsigned char Extended);

//**
// * @}	

//* Public function declaration -----------------------------------------------
//* @{公共函数声明区域(可供外部文件使用)

typedef struct{
	unsigned int 		mID;
	unsigned int 		mIsExtend;
} T_CAN_ID;

void CANOpen(unsigned int pBaudRate, T_CAN_ID *pRecvIDs, unsigned int pRecvIDNum);
void CANClose(void);
void CANSend(Can_Message_Object *pMessage);
void Callback_CanReceived(Can_Message_Object* pData);

//**
// * @}	

#endif //_DRVCAN_H_ 
