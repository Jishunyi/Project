/*****************************************************************************
	Copyright (C), 2010-2018, BST Electric Co., Ltd.
	File name: user.c
	Author: Yujinjun 
	Version: V1.0 
	Date: 2019-07-17
	Description: �û��Ǳ��ļ�
	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/

//* Private include files -----------------------------------------------------
//* @{����ͷ�ļ���������(���ޱ��ļ���ʹ��)

#include "user.h"
//��ʼ��data+timeָ�� //���ڿ��ƹ��rtc���ݷֳ�������ָ������ڴ˴��������
T_WRITE_DATAREG RTCSET = {0x00, 0x9C, 0x5A, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //5A A5 09 82 00 9C year_high year_low month day hour min
extern unsigned char RTC_NOW[6];
unsigned char gVersion[32] = "Ver:1.0.0.20191010";
const unsigned char StageSigDefine[][2] = {
	" 0",
	" 1",
	" 2",
	" 3",
	" 4",
	" 5",
	" 6",
	" 7",
	" 8",
	" 9",
	" A",
	" B",
	" G",
	" H",
	" L",
	" M",
	" P",
	" R",
	" -",
	"  ",
	"12",
	"13",
	"23",
	" C",
	" D",
	" E",
	" F",
	" I",
	" J",
	" K",
	" N",
	" O",
	" Q",
	" S",
	" T",
	" U",
	" V",
	" W",
	" X",
	" Y",
	" Z",
	"15",
	"17",
	"19",
};
const unsigned char HextoASCII_Code[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'G', 'H', 'L', 'M', 'P', 'R', '-', ' ',
	' ', ' ', ' ', 'C', 'D', 'E', 'F', 'I', 'J', 'K',
	'N', '0', 'Q', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
	'Z', ' ', ' ', ' '};
void Callback_SysTick(void) //1msϵͳʱ�ӳ�ʱ�жϵĻص�����
{
	if (gSysTickCount % 1000 == 0 && gSendRightNow == 1) //����ͨѶ��ÿ1���ȡһ��ʱ�����
	{
		unsigned char tmpData[3] = {0x00, 0x10, 0x04};
		SendFrameToMPU(0x83, tmpData, 3);
		//���ð汾��
		//			T_WRITE_DATAREG tmpData;
		//			tmpData.mADR_H = (unsigned char)(E_ID_VERSION>>8);
		//			tmpData.mADR_L = (unsigned char)(E_ID_VERSION);
		//			memcpy(tmpData.mDATA, gVersion, strlen((char*)gVersion));
		//			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), strlen((char*)gVersion)+2);
	}
}

//=========================================
//  User define for parameter
//=========================================
unsigned short CRC16_Modbus(unsigned char *pdata, int len)
{
	unsigned short crc = 0xFFFF;
	int i, j;
	for (j = 0; j < len; j++)
	{
		crc = crc ^ pdata[j];
		for (i = 0; i < 8; i++)
		{
			if ((crc & 0x0001) > 0)
			{
				crc = crc >> 1;
				crc = crc ^ 0xA001;
			}
			else
				crc = crc >> 1;
		}
	}
	return crc;
}
void UserParameters(void)
{
	T_COMMON_PARAMETRES *tmpPara = (T_COMMON_PARAMETRES *)gParameters.mData;
	tmpPara->mFlr = 2;
	gParameters.mDevType = E_DEV_LOPPAD;
	//gParameters.mDevType = E_DEV_FLRPAD;
	SendPageIndexToMPU(E_PAGE_LOPPAD);
}

//=========================================
//  User define for timer
//=========================================
unsigned int Countnum = 5;
unsigned int CountnumFlag = 0;
void TimerInit(void)
{
	StartTimer(0, 100);		//100us
	StartTimer(1, 1000000); //1s
}

void Callback_Timer(uint8 pNum) //32λ��ʱ����ʱ�жϵĻص�����,pNum==0:Timer32A, 1:B
{
	if (pNum == 0) //Timer32A
	{
	}
	else if (pNum == 1) //Timer32B
	{
		if (Countnum)
			Countnum--;
		else
			Countnum = 10;
		CountnumFlag = 1;
	}
}

//=========================================
//  User define for controller port
//=========================================

void ConrtollerPortInit(void)
{
	//	{ //UART
	//		UARTOpen(E_RS485, 19200, DATALEN_8, STOPBIT_1, VERIFY_ODD);
	//		SetFrameIntervalOfController(E_CONTROLLER_PORT_UART, 4);  //ms
	//		{
	////			unsigned char gHeader[2] = {0x55, 0xaa};
	////			SetFrameHeaderOfController(E_CONTROLLER_PORT_UART, gHeader, 2);
	//		}
	//		{
	////			unsigned char gFooter[2] = {0x55, 0xaa};
	////			SetFrameFooterOfController(E_CONTROLLER_PORT_UART, gFooter, 2);
	//		}
	//	}

	{ //CAN
		T_CAN_ID gCANIDList[] = {
			{0x740, 0}, //
			{0x600, 0}, //
						//{0x602, 0},	//
		};
		CANOpen(100000, gCANIDList, sizeof(gCANIDList) / sizeof(T_CAN_ID));
		//CANOpen(100000, gCANIDList,0);
	}
}

#if D_FILTER_RAW_DATA_FROM_CONTROLLER
void RecvRawDataFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char pData)
{
	//�Ӵ��ڽ��յ�һ���ֽ�
}
#endif

#if D_FILTER_RAW_FRAME_FROM_CONTROLLER
void RecvRawFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	//��CAN���յ�һ��֡
}
#endif

//=========================================
//  ¥���ַ�ӳ��
//=========================================
unsigned int IsAscii(unsigned char pData)
{
	if (((pData >= '0') && (pData <= '9')) || ((pData >= 'A') && (pData <= 'Z')) || pData == '-')
		return 1;
	return 0;
}

unsigned char GetAsciiPic(unsigned char pData)
{
	if ((pData >= '1') && (pData <= '9'))
		return (pData - '0' + 1); //¥������ſհ�ͼƬ��Ҫ+1
	else if ((pData == (unsigned char)('0')))
		return (pData - '0' + 1);
	else if ((pData >= 'A') && (pData <= 'Z'))
		return pData - 'A' + 10;
	else if (pData == '-')
		return 36;
	else if (pData == '*')
		return 37;
	else if (pData == '+')
		return 38;
	else if (pData == '_')
		return 39;
	return 0xff;
}

//=========================================
//  ���ƹ�Э�����
//=========================================

/****************************�¶��� NICE3000************************/
typedef struct
{
	uint8_t mAddr : 8; //Byte0

	uint8_t mArrowDir : 1;
	uint8_t mArrowDis : 2;
	uint8_t mOverLoad : 1;
	uint8_t mDoorOpen_1 : 1;
	uint8_t mDoorClose_1 : 1;
	uint8_t mDoorOpen_2 : 1;
	uint8_t mDoorClose_2 : 1;

	uint8_t mFloorHBit0_5 : 6;
	uint8_t mFloorLBit5 : 1;
	uint8_t mArriveSingle : 1;

	uint8_t mFloorLBit0_4 : 5;
	uint8_t mCurrentState : 3;

	uint8_t mXorValue;
} T_FRAME_PARSER2;

