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

/* WARN���� */
volatile uint32_t EWarnCnt = 0;

/* bus-off���� */
volatile uint32_t BOffCnt = 0;

/* �Ե�ǰʹ�õĲ����ʽ��б��ݣ������ݳ���ʱ�����³�ʼ��ʹ�� */
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
	
	/* ȷ��CANģ��û������ */
	LPC_SYSCON->PRESETCTRL |= 0x1 << 3;
	
	/* ��CANģ�� */
	LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 17;
	
	/* ʹCANģ�������ģʽ�л�����ʼ��ģʽ */
	if (!(LPC_CAN->CNTL & CTRL_INIT))
	{
		LPC_CAN->CNTL |= CTRL_INIT;
	}
	
	/* ��ƵֵΪ3������Ĵ�����ֵ��Ҫ��1 */
	LPC_CAN->CLKDIV = 0x02;

	/* ��ʼ����bit timing */
	LPC_CAN->CNTL |= CTRL_CCE;
	
	/* �趨������*/
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
			LPC_CAN->BT = BITRATE20K16MHZ;	/* Ĭ��20K */
	}
	
	LPC_CAN->BRPE = 0x00;
	
	/* ֹͣ����bit timing */
	LPC_CAN->CNTL &= ~CTRL_CCE;
	
	/* ʹCANģ��ӳ�ʼ��ģʽ�л�������ģʽ */
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
	
	/* ����������ж� */
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
	
	/* Ҫ���͵����ݳ��ȣ����ܳ���8���ֽ� */
	if (pMessage->dlc > DLC_MAX)
	{
		pMessage->dlc = DLC_MAX;	
	}
	
	if (pMessage->ext)
	{
		/* Mxtd: 1, Mdir: 1, Mask: 0x1FFFFFFF����չ֡ȫ�����룩 */
		LPC_CAN->IF1_MSK1 = 0xFFFF;
		LPC_CAN->IF1_MSK2 = 0x1FFF | MASK_MDIR | MASK_MXTD;
		
		/* MsgVal: 1, Mtd: 1, Dir: 1 */
		/* ����Mask�趨Ϊȫ�����룬��˷���IDֻ��Ϊmsg_ptr->id */	
		LPC_CAN->IF1_ARB1 = pMessage->id & 0xFFFF;
		LPC_CAN->IF1_ARB2 = ID_MVAL | ID_DIR | ID_MTD | ((pMessage->id >> 16) & 0x1FFF);				
	}
	else
	{
		/* Mxtd: 0, Mdir: 1, Mask: 0x7FF����׼֡ȫ�����룩 */
		LPC_CAN->IF1_MSK1 = 0x0000;
		LPC_CAN->IF1_MSK2 = (0x07FF << 2) | MASK_MDIR;				/* IF1_MSK2�ĵ���λ�Ա�׼֡��Ч������������λ */
		
		/* MsgVal: 1, Mtd: 0, Dir: 1 */
		/* ����Mask�趨Ϊȫ�����룬��˷���IDֻ��Ϊmsg_ptr->id */	
		LPC_CAN->IF1_ARB1 = 0x0000;
		LPC_CAN->IF1_ARB2 = ID_MVAL | ID_DIR | (pMessage->id << 2);	/* IF1_ARB2�ĵ���λ�Ա�׼֡��Ч������������λ */			
	}
	
	LPC_CAN->IF1_MCTRL = UMSK | TXRQ | EOB | (pMessage->dlc & DLC_MASK);
	
	/* ���Ҫ���͵����� */
	LPC_CAN->IF1_DA1 = pMessage->data[0];
	LPC_CAN->IF1_DA2 = pMessage->data[1];
	LPC_CAN->IF1_DB1 = pMessage->data[2];
	LPC_CAN->IF1_DB2 = pMessage->data[3];
	
	/* �趨Ҫ���͵���Ϣ���������������� */
	LPC_CAN->IF1_CMDMSK = WR | MASK | ARB | CTRLA | TREQ | DATAA | DATAB;  //����IF1Ϊдͨ��
	LPC_CAN->IF1_CMDREQ = 19; //Ĭ��ѡ���ı��Ϊ1��RAM���з������ݣ���������������������ɶ��У���ΧΪ0x01~0x20
	while (LPC_CAN->IF1_CMDREQ & IFCREQ_BUSY) {}
}

