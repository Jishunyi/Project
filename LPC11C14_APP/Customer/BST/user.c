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

unsigned char gVersion[32] = "V1.0.0.20190621";

void Callback_SysTick(void)		//1ms系统时钟超时中断的回调函数
{
	
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
		//UARTOpen(E_RS485, 115200, DATALEN_8, STOPBIT_1, VERIFY_NONE);
		//SetFrameIntervalOfController(E_CONTROLLER_PORT_UART, 5);  //ms
		//unsigned char gHeader[2] = {0x55, 0xaa};
		//SetFrameHeaderOfController(E_CONTROLLER_PORT_UART, gHeader, 2);
		//unsigned char gFooter[2] = {0x55, 0xaa};
		//SetFrameFooterOfController(E_CONTROLLER_PORT_UART, gFooter, 2);
	}
	
	{ //CAN
		T_CAN_ID  gCANIDList[] = {
			{0x630, 0},	//基本信息
			{0x601, 0},	//前门上呼记忆
			{0x602, 0},	//前门下呼记忆
		};	
		CANOpen(20000, gCANIDList, sizeof(gCANIDList)/sizeof(T_CAN_ID));
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
	unsigned char  mBZZ : 1;
	unsigned char  mDZ_C : 1;
	unsigned char  mFULL : 1;
	unsigned char  mOL : 1;
	unsigned char  mINS : 1;
	unsigned char  mDN : 1;
	unsigned char  mUP : 1;
	unsigned char  mRUN_M : 1;
	unsigned char  mD_M : 2;
	unsigned char  mOFF_D : 1;
	unsigned char  mOFF_D1 : 1;
	unsigned char  mReserve1 : 1;
	unsigned char  mES : 1;
	unsigned char  mSEV : 1;
	unsigned char  mReserve2 : 1;
	unsigned char  mXF : 1;
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
	unsigned int    mExtendFlag;	/* 0--标准帧，1--扩展帧 */
	unsigned int		mID;
	unsigned int 		mLength;
	union _DataParser{
		unsigned char		mData[8];
		struct _Frame630{
			unsigned char  mFlrH;
			unsigned char  mFlrL;
			unsigned char  mFlrAddr;//实际楼层位置，从1开始
			unsigned char  mFunc[3];
		}mFrame630;
		struct  _Frame601{
			unsigned char		mFlrState[6];
		}mFrame601;
		struct _Frame602{
			unsigned char		mFlrState[6];
		}mFrame602;
		struct _Frame610{
			unsigned char		mFlrState[6];
		}mFrame610;
	} mDataParser;
} T_FRAME_PARSER;

void ParseFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	if(pPortType == E_CONTROLLER_PORT_UART)
	{
		
	}
	else if(pPortType == E_CONTROLLER_PORT_CAN)
	{
		T_FRAME_PARSER *tmpParser = 0;
		//unsigned int tmpFrameSize = sizeof(T_FRAME_PARSER);
		//if(tmpFrameSize > pLen)
		//	return;
		if(!pFrame)
			return;
		tmpParser = (T_FRAME_PARSER *)pFrame;
		if(tmpParser->mID == 0x630)
		{
			unsigned char tmpValue[8] = {0};
			T_FUNC_BITS *tmpFuncBits = 0;
			T_DISP_FUNCTION *tmpFunc = 0;
			
			//Floor
			tmpValue[1] = tmpParser->mDataParser.mFrame630.mFlrH;
			tmpValue[2] = tmpParser->mDataParser.mFrame630.mFlrL;
			WriteMPUData(pPortType, E_MPU_DATA_FLOOR, tmpValue);
			
			//Arrow
			tmpFuncBits = (T_FUNC_BITS *)(tmpParser->mDataParser.mFrame630.mFunc);
			tmpValue[0] = (tmpFuncBits->mRUN_M << 2) + (tmpFuncBits->mDN << 1) + tmpFuncBits->mUP;
			WriteMPUData(pPortType, E_MPU_DATA_ARROW, tmpValue);
			
			//Function
			memset(tmpValue, 0, 8);
			tmpFunc = (T_DISP_FUNCTION *)tmpValue;
			tmpFunc->mJianXiu = tmpFuncBits->mINS;
			tmpFunc->mChaoZai = tmpFuncBits->mOL;
			tmpFunc->mManZai = tmpFuncBits->mFULL;
			tmpFunc->mVIP = tmpFuncBits->mSEV;
			tmpFunc->mKaiMenQu = tmpFuncBits->mDZ_C;
			tmpFunc->mBaoZhanZhong = tmpFuncBits->mBZZ;
			tmpFunc->mYunXingDaoZhan = tmpFuncBits->mOFF_D1;
			tmpFunc->mJiTing = tmpFuncBits->mES;
			WriteMPUData(pPortType, E_MPU_DATA_FUNCTION, tmpValue);
		}
		else if(tmpParser->mID == 0x601)
		{
			T_COMMON_PARAMETRES *tmpPara = (T_COMMON_PARAMETRES *)gParameters.mData;
			unsigned char tmpFlr = tmpPara->mFlr-1;
			unsigned int tmpRow = tmpFlr/8;
			unsigned int tmpColumn = tmpFlr%8;
			if(tmpParser->mDataParser.mFrame601.mFlrState[tmpRow] & (1<<tmpColumn))  //上按钮被登记
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
		else if(tmpParser->mID == 0x602)
		{
			T_COMMON_PARAMETRES *tmpPara = (T_COMMON_PARAMETRES *)gParameters.mData;
			unsigned char tmpFlr = tmpPara->mFlr-1;
			unsigned int tmpRow = tmpFlr/8;
			unsigned int tmpColumn = tmpFlr%8;
			if(tmpParser->mDataParser.mFrame602.mFlrState[tmpRow] & (1<<tmpColumn))  //下按钮被登记
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
		else if(tmpParser->mID == 0x610)
		{
			//WriteMPUData(pPortType, E_MPU_DATA_FLRPAD, tmpParser->mDataParser.mFrame601.mFlrState);
		}
	}
}

//=========================================
//  控制柜呼梯协议发送
//=========================================

void CallElevatorToController(T_UI_NODE_ID pButtonID)  //呼梯命令给控制柜
{
	Can_Message_Object tmpObject = {0};
	T_COMMON_PARAMETRES *tmpPara = (T_COMMON_PARAMETRES *)gParameters.mData;
	tmpObject.id = 0x100 + tmpPara->mFlr - 1;
	tmpObject.ext = 0;
	tmpObject.dlc = 8;
	if(pButtonID == E_ID_LOPPAD_UP)
		tmpObject.data[0] = 1;
	else if(pButtonID == E_ID_LOPPAD_DN)
		tmpObject.data[0] = 1<<8;
	SendFrameToController(E_CONTROLLER_PORT_CAN, (unsigned char *)(&tmpObject), sizeof(Can_Message_Object));
}

