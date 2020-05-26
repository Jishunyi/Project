/*****************************************************************************
	Copyright (C), 2010-2018, BST Electric Co., Ltd.
	File name: user.c
	Author: Jinwenjie 
	Version: V0 
	Date: 2018-11-08
	Description: 用户非标文件
	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/

//* Private include files -----------------------------------------------------
//* @{引用头文件声明区域(仅限本文件内使用)

#include "user.h"

unsigned char gVersion[32] = "V1.0.0.20190625";

void Callback_SysTick(void)		//1ms系统时钟超时中断的回调函数
{
	
}

//=========================================
//  User define for parameter
//=========================================

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

void TimerInit(void)
{
	StartTimer(0, 100);  //100us
	StartTimer(1, 1000000); //1s
}

void Callback_Timer(uint8 pNum)		//32位定时器超时中断的回调函数,pNum==0:Timer32A, 1:B
{
	if(pNum == 0)  //Timer32A
	{
		
	}
	else if(pNum == 1)  //Timer32B
	{
		
	}
}

//=========================================
//  User define for controller port
//=========================================

void ConrtollerPortInit(void)
{
	{ //UART
		UARTOpen(E_RS485, 38400, DATALEN_8, STOPBIT_1, VERIFY_NONE);
		SetFrameIntervalOfController(E_CONTROLLER_PORT_UART, 10);  //ms
		{
//			unsigned char gHeader[2] = {0x55, 0xaa};
//			SetFrameHeaderOfController(E_CONTROLLER_PORT_UART, gHeader, 2);
		}
		{
//			unsigned char gFooter[2] = {0x55, 0xaa};
//			SetFrameFooterOfController(E_CONTROLLER_PORT_UART, gFooter, 2);
		}
	}
	
	{ //CAN
//		T_CAN_ID  gCANIDList[] = {
//			{0x630, 0},	//基本信息
//			{0x601, 0},	//前门上呼记忆
//			{0x602, 0},	//前门下呼记忆
//		};	
//		CANOpen(20000, gCANIDList, sizeof(gCANIDList)/sizeof(T_CAN_ID));
	}
}

#if D_FILTER_RAW_DATA_FROM_CONTROLLER
void RecvRawDataFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char pData)
{
	//从串口接收到一个字节
}
#endif

#if D_FILTER_RAW_FRAME_FROM_CONTROLLER
void RecvRawFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	//从CAN接收到一个帧
}
#endif

//=========================================
//  楼层字符映射
//=========================================
unsigned int IsAscii(unsigned char pData)
{
	if(((pData >= '0') && (pData <= '9')) || ((pData >= 'A') && (pData <= 'Z')))
		return 1;
	return 0;
}

unsigned char GetAsciiPic(unsigned char pData)
{
	if((pData >= '1') && (pData <= '9'))
		return pData-'0';
	else if((pData == (unsigned char)('0')))
		return pData-'0';
	else if((pData >= 'A') && (pData <= 'Z'))
		return pData-'A'+10;
	return 0xff;
}

//=========================================
//  控制柜协议解析
//=========================================
/** [T_FRAME_PARSER]
	For RS485/RS232
	For CAN: 参考Can_Message_Object定义
*/

typedef struct{
	unsigned char  mDirection : 3;
	unsigned char  mIndependence : 1;
	unsigned char  mDriver : 1;
	unsigned char  mFull : 1;
	unsigned char  mOverload : 1;
	unsigned char  mLock : 1;
	unsigned char  mFire : 1;
	unsigned char  mInspect : 1;
	unsigned char  mFault : 1;
	unsigned char  mEarthquake : 1;
	unsigned char  mUpLight : 1;
	unsigned char  mDnLight : 1;
	unsigned char  mUpSound : 1;
	unsigned char  mDnSound : 1;
}T_FUNC_BITS;

