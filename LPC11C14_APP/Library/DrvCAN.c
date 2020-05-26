/*****************************************************************************
	Copyright (C), 2010-2015, BST Electric Co., Ltd.
	File name: DrvCAN.c
	Author: Yong Hao Version: V0 Date: 22. April. 2015
	Description: CAN driver
	Version: V0
	Function List: 
	1. void InitCan(unsigned int BaudRate);
	2. void CAN_IRQHandler(void);
	3. void CAN_MessageProcess(uint8_t msgno);
	4. void CAN_Send(uint8_t msg_no, uint8_t received_flag, uint32_t *msg_ptr);
	
	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/

#include "DrvCAN.h"
#include "logic.h"

/* WARN计数 */
volatile uint32_t EWarnCnt = 0;

/* bus-off计数 */
volatile uint32_t BOffCnt = 0;

/* 对当前使用的波特率进行备份，在数据出错时，重新初始化使用 */
unsigned int 	gBaudRateBackUp = 0;
T_CAN_ID 		 *gRecvIDs = 0;
unsigned int  gRecvIDNum = 0;

void CANOpen(unsigned int pBaudRate, T_CAN_ID *pRecvIDs, unsigned int pRecvIDNum)
{
	unsigned int i;
	gBaudRateBackUp = pBaudRate;

	if(pRecvIDNum != gRecvIDNum)
	{
		if(gRecvIDs)
		{
			free(gRecvIDs);
			gRecvIDs = 0;
		}
		gRecvIDNum = pRecvIDNum;
	}
	if(!gRecvIDs)
		gRecvIDs = (T_CAN_ID *)malloc(sizeof(T_CAN_ID)*gRecvIDNum);
	if(gRecvIDNum > 0)
	{
		memcpy((void*)gRecvIDs, (void*)pRecvIDs, gRecvIDNum*sizeof(T_CAN_ID));
	}	
	
	/* 确定CAN模块没有重启 */
	LPC_SYSCON->PRESETCTRL |= 0x1 << 3;
	
	/* 打开CAN模块 */
	LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 17;
	
	/* 使CAN模块从正常模式切换到初始化模式 */
	if (!(LPC_CAN->CNTL & CTRL_INIT))
	{
		LPC_CAN->CNTL |= CTRL_INIT;
	}
	
	/* 分频值为3，填入寄存器的值需要减1 */
	LPC_CAN->CLKDIV = 0x02;

	/* 开始配置bit timing */
	LPC_CAN->CNTL |= CTRL_CCE;
	
	/* 设定波特率*/
	switch (pBaudRate)
	{
		case 20000:
			LPC_CAN->BT = BITRATE20K16MHZ;
			break;
		case 25000:
			LPC_CAN->BT = BITRATE25K16MHZ;
			break;
		case 27777:
			LPC_CAN->BT = BITRATE27_6K16MHZ;
			break;
		case 33333:
			LPC_CAN->BT = BITRATE33_3K16MHZ;
			break;
		case 50000:
			LPC_CAN->BT = BITRATE50K16MHZ;
			break;
		case 100000:
			LPC_CAN->BT = BITRATE100K16MHZ;
			break;
		case 125000:
			LPC_CAN->BT = BITRATE125K16MHZ;
			break;
		case 250000:
			LPC_CAN->BT = BITRATE250K16MHZ;
			break;
		default:
			LPC_CAN->BT = BITRATE20K16MHZ;	/* 默认20K */
	}
	
	LPC_CAN->BRPE = 0x00;
	
	/* 停止配置bit timing */
	LPC_CAN->CNTL &= ~CTRL_CCE;
	
	/* 使CAN模块从初始化模式切换回正常模式 */
	LPC_CAN->CNTL &= ~CTRL_INIT;
	while (LPC_CAN->CNTL & CTRL_INIT)
	{
		;
	}
	
	for(i=0;i<gRecvIDNum;i++)
	{
		InitRecvObject(i+1, gRecvIDs[i].mID, gRecvIDs[i].mIsExtend);
	}
	
	NVIC_EnableIRQ(CAN_IRQn);
	NVIC_SetPriority(CAN_IRQn, 0);
	
	/* 开其他相关中断 */
	LPC_CAN->CNTL |= CTRL_IE | CTRL_EIE;
}

void CANClose(void)
{
		if(gRecvIDs)
		{
			free(gRecvIDs);
			gRecvIDs = 0;
		}
		gRecvIDNum = 0;
}

