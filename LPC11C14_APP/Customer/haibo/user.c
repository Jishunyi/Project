/*****************************************************************************
	Copyright (C), 2010-2018, BST Electric Co., Ltd.
	File name: user.c
	Author: Yujinjun 
	Version: V1.0 
	Date: 2019-07-17
	Description: 用户非标文件
	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/

//* Private include files -----------------------------------------------------
//* @{引用头文件声明区域(仅限本文件内使用)

#include "user.h"
unsigned char gVersion[32] ="Ver:1.0.0.20191010";
const unsigned char  StageSigDefine[][2]={
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
const unsigned char  HextoASCII_Code[]={
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','G','H','L','M','P','R','-',' ',
	' ',' ',' ','C','D','E','F'	,'I','J','K',
	'N','0','Q','S','T','U','V','W','X','Y',
	'Z',' ',' ',' '
};
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
unsigned int Countnum=5;
unsigned int CountnumFlag=0;
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
		if(Countnum)
			Countnum--;
		else
			Countnum=10;
			CountnumFlag=1;
			
	}
}

//=========================================
//  User define for controller port
//=========================================

void ConrtollerPortInit(void)
{
	{ //UART
		UARTOpen(E_RS485, 9600, DATALEN_8, STOPBIT_1, VERIFY_NONE);
		SetFrameIntervalOfController(E_CONTROLLER_PORT_UART, 5);  //ms
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
	if(((pData >= '0') && (pData <= '9')) || ((pData >= 'A') && (pData <= 'Z'))||pData=='-')
		return 1;
	return 0;
}

unsigned char GetAsciiPic(unsigned char pData)
{
	if((pData >= '1') && (pData <= '9'))
		return (pData-'0');
	else if((pData == (unsigned char)('0')))
		return (pData-'0'+40);
	else if((pData >= 'A') && (pData <= 'Z'))
		return pData-'A'+10;
	else if(pData=='-')
		return 36;
	else if(pData=='*')
		return 37;
	else if(pData=='+')
		return 38;
	else if(pData=='_')
		return 39;
	return 0xff;
}

//=========================================
//  控制柜协议解析
//=========================================

/****************************德尔法 NICE3000************************/
typedef struct {
	uint8_t mAddr:           8;//Byte0
	
	uint8_t mArrowDir:       1;
	uint8_t mArrowDis:       2;
	uint8_t mOverLoad: 		   1;
	uint8_t mDoorOpen_1:		 1;
	uint8_t mDoorClose_1:	   1;
	uint8_t mDoorOpen_2:     1;
	uint8_t mDoorClose_2:    1;
	
	uint8_t mFloorHBit0_5:   6;
	uint8_t mFloorLBit5:     1;
	uint8_t mArriveSingle:   1;
	
	uint8_t mFloorLBit0_4:   5;
	uint8_t mCurrentState:   3;
	
	uint8_t mXorValue;
}T_FRAME_PARSER2;

enum{
	Auto=0,
	Fire,
	Drive,
	Vip,
	Inspection,
	Fault
};
/***************************校验函数***************************************/

unsigned char yihuo_value(unsigned char *data_value,unsigned char length)
{
		unsigned char first_value = 0x0fa;
		int i;
		while(length -- )
		{
			first_value  ^= *data_value ++;
		}
		return (first_value);
}

/**********************End Function*******************************/

typedef struct{
	T_UI_NODE_ID  	mUiNodeID;
	unsigned int		mCount;
}T_CALLTASK;

T_CALLTASK  gCallTask = {0};

void SendCallToController(unsigned char pID)
{
	unsigned int tmpSendVerify = 0;
	unsigned char tmpValue[11] = {0};
	tmpValue[0] = pID;
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
			tmpValue[1] |= 1<<0;
			tmpValue[2] = 0;
		}
		else if(gCallTask.mUiNodeID == E_ID_LOPPAD_DN)
		{
			tmpValue[1] = 0x02;
			tmpValue[2] = 0;
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
	
	tmpSendVerify = yihuo_value(tmpValue, 3);
	tmpValue[3] = tmpSendVerify;
	tmpValue[4] = tmpSendVerify>>8;
	UARTSend(tmpValue, 5);
}