/*******************************************************************************
	Function:			CAN_IRQHandler
	Description:		CAN�����жϴ�����
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
		/* �Ƿ�״̬�ж� */
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
		/* ����״̬�жϣ������޴����� */
		else if ((canstat & STAT_LEC) == 0) 	
		{			
			/* ������ǰ�����жϵı��Ķ����� */
			msg_no = can_int & 0x7FFF;
			if ((msg_no >= 0x01) && (msg_no <= 0x20))
			{
				/* ���ճɹ�����λ��λ */
				LPC_CAN->STAT &= ~STAT_RXOK;
				
				while (LPC_CAN->IF2_CMDREQ & IFCREQ_BUSY) {}
				
				/* �趨Ҫ��ȡ����Ϣ���������������� */
				LPC_CAN->IF2_CMDMSK = RD | MASK | ARB | CTRLA | INTPND | TREQ | DATAA | DATAB;  //����IF2Ϊ��ͨ��
				LPC_CAN->IF2_CMDREQ = msg_no; 
				while (LPC_CAN->IF2_CMDREQ & IFCREQ_BUSY) {}		
				
				/* ���������е�������Ϣ������ */
				gCurCanFrame.ext = ext;
				/* �Ƿ���չ֡ */
				if (LPC_CAN->IF2_ARB2 & ID_MTD)
				{
					ext = 1;
					/* ȡ����չ֡ID */
					gCurCanFrame.id = (LPC_CAN->IF2_ARB1 | ((LPC_CAN->IF2_ARB2 & 0x1FFF) << 16));
				}
				else
				{
					ext = 0;
					/* ȡ����׼֡ID��λ��bit12~bit2�� */
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
		/* �д��������³�ʼ��CANģ�� */
		else
		{
			//CANOpen(gBaudRateBackUp, gRecvIDs, gRecvIDNum);
		}
	}
}

//CAN���յ�һ֡���ݺ�����ִ��Callback_CanReceived�������ڴ˺��������ӽ���ʱ����߼��ж�
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
	Description:		��ʼ�����Ķ���
	Input:				num--���Ķ�����	RecvID--�ñ��Ķ������ID
						Extended--0��ʾ��׼֡��1��ʾ��չ֡
	Output:				None
	Return:				None
	Others:				None
********************************************************************************/
void InitRecvObject(unsigned char num, unsigned int RecvID, unsigned char Extended)
{
	LPC_CAN->IF1_CMDMSK = WR | MASK | ARB | CTRLA | DATAA | DATAB;

	if (Extended)	/* �Ƿ���չ֡ */
	{
		/* Mxtd: 1, Mdir: 1, Mask: 0x1FFFFFFF����չ֡ȫ�����룩 */
		LPC_CAN->IF1_MSK1 = 0xFFFF;
		LPC_CAN->IF1_MSK2 = 0x1FFF | MASK_MDIR | MASK_MXTD;
		
		/* MsgVal: 1, Mtd: 1, Dir: 0 */
		/* ����Mask�趨Ϊȫ�����룬��˽���IDֻ��ΪRecvID */	
		LPC_CAN->IF1_ARB1 = RecvID & 0xFFFF;
		LPC_CAN->IF1_ARB2 = ID_MVAL | ID_MTD | ((RecvID >> 16) & 0x1FFF);					
	}
	else
	{
		/* Mxtd: 0, Mdir: 1, Mask: 0x7FF����׼֡ȫ�����룩 */
		LPC_CAN->IF1_MSK1 = 0x0000;
		LPC_CAN->IF1_MSK2 = (0x07FF << 2) | MASK_MDIR;		/* IF1_MSK2�ĵ���λ�Ա�׼֡��Ч������������λ */
		
		/* MsgVal: 1, Mtd: 0, Dir: 0 */
		/* ����Mask�趨Ϊȫ�����룬��˽���IDֻ��ΪRecvID */	
		LPC_CAN->IF1_ARB1 = 0x0000;
		LPC_CAN->IF1_ARB2 = ID_MVAL | (RecvID << 2);		/* IF1_ARB2�ĵ���λ�Ա�׼֡��Ч������������λ */		
	}
	
	/* �����Σ��������жϣ���EOB���趨���ݳ���Ϊ8�ֽ� */
	LPC_CAN->IF1_MCTRL = UMSK | RXIE | EOB | DLC_MAX;
   
	/* ��ձ������ݼĴ��� */
	LPC_CAN->IF1_DA1 = 0x0000;
	LPC_CAN->IF1_DA2 = 0x0000;
	LPC_CAN->IF1_DB1 = 0x0000;
	LPC_CAN->IF1_DB2 = 0x0000;

	/* �������� */
	LPC_CAN->IF1_CMDREQ = num;
	while (LPC_CAN->IF1_CMDREQ & IFCREQ_BUSY) {}
}