void CANSend(Can_Message_Object *pMessage)
{
	if(pMessage == 0)
		return;
	
	/* 要发送的数据长度，不能超过8个字节 */
	if (pMessage->dlc > DLC_MAX)
	{
		pMessage->dlc = DLC_MAX;	
	}
	
	if (pMessage->ext)
	{
		/* Mxtd: 1, Mdir: 1, Mask: 0x1FFFFFFF（扩展帧全部掩码） */
		LPC_CAN->IF1_MSK1 = 0xFFFF;
		LPC_CAN->IF1_MSK2 = 0x1FFF | MASK_MDIR | MASK_MXTD;
		
		/* MsgVal: 1, Mtd: 1, Dir: 1 */
		/* 由于Mask设定为全部掩码，因此发送ID只能为msg_ptr->id */	
		LPC_CAN->IF1_ARB1 = pMessage->id & 0xFFFF;
		LPC_CAN->IF1_ARB2 = ID_MVAL | ID_DIR | ID_MTD | ((pMessage->id >> 16) & 0x1FFF);				
	}
	else
	{
		/* Mxtd: 0, Mdir: 1, Mask: 0x7FF（标准帧全部掩码） */
		LPC_CAN->IF1_MSK1 = 0x0000;
		LPC_CAN->IF1_MSK2 = (0x07FF << 2) | MASK_MDIR;				/* IF1_MSK2的低两位对标准帧无效，所以左移两位 */
		
		/* MsgVal: 1, Mtd: 0, Dir: 1 */
		/* 由于Mask设定为全部掩码，因此发送ID只能为msg_ptr->id */	
		LPC_CAN->IF1_ARB1 = 0x0000;
		LPC_CAN->IF1_ARB2 = ID_MVAL | ID_DIR | (pMessage->id << 2);	/* IF1_ARB2的低两位对标准帧无效，所以左移两位 */			
	}
	
	LPC_CAN->IF1_MCTRL = UMSK | TXRQ | EOB | (pMessage->dlc & DLC_MASK);
	
	/* 填充要发送的数据 */
	LPC_CAN->IF1_DA1 = pMessage->data[0];
	LPC_CAN->IF1_DA2 = pMessage->data[1];
	LPC_CAN->IF1_DB1 = pMessage->data[2];
	LPC_CAN->IF1_DB2 = pMessage->data[3];
	
	/* 设定要发送的信息，并发送命令请求 */
	LPC_CAN->IF1_CMDMSK = WR | MASK | ARB | CTRLA | TREQ | DATAA | DATAB;  //设置IF1为写通道
	LPC_CAN->IF1_CMDREQ = 19; //默认选报文编号为1的RAM进行发送数据，如果处理不过来，可以做成队列，范围为0x01~0x20
	while (LPC_CAN->IF1_CMDREQ & IFCREQ_BUSY) {}
}

/*******************************************************************************
	Function:			CAN_IRQHandler
	Description:		CAN接收中断处理函数
	Input:				None
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/

Can_Message_Object  			gCurCanFrame;

void CAN_IRQHandler(void)
{
	uint32_t canstat, can_int, msg_no;
	uint8_t  ext;
	while ((can_int = LPC_CAN->INT) != 0)
	{
		canstat = LPC_CAN->STAT;
		/* 是否状态中断 */
		if (can_int & CAN_STATUS_INTERRUPT)		{
			if (canstat & STAT_EWARN)			{
				EWarnCnt++;
				return;
			}
			if (canstat & STAT_BOFF)			{
				BOffCnt++;
				return;
			}
		}
		/* 不是状态中断，并且无错误码 */
		else if ((canstat & STAT_LEC) == 0) 	
		{			
			/* 读出当前引发中断的报文对象编号 */
			msg_no = can_int & 0x7FFF;
			if ((msg_no >= 0x01) && (msg_no <= 0x20))
			{
				/* 接收成功，复位该位 */
				LPC_CAN->STAT &= ~STAT_RXOK;
				
				while (LPC_CAN->IF2_CMDREQ & IFCREQ_BUSY) {}
				
				/* 设定要读取的信息，并发送命令请求 */
				LPC_CAN->IF2_CMDMSK = RD | MASK | ARB | CTRLA | INTPND | TREQ | DATAA | DATAB;  //设置IF2为读通道
				LPC_CAN->IF2_CMDREQ = msg_no; 
				while (LPC_CAN->IF2_CMDREQ & IFCREQ_BUSY) {}		
				
				/* 报文数据中的所有信息读出来 */
				gCurCanFrame.ext = ext;
				/* 是否扩展帧 */
				if (LPC_CAN->IF2_ARB2 & ID_MTD)
				{
					ext = 1;
					/* 取出扩展帧ID */
					gCurCanFrame.id = (LPC_CAN->IF2_ARB1 | ((LPC_CAN->IF2_ARB2 & 0x1FFF) << 16));
				}
				else
				{
					ext = 0;
					/* 取出标准帧ID（位于bit12~bit2） */
					gCurCanFrame.id = (LPC_CAN->IF2_ARB2 & 0x1FFF) >> 2;
				}
				gCurCanFrame.dlc = LPC_CAN->IF2_MCTRL & 0x000F;
				gCurCanFrame.data[0] = LPC_CAN->IF2_DA1;
				gCurCanFrame.data[1] = LPC_CAN->IF2_DA2;
				gCurCanFrame.data[2] = LPC_CAN->IF2_DB1;
				gCurCanFrame.data[3] = LPC_CAN->IF2_DB2;
				
				Callback_CanReceived(&gCurCanFrame);
			}
		}
		/* 有错误则重新初始化CAN模块 */
		else
		{
			//CANOpen(gBaudRateBackUp, gRecvIDs, gRecvIDNum);
		}
	}
}