typedef struct {
	unsigned char  mFlr1 : 1;
	unsigned char  mFlr2 : 1;
	unsigned char  mFlr3 : 1;
	unsigned char  mFlr4 : 1;
	unsigned char  mFlr5 : 1;
	unsigned char  mFlr6 : 1;
	unsigned char  mFlr7 : 1;
	unsigned char  mFlr8 : 1;
	unsigned char  mFlr9 : 1;
	unsigned char  mFlr10 : 1;
	unsigned char  mFlr11 : 1;
	unsigned char  mFlr12 : 1;
	unsigned char  mFlr13 : 1;
	unsigned char  mFlr14 : 1;
	unsigned char  mFlr15 : 1;
	unsigned char  mFlr16 : 1;
	unsigned char  mFlr17 : 1;
	unsigned char  mFlr18 : 1;
	unsigned char  mFlr19 : 1;
	unsigned char  mFlr20 : 1;
	unsigned char  mFlr21 : 1;
	unsigned char  mFlr22 : 1;
	unsigned char  mFlr23 : 1;
	unsigned char  mFlr24 : 1;
	unsigned char  mFlr25 : 1;
	unsigned char  mFlr26 : 1;
	unsigned char  mFlr27 : 1;
	unsigned char  mFlr28 : 1;
	unsigned char  mFlr29 : 1;
	unsigned char  mFlr30 : 1;
	unsigned char  mFlr31 : 1;
	unsigned char  mFlr32 : 1;
	unsigned char  mFlr33 : 1;
	unsigned char  mFlr34 : 1;
	unsigned char  mFlr35 : 1;
	unsigned char  mFlr36 : 1;
	unsigned char  mFlr37 : 1;
	unsigned char  mFlr38 : 1;
	unsigned char  mFlr39 : 1;
	unsigned char  mFlr40 : 1;
	unsigned char  mFlr41 : 1;
	unsigned char  mFlr42 : 1;
	unsigned char  mFlr43 : 1;
	unsigned char  mFlr44 : 1;
	unsigned char  mFlr45 : 1;
	unsigned char  mFlr46 : 1;
	unsigned char  mFlr47 : 1;
	unsigned char  mFlr48 : 1;
}T_BUTTON_BITS;

typedef struct {
	unsigned char    	mHeader;
	unsigned char			mID;
	unsigned char 		mCode;
	union _DataParser{
		unsigned char		mData[6];
		struct _Code01{
			unsigned char  mFlrH;
			unsigned char  mFlrM;
			unsigned char  mFlrL;
			unsigned char  mFunc[2];
			unsigned char  mLastErr;
		}mCode01;
		struct _Code04{	//轿内指令灯
			unsigned char  mRegFlr[6];
		}mCode04;
		struct _Code05{
			unsigned char  mBtnOpenLight : 1;
			unsigned char  mBtnCloseLight : 1;
		}mCode05;
		struct _Code07{  //前门上召唤灯
			unsigned char  mRegFlr[6];
		}mCode07;
		struct _Code08{  //前门下召唤灯
			unsigned char  mRegFlr[6];
		}mCode08;
		struct _Code09{  //后门上召唤灯
			unsigned char  mRegFlr[6];
		}mCode09;
		struct _Code0A{  //后门下召唤灯
			unsigned char  mRegFlr[6];
		}mCode0A;
	} mDataParser;
	unsigned char 		mVerify[2];
} T_FRAME_PARSER;

unsigned int CRC_16(unsigned char *Pdata,unsigned char clength)
{
	unsigned int crc16=0xffff;
	unsigned char a,f;
	for (a=0;a<clength;a++)
	{
		crc16 = crc16 ^ (unsigned int)Pdata[a];
		for (f=0;f<8;f++)
		{
			if(crc16 & 1)
			{
				crc16 = crc16 >> 1;
				crc16 ^= 0xA001;
			}
			else 
				crc16 = crc16 >> 1;
		}
	}
	return crc16;
}


typedef struct{
	T_UI_NODE_ID  	mUiNodeID;
	unsigned int		mCount;
}T_CALLTASK;

T_CALLTASK  gCallTask = {0};