enum
{
	Auto = 0,
	Fire,
	Drive,
	Vip,
	Inspection,
	Fault
};
/***************************У�麯��***************************************/

unsigned char yihuo_value(unsigned char *data_value, unsigned char length)
{
	unsigned char first_value = 0x0fa;
	int i;
	while (length--)
	{
		first_value ^= *data_value++;
	}
	return (first_value);
}

/**********************End Function*******************************/

typedef struct
{
	T_UI_NODE_ID mUiNodeID;
	unsigned int mCount;
} T_CALLTASK;

T_CALLTASK gCallTask = {0};

void SendCallToController(unsigned char pID)
{
	unsigned int tmpSendVerify = 0;
	unsigned char tmpValue[11] = {0};
	tmpValue[0] = pID;
	if (gCallTask.mCount > 1)
	{
		if ((gCallTask.mUiNodeID >= E_ID_FLRPAD_1) && (gCallTask.mUiNodeID <= E_ID_FLRPAD_48))
		{
			tmpValue[2] = 0x03;
			tmpValue[3] = 0x01;
			tmpValue[4] = gCallTask.mUiNodeID - E_ID_FLRPAD_1 + 1;
		}
		else if (gCallTask.mUiNodeID == E_ID_LOPPAD_UP)
		{
			tmpValue[1] |= 1 << 0;
			tmpValue[2] = 0;
		}
		else if (gCallTask.mUiNodeID == E_ID_LOPPAD_DN)
		{
			tmpValue[1] = 0x02;
			tmpValue[2] = 0;
		}
		else if (gCallTask.mUiNodeID == E_ID_COPPAD_OPEN)
		{
			tmpValue[2] = 0x03;
			tmpValue[8] = 1;
		}
		else if (gCallTask.mUiNodeID == E_ID_COPPAD_CLOSE)
		{
			tmpValue[2] = 0x03;
			tmpValue[8] = 2;
		}
		gCallTask.mCount--;
	}
	else if (gCallTask.mCount == 1)
	{
		if ((gCallTask.mUiNodeID >= E_ID_FLRPAD_1) && (gCallTask.mUiNodeID <= E_ID_FLRPAD_48))
		{
			tmpValue[2] = 0x03;
			tmpValue[3] = 0x00;
			tmpValue[4] = gCallTask.mUiNodeID - E_ID_FLRPAD_1 + 1;
		}
		gCallTask.mCount--;
	}

	tmpSendVerify = yihuo_value(tmpValue, 3);
	tmpValue[3] = tmpSendVerify;
	tmpValue[4] = tmpSendVerify >> 8;
	UARTSend(tmpValue, 5);
}