unsigned char* Flr_pro(unsigned char *pFrame)
{
	T_FRAME_PARSER2 *tmpFrameParser = (T_FRAME_PARSER2 *)pFrame;
	static unsigned char tmpValue[8] = {0};
	unsigned char Val_L=0,Val_H=0;
	Val_L=(tmpFrameParser->mFloorLBit0_4&0x1f)|((tmpFrameParser->mFloorLBit5&0x01)<<5);
	Val_H=(tmpFrameParser->mFloorHBit0_5&0x3f);
	if(Val_L>43) Val_L=19;
	if(Val_H>43) Val_H=19;
	tmpValue[2]=StageSigDefine[Val_L][1];
	tmpValue[1]=StageSigDefine[Val_H][1];
	tmpValue[0]=StageSigDefine[Val_H][0];
	return tmpValue;
}

void ParseFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	if(pPortType == E_CONTROLLER_PORT_UART)
	{
		if(pLen == 5)
		{
			T_FRAME_PARSER2 *tmpFrameParser = (T_FRAME_PARSER2 *)pFrame;
		
			{
				//计算校验
				unsigned char tmpVerify = yihuo_value(pFrame, 4);
				unsigned char tmpFrameVerify = tmpFrameParser->mXorValue;

				if(tmpFrameVerify == tmpVerify)
				{					
					unsigned char tmpID = 0;
					unsigned char tmpValue[8] = {0};
					if(tmpFrameParser->mAddr == 63)
					{
						{
							memcpy(tmpValue,Flr_pro(pFrame),3);
							WriteMPUData(pPortType, E_MPU_DATA_FLOOR, tmpValue);
							memset (tmpValue,0,sizeof (tmpValue));
						}
						
						{
							if(tmpFrameParser->mArrowDir==1)
							{
								tmpValue[0]|=0x01;
							}
							tmpValue[0]|=(tmpFrameParser->mArrowDis<<1);
							if(tmpValue[0]==0) tmpValue[0]|=0x80;
							WriteMPUData(pPortType, E_MPU_DATA_ARROW, tmpValue);
							memset(tmpValue, 0, 8);
						}
						
						{
							if(tmpFrameParser->mOverLoad)
								tmpValue[0] |= 1<<1;
							if(tmpFrameParser->mCurrentState == Fire)
								tmpValue[0] |= 1<<2;
							else if(tmpFrameParser->mCurrentState == Inspection)
								tmpValue[0] |= 1<<5;
							else if(tmpFrameParser->mCurrentState == Vip)
								tmpValue[0] |= 1<<7;
							else if(tmpFrameParser->mCurrentState == Drive)
								tmpValue[0] |= 1<<3;
							else if(tmpFrameParser->mCurrentState == Fault)
								tmpValue[0] |= 1<<6;
							WriteMPUData(pPortType, E_MPU_DATA_FUNCTION, tmpValue);
							memset(tmpValue,0,sizeof(tmpValue));
						
						}
				  }
			  }
			}
		}
	}
}

//=========================================
//  控制柜呼梯协议发送
//=========================================

void CallElevatorToController(T_UI_NODE_ID pButtonID)  //呼梯命令给控制柜
{
	gCallTask.mUiNodeID = pButtonID;
	gCallTask.mCount = 2;

	{
		T_COMMON_PARAMETRES *tmpCommPara = (T_COMMON_PARAMETRES *)gParameters.mData;
		unsigned char tmpID = 0; //1~48前门外呼ID，49~96后门外呼ID，97：前门操纵箱，98后门操纵箱，99残疾人操纵箱，100福操纵箱
		if(gParameters.mDevType == E_DEV_LOPPAD)
		{
			tmpID = tmpCommPara->add_flr;// + tmpCommPara->mDoorAddr*48;
		}
		else if(gParameters.mDevType == E_DEV_FLRPAD)
		{
			tmpID = 0x61 + tmpCommPara->mDoorAddr;
		}
		//SendCallToController(tmpID); //不知为什么必须增加此行
	}
}