void SendCallToController(unsigned char pID)
{
	unsigned int tmpSendVerify = 0;
	unsigned char tmpValue[11] = {0};
	tmpValue[0] = 0xF2;
	tmpValue[1] = pID;
	if(gCallTask.mCount > 1)
	{
		if((gCallTask.mUiNodeID >= E_ID_FLRPAD_1) && (gCallTask.mUiNodeID <= E_ID_FLRPAD_48))
		{
			tmpValue[2] = 0x03;
			tmpValue[3] = 0x01;
			tmpValue[4] = gCallTask.mUiNodeID - E_ID_FLRPAD_1 + 1;
		}
		else if(gCallTask.mUiNodeID == E_ID_LOPPAD_UP)
		{
			tmpValue[2] = 0x06;
			tmpValue[3] = 1;
		}
		else if(gCallTask.mUiNodeID == E_ID_LOPPAD_DN)
		{
			tmpValue[2] = 0x06;
			tmpValue[3] = 2;
		}		
		else if(gCallTask.mUiNodeID == E_ID_COPPAD_OPEN)
		{
			tmpValue[2] = 0x03;
			tmpValue[8] = 1;
		}		
		else if(gCallTask.mUiNodeID == E_ID_COPPAD_CLOSE)
		{
			tmpValue[2] = 0x03;
			tmpValue[8] = 2;
		}						
		gCallTask.mCount--;
	}
	else if(gCallTask.mCount == 1)
	{
		if((gCallTask.mUiNodeID >= E_ID_FLRPAD_1) && (gCallTask.mUiNodeID <= E_ID_FLRPAD_48))
		{
			tmpValue[2] = 0x03;
			tmpValue[3] = 0x00;
			tmpValue[4] = gCallTask.mUiNodeID - E_ID_FLRPAD_1 + 1;
		}
		gCallTask.mCount--;
	}
	tmpSendVerify = CRC_16(tmpValue, 9);
	tmpValue[9] = tmpSendVerify;
	tmpValue[10] = tmpSendVerify>>8;
	UARTSend(tmpValue, 11);
}

void ParseFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	if(pPortType == E_CONTROLLER_PORT_UART)
	{
		if(pLen == 11)
		{
			T_FRAME_PARSER *tmpFrameParser = (T_FRAME_PARSER *)pFrame;
			if(tmpFrameParser->mHeader == 0xF1)
			{
				//计算校验
				unsigned int tmpVerify = CRC_16(pFrame, 9);
				unsigned int tmpFrameVerify = (((unsigned int)(tmpFrameParser->mVerify[1]))<<8) + tmpFrameParser->mVerify[0];
				if(tmpFrameVerify == tmpVerify)
				{					
					T_COMMON_PARAMETRES *tmpCommPara = (T_COMMON_PARAMETRES *)gParameters.mData;
					if(tmpFrameParser->mCode == 0x01)
					{
						unsigned char tmpID = 0; //1~48前门外呼ID，49~96后门外呼ID，97：前门操纵箱，98后门操纵箱，99残疾人操纵箱，100福操纵箱
						if(gParameters.mDevType == E_DEV_LOPPAD)
						{
							tmpID = tmpCommPara->mFlr + tmpCommPara->mDoorAddr*48;
						}
						else if(gParameters.mDevType == E_DEV_FLRPAD)
						{
							tmpID = 0x61 + tmpCommPara->mDoorAddr;
						}
						if(tmpFrameParser->mID == tmpID)
						{
							unsigned char tmpValue[8] = {0};
							T_FUNC_BITS *tmpFuncBits = 0;
							T_DISP_FUNCTION *tmpFunc = 0;
								
							if(gCallTask.mCount > 0)
							{	
								SendCallToController(tmpID);
							}
							
							//Floor
							tmpValue[0] = tmpFrameParser->mDataParser.mCode01.mFlrH;
							tmpValue[1] = tmpFrameParser->mDataParser.mCode01.mFlrM;
							tmpValue[2] = tmpFrameParser->mDataParser.mCode01.mFlrL;
							WriteMPUData(pPortType, E_MPU_DATA_FLOOR, tmpValue);
							
							//Arrow
							tmpFuncBits = (T_FUNC_BITS *)(tmpFrameParser->mDataParser.mCode01.mFunc);
							if(tmpFuncBits->mDirection == 2)
								tmpValue[0] = 1;
							else if(tmpFuncBits->mDirection == 3)
								tmpValue[0] = 5;
							else if(tmpFuncBits->mDirection == 4)
								tmpValue[0] = 2;
							else if(tmpFuncBits->mDirection == 5)
								tmpValue[0] = 6;
							else
								tmpValue[0] = 0;
							WriteMPUData(pPortType, E_MPU_DATA_ARROW, tmpValue);
							
							//Function
							memset(tmpValue, 0, 8);
							tmpFunc = (T_DISP_FUNCTION *)tmpValue;
							WriteMPUData(pPortType, E_MPU_DATA_FUNCTION, tmpValue);
						}
					}
					else if(tmpFrameParser->mCode == 0x04)
					{
						unsigned char tmpDoorAddr = tmpCommPara->mDoorAddr + 0x61;
						if(tmpFrameParser->mID == tmpDoorAddr)
						{
							unsigned char tmpValue[8] = {0};
							memcpy(tmpValue, tmpFrameParser->mDataParser.mCode04.mRegFlr, 6);
							WriteMPUData(pPortType, E_MPU_DATA_FLRPAD, tmpValue);
						}
					}
					else if(tmpFrameParser->mCode == 0x05)
					{
						unsigned char tmpDoorAddr = tmpCommPara->mDoorAddr + 0x61;
						if(tmpFrameParser->mID == tmpDoorAddr)
						{
							unsigned char tmpValue[8] = {0};
							tmpValue[0] = (tmpFrameParser->mDataParser.mCode05.mBtnCloseLight<<1) + tmpFrameParser->mDataParser.mCode05.mBtnOpenLight;
							WriteMPUData(pPortType, E_MPU_DATA_COPPAD, tmpValue);
						}
					}
					else if(tmpFrameParser->mCode == 0x07)
					{
						if(tmpCommPara->mDoorAddr == E_DOOR_FRONT)
						{
							if(tmpFrameParser->mID == 0)
							{
								unsigned char tmpFlr = tmpCommPara->mFlr-1;
								unsigned int tmpRow = tmpFlr/8;
								unsigned int tmpColumn = tmpFlr%8;
								if(tmpFrameParser->mDataParser.mCode07.mRegFlr[tmpRow] & (1<<tmpColumn))
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] | 1;
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
								else
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] & (~(1));
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
							}
						}
					}
					else if(tmpFrameParser->mCode == 0x08)
					{
						if(tmpCommPara->mDoorAddr == E_DOOR_FRONT)
						{
							if(tmpFrameParser->mID == 0)
							{
								unsigned char tmpFlr = tmpCommPara->mFlr-1;
								unsigned int tmpRow = tmpFlr/8;
								unsigned int tmpColumn = tmpFlr%8;
								if(tmpFrameParser->mDataParser.mCode07.mRegFlr[tmpRow] & (1<<tmpColumn))
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] | (1<<1);
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
								else
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] & (~(1<<1));
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
							}
						}
					}
					else if(tmpFrameParser->mCode == 0x09)
					{
						if(tmpCommPara->mDoorAddr == E_DOOR_REAR)
						{
							if(tmpFrameParser->mID == 0)
							{
								unsigned char tmpFlr = tmpCommPara->mFlr-1;
								unsigned int tmpRow = tmpFlr/8;
								unsigned int tmpColumn = tmpFlr%8;
								if(tmpFrameParser->mDataParser.mCode09.mRegFlr[tmpRow] & (1<<tmpColumn))
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] | 1;
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
								else
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] & (~(1));
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
							}
						}
					}
					else if(tmpFrameParser->mCode == 0x0A)
					{
						if(tmpCommPara->mDoorAddr == E_DOOR_REAR)
						{
							if(tmpFrameParser->mID == 0)
							{
								unsigned char tmpFlr = tmpCommPara->mFlr-1;
								unsigned int tmpRow = tmpFlr/8;
								unsigned int tmpColumn = tmpFlr%8;
								if(tmpFrameParser->mDataParser.mCode0A.mRegFlr[tmpRow] & (1<<tmpColumn))
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] | (1<<1);
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
								else
								{
									unsigned char tmpChar = ReadMPUData(E_MPU_DATA_LOPPAD)->mValue[0] & (~(1<<1));
									WriteMPUData(pPortType, E_MPU_DATA_LOPPAD, &tmpChar);
								}
							}
						}
					}
				}
			}
		}
	}
	else if(pPortType == E_CONTROLLER_PORT_CAN)
	{
	
	}
}

//=========================================
//  控制柜呼梯协议发送
//=========================================

void CallElevatorToController(T_UI_NODE_ID pButtonID)  //呼梯命令给控制柜
{
	gCallTask.mUiNodeID = pButtonID;
	gCallTask.mCount = 10;

	{
		T_COMMON_PARAMETRES *tmpCommPara = (T_COMMON_PARAMETRES *)gParameters.mData;
		unsigned char tmpID = 0; //1~48前门外呼ID，49~96后门外呼ID，97：前门操纵箱，98后门操纵箱，99残疾人操纵箱，100福操纵箱
		if(gParameters.mDevType == E_DEV_LOPPAD)
		{
			tmpID = tmpCommPara->mFlr + tmpCommPara->mDoorAddr*48;
		}
		else if(gParameters.mDevType == E_DEV_FLRPAD)
		{
			tmpID = 0x61 + tmpCommPara->mDoorAddr;
		}
		SendCallToController(tmpID); //不知为什么必须增加此行
	}
}