unsigned char *Flr_pro(unsigned char *pFrame)
{
	T_FRAME_PARSER2 *tmpFrameParser = (T_FRAME_PARSER2 *)pFrame;
	static unsigned char tmpValue[8] = {0};
	unsigned char Val_L = 0, Val_H = 0;
	Val_L = (tmpFrameParser->mFloorLBit0_4 & 0x1f) | ((tmpFrameParser->mFloorLBit5 & 0x01) << 5);
	Val_H = (tmpFrameParser->mFloorHBit0_5 & 0x3f);
	if (Val_L > 43)
		Val_L = 19;
	if (Val_H > 43)
		Val_H = 19;
	tmpValue[2] = StageSigDefine[Val_L][1];
	tmpValue[1] = StageSigDefine[Val_H][1];
	tmpValue[0] = StageSigDefine[Val_H][0];
	return tmpValue;
}

#define SLAVE_ADDRESS 0x01
void SetFuncPosition(unsigned char *pValue)
{
	int i = 0;
	int tmpFuncNum = 0;
	for (i = 0; i < 4; i++)
	{
		if (pValue[i] != 0)
		{
			tmpFuncNum++;
		}
	}

	switch (tmpFuncNum)
	{
	case 0:
	{
		unsigned char tmpData[4] = {0x30, 0x00, 0x00, 0x00};
		tmpData[1] = 0x00;
		tmpData[3] = 0x00;
		SendFrameToMPU(0x82, tmpData, 4);

		tmpData[1] = 0x01;
		tmpData[3] = 0x00;
		SendFrameToMPU(0x82, tmpData, 4);

		tmpData[1] = 0x02;
		tmpData[3] = 0x00;
		SendFrameToMPU(0x82, tmpData, 4);
	}
	break;
	case 1:
	{
		unsigned char tmpData[4] = {0x30, 0x00, 0x00, 0x00};
		tmpData[1] = 0x00;
		tmpData[3] = 0x00;
		SendFrameToMPU(0x82, tmpData, 4);

		tmpData[1] = 0x01;
		if (pValue[0] != 0)
		{
			tmpData[3] = pValue[0];
		}
		if (pValue[1] != 0)
		{
			tmpData[3] = pValue[1];
		}
		if (pValue[2] != 0)
		{
			tmpData[3] = pValue[2];
		}
		if (pValue[3] != 0)
		{
			tmpData[3] = pValue[3];
		}
		SendFrameToMPU(0x82, tmpData, 4);

		tmpData[1] = 0x02;
		tmpData[3] = 0x00;
		SendFrameToMPU(0x82, tmpData, 4);
	}
	break;
	case 2:
	{
		unsigned char tmpData[4] = {0x30, 0x00, 0x00, 0x00};
		tmpData[1] = 0x01;
		tmpData[3] = 0x00;
		SendFrameToMPU(0x82, tmpData, 4);

		if (pValue[0] != 0)
		{
			tmpData[1] = 0x00;
			tmpData[3] = pValue[0];
			SendFrameToMPU(0x82, tmpData, 4);

			if (pValue[1] != 0)
			{
				tmpData[1] = 0x02;
				tmpData[3] = pValue[1];
				SendFrameToMPU(0x82, tmpData, 4);
			}
			else
			{
				if (pValue[2] != 0)
				{
					tmpData[1] = 0x02;
					tmpData[3] = pValue[2];
					SendFrameToMPU(0x82, tmpData, 4);
				}
				else
				{
					tmpData[1] = 0x02;
					tmpData[3] = pValue[3];
					SendFrameToMPU(0x82, tmpData, 4);
				}
			}
		}
		else
		{
			if (pValue[1] != 0)
			{
				tmpData[1] = 0x00;
				tmpData[3] = pValue[1];
				SendFrameToMPU(0x82, tmpData, 4);
				if (pValue[2] != 0)
				{
					tmpData[1] = 0x02;
					tmpData[3] = pValue[2];
					SendFrameToMPU(0x82, tmpData, 4);
				}
				else
				{
					tmpData[1] = 0x02;
					tmpData[3] = pValue[3];
					SendFrameToMPU(0x82, tmpData, 4);
				}
			}
			else
			{
				tmpData[1] = 0x00;
				tmpData[3] = pValue[2];
				SendFrameToMPU(0x82, tmpData, 4);

				tmpData[1] = 0x02;
				tmpData[3] = pValue[3];
				SendFrameToMPU(0x82, tmpData, 4);
			}
		}
	}
	break;
	case 3:
	{
		unsigned char tmpData[4] = {0x30, 0x00, 0x00, 0x00};
		if (pValue[0] != 0)
		{
			tmpData[1] = 0x00;
			tmpData[3] = pValue[0];
			SendFrameToMPU(0x82, tmpData, 4);
			if (pValue[1] != 0)
			{
				tmpData[1] = 0x01;
				tmpData[3] = pValue[1];
				SendFrameToMPU(0x82, tmpData, 4);
				if (pValue[2] != 0)
				{
					tmpData[1] = 0x02;
					tmpData[3] = pValue[2];
					SendFrameToMPU(0x82, tmpData, 4);
				}
				else
				{
					tmpData[1] = 0x02;
					tmpData[3] = pValue[3];
					SendFrameToMPU(0x82, tmpData, 4);
				}
			}
			else
			{
				tmpData[1] = 0x01;
				tmpData[3] = pValue[2];
				SendFrameToMPU(0x82, tmpData, 4);

				tmpData[1] = 0x02;
				tmpData[3] = pValue[3];
				SendFrameToMPU(0x82, tmpData, 4);
			}
		}
		else
		{
			tmpData[1] = 0x00;
			tmpData[3] = pValue[1];
			SendFrameToMPU(0x82, tmpData, 4);

			tmpData[1] = 0x01;
			tmpData[3] = pValue[2];
			SendFrameToMPU(0x82, tmpData, 4);

			tmpData[1] = 0x02;
			tmpData[3] = pValue[3];
			SendFrameToMPU(0x82, tmpData, 4);
		}
	}
	break;
	default:
		break;
	}
}
void DispalyUpdate(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *Data)
{
	unsigned int tmp = 0;
	unsigned char tmpValue[8] = {0x80};

	switch (Data[4])
	{
	case 0x01: //��ͷ
	{
		tmp = Data[5];
		tmp <<= 8;
		tmp |= Data[6];
		memset(tmpValue, 0, sizeof(tmpValue));
		switch (tmp)
		{
		case 0x0000:
			tmpValue[0] = 0x03;
			break;
		case 0x0001:
			tmpValue[0] = 0x01;
			break;
		case 0x0100:
			tmpValue[0] = 0x00;
			break;
		case 0x0101:
			tmpValue[0] = 0x02;
			break;
		}

		if (tmpValue[0] != 0)
		{
			//��ʼ������ָ�� //����ģʽ
			T_WRITE_DATAREG BackgroundData_normal = {0x00, 0x82, 0x00, 0x50, 0x00, 0x00}; //5A A5 07 82 00 82 ���� ���� 00 00
			SendFrameToMPU(0x82, (unsigned char *)(&BackgroundData_normal), 6);
		}
		tmpValue[0] |= 0x80;
		WriteMPUData(pPortType, E_MPU_DATA_ARROW, tmpValue);
		memset(tmpValue, 0, 8);
	}
	break;
	case 0x02: //¥��
	{
		tmpValue[0] = Data[7];
		tmpValue[1] = Data[8];
		tmpValue[2] = Data[9];

		WriteMPUData(pPortType, E_MPU_DATA_FLOOR, tmpValue);
		memset(tmpValue, 0, sizeof(tmpValue));
		//SendFrameToMPU
	}
	case 0x05:
	{
		static unsigned char tmpData[5] = {0x00, 0x00, 0x00, 0X00};
		if (Data[6] == 0x01) //����
		{
			switch (Data[5])
			{
			case 0x01:
			{
				tmpData[0] = 0x01;
			}
			break;
			case 0x02:
			{
				tmpData[1] = 0x02;
			}
			break;
			case 0x03:
			{
				tmpData[2] = 0x03;
			}
			break;
			case 0x05:
			{
				tmpData[3] = 0x04;
			}
			break;
			default:
				break;
			}
		}
		else if (Data[6] == 0x00) //Ϩ��
		{
			switch (Data[5])
			{
			case 0x01:
			{
				tmpData[0] = 0x00;
			}
			break;
			case 0x02:
			{
				tmpData[1] = 0x00;
			}
			break;
			case 0x03:
			{
				tmpData[2] = 0x00;
			}
			break;
			case 0x05:
			{
				tmpData[3] = 0x00;
			}
			break;
			default:
				break;
			}
		}
		SetFuncPosition(tmpData);
		//SendFrameToMPU
	}
	break;
	case 0x03: //����У׼
	{
		//			{ //SendFrameToMPU��ʽ
		//				RTCSET.mDATA[2]=Data[6]-0xD0; //2000������ ʱЧ0xFF-0xD0=47��
		//				RTCSET.mDATA[3]=Data[7];
		//				RTCSET.mDATA[4]=Data[8];
		//
		//				if((RTC_NOW[0]!=RTCSET.mDATA[2])||(RTC_NOW[1]!=RTCSET.mDATA[3])||(RTC_NOW[2]!=RTCSET.mDATA[4])){
		//					RTCSET.mDATA[5]=RTC_NOW[3];
		//					RTCSET.mDATA[6]=RTC_NOW[4];
		//					RTCSET.mDATA[7]=0; //��
		//					SendFrameToMPU(0x82, (unsigned char *)(&RTCSET), 10);
		//				}
		//			}

		{ //WriteMPUData��ʽ

			tmpValue[0] = Data[6] - 0xD0; //2000������ ʱЧ0xFF-0xD0=47��
			tmpValue[1] = Data[7];
			tmpValue[2] = Data[8];

			if ((RTC_NOW[0] != tmpValue[2]) || (RTC_NOW[1] != tmpValue[3]) || (RTC_NOW[2] != tmpValue[4]))
			{
				tmpValue[3] = RTC_NOW[3];
				tmpValue[4] = RTC_NOW[4];
				tmpValue[5] = 0; //��
				WriteMPUData(pPortType, E_MPU_DATA_DATETIME, tmpValue);
				memset(tmpValue, 0, sizeof(tmpValue));
			}
		}
	}
	break;
	case 0x04: //ʱ��У׼
	{
		//			{//SendFrameToMPU��ʽ
		//				RTCSET.mDATA[5]=Data[5];
		//				RTCSET.mDATA[6]=Data[6];
		//				RTCSET.mDATA[7]=0; //��
		//				if((RTC_NOW[3]!=RTCSET.mDATA[5])||(RTC_NOW[4]!=RTCSET.mDATA[6])){
		//					RTCSET.mDATA[2]=RTC_NOW[0];
		//					RTCSET.mDATA[3]=RTC_NOW[1];
		//					RTCSET.mDATA[4]=RTC_NOW[2];
		//					SendFrameToMPU(0x82, (unsigned char *)(&RTCSET), 10);
		//				}
		//			}

		{ //WriteMPUData��ʽ
			tmpValue[3] = Data[5];
			tmpValue[4] = Data[6];
			tmpValue[5] = 0; //��
			if ((RTC_NOW[3] != RTCSET.mDATA[5]) || (RTC_NOW[4] != RTCSET.mDATA[6]))
			{
				tmpValue[0] = RTC_NOW[0];
				tmpValue[1] = RTC_NOW[1];
				tmpValue[2] = RTC_NOW[2];
				WriteMPUData(pPortType, E_MPU_DATA_DATETIME, tmpValue);
				memset(tmpValue, 0, sizeof(tmpValue));
			}
		}
	}
	break;
	case 0x06: //����
	{
		tmpValue[0] = Data[5];
		WriteMPUData(pPortType, E_MPU_DATA_BACKGROUND, tmpValue);
		memset(tmpValue, 0, sizeof(tmpValue));
	}
	break;
	}
}