//CAN接收到一帧数据后立即执行Callback_CanReceived，可以在此函数中增加接收时间等逻辑判断
void Callback_CanReceived(Can_Message_Object* pData)
{
	static unsigned int sCanMsgSize = sizeof(Can_Message_Object);
	if(pData)
	{
		RecvRawFrameFromController(E_CONTROLLER_PORT_CAN, (unsigned char *)(pData), sCanMsgSize);
	}
}

/*******************************************************************************
	Function:			InitRecvObject
	Description:		初始化报文对象
	Input:				num--报文对象编号	RecvID--该报文对象接收ID
						Extended--0表示标准帧，1表示扩展帧
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void InitRecvObject(unsigned char num, unsigned int RecvID, unsigned char Extended)
{
	LPC_CAN->IF1_CMDMSK = WR | MASK | ARB | CTRLA | DATAA | DATAB;

	if (Extended)	/* 是否扩展帧 */
	{
		/* Mxtd: 1, Mdir: 1, Mask: 0x1FFFFFFF（扩展帧全部掩码） */
		LPC_CAN->IF1_MSK1 = 0xFFFF;
		LPC_CAN->IF1_MSK2 = 0x1FFF | MASK_MDIR | MASK_MXTD;
		
		/* MsgVal: 1, Mtd: 1, Dir: 0 */
		/* 由于Mask设定为全部掩码，因此接收ID只能为RecvID */	
		LPC_CAN->IF1_ARB1 = RecvID & 0xFFFF;
		LPC_CAN->IF1_ARB2 = ID_MVAL | ID_MTD | ((RecvID >> 16) & 0x1FFF);					
	}
	else
	{
		/* Mxtd: 0, Mdir: 1, Mask: 0x7FF（标准帧全部掩码） */
		LPC_CAN->IF1_MSK1 = 0x0000;
		LPC_CAN->IF1_MSK2 = (0x07FF << 2) | MASK_MDIR;		/* IF1_MSK2的低两位对标准帧无效，所以左移两位 */
		
		/* MsgVal: 1, Mtd: 0, Dir: 0 */
		/* 由于Mask设定为全部掩码，因此接收ID只能为RecvID */	
		LPC_CAN->IF1_ARB1 = 0x0000;
		LPC_CAN->IF1_ARB2 = ID_MVAL | (RecvID << 2);		/* IF1_ARB2的低两位对标准帧无效，所以左移两位 */		
	}
	
	/* 开屏蔽，开接收中断，开EOB，设定数据长度为8字节 */
	LPC_CAN->IF1_MCTRL = UMSK | RXIE | EOB | DLC_MAX;
   
	/* 清空报文数据寄存器 */
	LPC_CAN->IF1_DA1 = 0x0000;
	LPC_CAN->IF1_DA2 = 0x0000;
	LPC_CAN->IF1_DB1 = 0x0000;
	LPC_CAN->IF1_DB2 = 0x0000;

	/* 命令请求 */
	LPC_CAN->IF1_CMDREQ = num;
	while (LPC_CAN->IF1_CMDREQ & IFCREQ_BUSY) {}
}