void DispalyUpdate_CAN(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *Data)
{
	unsigned char tmp[8];								 //Can���յ�������
	unsigned char tmpData[4] = {0x10, 0x00, 0x00, 0x00}; //10 ��ַ 00 ͼƬ

	Can_Message_Object *tmpCanMessage = (Can_Message_Object *)Data;
	// ȡCan���յ�������unsigned short [4]
	// 00 11 AA BB CC DD EE FF
	//����short[4]: FFEE DDCC BBAA 1100
	tmp[0] = tmpCanMessage->data[0] & 0xff;		   //�Ͱ�λ 00
	tmp[1] = (tmpCanMessage->data[0] >> 8) & 0xff; //�߰�λ 11
	tmp[2] = tmpCanMessage->data[1] & 0xff;		   //�Ͱ�λ AA
	tmp[3] = (tmpCanMessage->data[1] >> 8) & 0xff; //�߰�λ BB
	tmp[4] = tmpCanMessage->data[2] & 0xff;		   //�Ͱ�λ CC
	tmp[5] = (tmpCanMessage->data[2] >> 8) & 0xff; //�߰�λ DD
	tmp[6] = tmpCanMessage->data[3] & 0xff;		   //�Ͱ�λ EE
	tmp[7] = (tmpCanMessage->data[3] >> 8) & 0xff; //�߰�λ FF
	if (tmpCanMessage->id == 0x740)
	{
		switch (tmp[2])
		{
		case 0x46: //������� //1D 00 46 FF ���� 00 00
		{
			//��ʼ������ָ�� //����ģʽ
			T_WRITE_DATAREG BackgroundData = {0x00, 0x82, 0x00, 0x50, 0x00, 0x00}; //5A A5 07 82 00 82 ���� ���� 00 00
			BackgroundData.mDATA[1] = tmp[4];
			SendFrameToMPU(0x82, (unsigned char *)(&BackgroundData), 6);
			CANSend((Can_Message_Object *)Data); //�ش�ԭ����
		}
		break;
		case 0x48: //��ʾ����
		{
			if (tmp[4] == 0x08) //ͣ��
			{
				tmpData[1] = 0x10;
				tmpData[3] = 02;
				SendFrameToMPU(0x82, tmpData, 4);
			}
			if (tmp[4] == 0x10) //����
			{
				tmpData[1] = 0x10;
				tmpData[3] = 05;
				SendFrameToMPU(0x82, tmpData, 4);
			}
			if (tmp[4] == 0x40) //����
			{
				tmpData[1] = 0x10;
				tmpData[3] = 01;
				SendFrameToMPU(0x82, tmpData, 4);
			}
			if (tmp[4] == 0x00) //��������
			{
				tmpData[1] = 0x10;
				tmpData[3] = 00;
				SendFrameToMPU(0x82, tmpData, 4);
			}
		}
		break;
		}
	}
	if (tmpCanMessage->id == 0x600)
	{
		switch (tmp[2])
		{
		case 0x40: //¥��+��ͷ��ʾ
		{
			if (tmp[6] == 0x52) //�ϼ�ͷ
			{
				tmpData[1] = 0x09;
				tmpData[3] = 06;
				SendFrameToMPU(0x82, tmpData, 4);
			}
			if (tmp[6] == 0x62) //�¼�ͷ
			{
				tmpData[1] = 0x09;
				tmpData[3] = 01;
				SendFrameToMPU(0x82, tmpData, 4);
			}
			if (tmp[6] == 0x00) //�޼�ͷ
			{
				tmpData[1] = 0x09;
				tmpData[3] = 00;
				SendFrameToMPU(0x82, tmpData, 4);
			}

			//¥����ʾ //��λ
			tmpData[1] = 0x03;
			if (tmp[3] == 0x60)
			{ //0
				tmpData[3] = 0x28;
			}
			else //��0
			{
				tmpData[3] = (tmp[3] - 0x60) / 2;
			}
			SendFrameToMPU(0x82, tmpData, 4);
			//¥����ʾ //ʮλ
			tmpData[1] = 0x05;
			if (tmp[4] == 0x60)
			{ //0
				tmpData[3] = 0x28;
			}
			else if (tmp[4] == 0x5A)
			{
				tmpData[3] = 0x24;
			}
			else //��0
			{
				tmpData[3] = (tmp[4] - 0x60) / 2;
			}
			SendFrameToMPU(0x82, tmpData, 4);
		}
		break;
		}
	}
}

void ParseFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	if (pPortType == E_CONTROLLER_PORT_CAN) //CANЭ��
	{
		DispalyUpdate_CAN(pPortType, pFrame);
	}

	if (pPortType == E_CONTROLLER_PORT_UART) //RS485Э��
	{
		unsigned char SendBuf[20];
		unsigned short Check1, Check2;
		//		if(pPortType != E_CONTROLLER_PORT_UART)
		//			return;
		Check1 = CRC16_Modbus(pFrame, pLen - 2);
		Check2 = pFrame[pLen - 2];
		Check2 |= (pFrame[pLen - 1] << 8);
		if (Check1 != Check2)
			return;
		if (pFrame[0] != SLAVE_ADDRESS)
		{
			return;
		}
		if (pFrame[1] == 0x08)
		{
			SendBuf[0] = SLAVE_ADDRESS;
			SendBuf[1] = 0x08;
			SendBuf[2] = 0;
			SendBuf[3] = 2;
			SendBuf[4] = 0;
			SendBuf[5] = 1;
			Check1 = CRC16_Modbus(SendBuf, 6);
			SendBuf[6] = Check1 & 0xff;
			SendBuf[7] = (Check1 >> 8) & 0xff;
			UARTSend(SendBuf, 8);
		}
		if (pFrame[1] == 0x10)
		{
			SendBuf[0] = SLAVE_ADDRESS;
			SendBuf[1] = 0x10;
			SendBuf[2] = 0;
			SendBuf[3] = 1;
			SendBuf[4] = 1;
			Check1 = CRC16_Modbus(SendBuf, 5);
			SendBuf[5] = Check1 & 0xff;
			SendBuf[6] = (Check1 >> 8) & 0xff;
			UARTSend(SendBuf, 7);
			DispalyUpdate(pPortType, pFrame);
		}
	}
}
//=========================================
//  ���ƹ����Э�鷢��
//=========================================

void CallElevatorToController(T_UI_NODE_ID pButtonID) //������������ƹ�
{
	gCallTask.mUiNodeID = pButtonID;
	gCallTask.mCount = 2;
	{
		T_COMMON_PARAMETRES *tmpCommPara = (T_COMMON_PARAMETRES *)gParameters.mData;
		unsigned char tmpID = 0; //1~48ǰ�����ID��49~96�������ID��97��ǰ�Ų����䣬98���Ų����䣬99�м��˲����䣬100��������
		if (gParameters.mDevType == E_DEV_LOPPAD)
		{
			tmpID = tmpCommPara->add_flr; // + tmpCommPara->mDoorAddr*48;
		}
		else if (gParameters.mDevType == E_DEV_FLRPAD)
		{
			tmpID = 0x61 + tmpCommPara->mDoorAddr;
		}
		//SendCallToController(tmpID); //��֪Ϊʲô�������Ӵ���
	}
}
