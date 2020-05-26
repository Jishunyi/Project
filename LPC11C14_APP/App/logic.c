/*****************************************************************************
	Copyright (C), 2010-2018, BST Electric Co., Ltd.
	File name: logic.c
	Author: Jinwenjie 
	Version: V0 
	Date: 2018-11-08
	Description: 逻辑内存维护
	History: 
	<author>  <time>  <version>  <desc>
******************************************************************************/

//* Private include files -----------------------------------------------------
//* @{引用头文件声明区域(仅限本文件内使用)

#include "logic.h"
#include "user.h"

/**************************  断电保存数据  *********************************************************/
volatile  unsigned char ever_add_flr;
T_PARAMETRES  gParameters = {0};
extern T_MPU_DATA  gMPUDataBuffer[D_MPU_DATA_TYPE_NUM];
unsigned char RTC_NOW[6];
void ModifyMenuValue(int pUiID)
{
	static uint8_t count=0;
	T_COMMON_PARAMETRES *tmpCommPara = (T_COMMON_PARAMETRES *)gParameters.mData;
	uint8_t OrigmData=gParameters.mData[536];
	unsigned char tmpBuffer[24] = {0};
	unsigned int tmpFindFlag = 0;
	unsigned int tmpParaFlag = 0;
	//memcpy(OrigmData,gParameters.mData,sizeof(gParameters.mData));

	{ //E_ID_SETPAGE_VOLUME
		if(pUiID == E_ID_SETPAGE_VOLUME_ADD)
		{
			if(count==2)
			{
				count=0;
				tmpFindFlag = 1;
				if(tmpCommPara->mVolume < 100)
				{
					tmpParaFlag = 1;
					tmpCommPara->mVolume++;
				}
			}
			else
			{
				count ++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_VOLUME_DEL)
		{
			tmpFindFlag = 1;
			if(tmpCommPara->mVolume > 0)
			{
				tmpParaFlag = 1;
				tmpCommPara->mVolume--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_VOLUME_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "%d %%", tmpCommPara->mVolume);
				SetUiTextToMPU(E_ID_SETPAGE_VOLUME_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_LUMINARY
		if(pUiID == E_ID_SETPAGE_LUMINARY_ADD)
		{
			tmpFindFlag = 1;
			if(tmpCommPara->mLuminary < 100)
			{
				tmpParaFlag = 1;
				tmpCommPara->mLuminary++;	
			}
		}
		else if(pUiID == E_ID_SETPAGE_LUMINARY_DEL)
		{
			tmpFindFlag = 1;
			if(tmpCommPara->mLuminary > 30)
			{
				tmpParaFlag = 1;
				tmpCommPara->mLuminary--;	
			}
		}
		else if(pUiID == E_ID_SETPAGE_LUMINARY_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "%d cd/m2", tmpCommPara->mLuminary);
				SetUiTextToMPU(E_ID_SETPAGE_LUMINARY_VALUE, tmpBuffer, strlen((char*)tmpBuffer));	
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_STANDBYTIME
		if(pUiID == E_ID_SETPAGE_STANDBYTIME_ADD)
		{
			if(tmpCommPara->mStandbyTimeout < 60)
			{
				tmpCommPara->mStandbyTimeout++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_STANDBYTIME_DEL)
		{
			if(tmpCommPara->mStandbyTimeout > 0)
			{
				tmpCommPara->mStandbyTimeout--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_STANDBYTIME_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "%d min", tmpCommPara->mLuminary);
				SetUiTextToMPU(E_ID_SETPAGE_STANDBYTIME_VALUE, tmpBuffer, strlen((char*)tmpBuffer));	
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_STANDBYLUMINARY
		if(pUiID == E_ID_SETPAGE_STANDBYLUMINARY_ADD)
		{
			if(tmpCommPara->mStandbyLuminary < 60)
			{
				tmpCommPara->mStandbyLuminary++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_STANDBYLUMINARY_DEL)
		{
			if(tmpCommPara->mStandbyLuminary > 0)
			{
				tmpCommPara->mStandbyLuminary--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_STANDBYLUMINARY_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "%d cd/m2", tmpCommPara->mLuminary);
				SetUiTextToMPU(E_ID_SETPAGE_STANDBYLUMINARY_VALUE, tmpBuffer, strlen((char*)tmpBuffer));	
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_DISPLAYTYPE
		if(pUiID == E_ID_SETPAGE_DISPLAYTYPE_ADD)
		{
			if(gParameters.mDevType < E_DEV_LOPPAD)
			{
				gParameters.mDevType++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_DISPLAYTYPE_DEL)
		{
			if(gParameters.mDevType > E_DEV_DISPLAY)
			{
				gParameters.mDevType--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_DISPLAYTYPE_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(gParameters.mDevType == E_DEV_DISPLAY)
					sprintf((char*)tmpBuffer, "Display");
				else if(gParameters.mDevType == E_DEV_KEYPAD)
					sprintf((char*)tmpBuffer, "Keypad");
				else if(gParameters.mDevType == E_DEV_FLRPAD)
					sprintf((char*)tmpBuffer, "FloorPad");
				else if(gParameters.mDevType == E_DEV_DCS)
					sprintf((char*)tmpBuffer, "DCS");
				else if(gParameters.mDevType == E_DEV_LOPPAD)
					sprintf((char*)tmpBuffer, "LOP");
				SetUiTextToMPU(E_ID_SETPAGE_DISPLAYTYPE_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_DISPLAYROTATION
		if(pUiID == E_ID_SETPAGE_DISPLAYROTATION_ADD)
		{
			if(tmpCommPara->mRotate < E_ANGLE_270)
			{
				tmpCommPara->mRotate++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_DISPLAYROTATION_DEL)
		{
			if(tmpCommPara->mVolume > E_ANGLE_0)
			{
				tmpCommPara->mVolume--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_DISPLAYROTATION_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(tmpCommPara->mRotate == E_ANGLE_0)
					sprintf((char*)tmpBuffer, "0°");
				else if(tmpCommPara->mRotate == E_ANGLE_90)
					sprintf((char*)tmpBuffer, "90°");
				else if(tmpCommPara->mRotate == E_ANGLE_180)
					sprintf((char*)tmpBuffer, "180°");
				else if(tmpCommPara->mRotate == E_ANGLE_270)
					sprintf((char*)tmpBuffer, "270°");
				SetUiTextToMPU(E_ID_SETPAGE_DISPLAYROTATION_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_ARROWSCROLLSPEED
		if(pUiID == E_ID_SETPAGE_ARROWSCROLLSPEED_ADD)
		{
			if(tmpCommPara->mArrowScrollSpeed < 250)
			{
				tmpCommPara->mArrowScrollSpeed+=25;
			}
		}
		else if(pUiID == E_ID_SETPAGE_ARROWSCROLLSPEED_DEL)
		{
			if(tmpCommPara->mArrowScrollSpeed > 50)
			{
				tmpCommPara->mArrowScrollSpeed-=50;
			}
		}
		else if(pUiID == E_ID_SETPAGE_ARROWSCROLLSPEED_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "%d ms", tmpCommPara->mArrowScrollSpeed);
				SetUiTextToMPU(E_ID_SETPAGE_ARROWSCROLLSPEED_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_TIMEFORMAT
		if(pUiID == E_ID_SETPAGE_TIMEFORMAT_ADD)
		{
			if(tmpCommPara->mTimeFormat < E_TIME_MDYHMS)
			{
				tmpCommPara->mTimeFormat++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_TIMEFORMAT_DEL)
		{
			if(tmpCommPara->mTimeFormat > E_TIME_YMDHMS)
			{
				tmpCommPara->mTimeFormat--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_TIMEFORMAT_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(tmpCommPara->mTimeFormat == E_TIME_YMDHMS)
					sprintf((char*)tmpBuffer, "yyyy-MM-dd hh:mm:ss");
				else 
					sprintf((char*)tmpBuffer, "MM-dd-yyyy hh:mm:ss");
				SetUiTextToMPU(E_ID_SETPAGE_TIMEFORMAT_VALUE, tmpBuffer, strlen((char*)tmpBuffer));	
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_DOORADDR
		if(pUiID == E_ID_SETPAGE_DOORADDR_ADD)
		{
			if(count==2)
			{
				count=0;
				tmpFindFlag = 1;
				if(tmpCommPara->mDoorAddr < 3)
				{
					tmpParaFlag = 1;
					tmpCommPara->mDoorAddr++;
				}
			}
			else
			{
				count ++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_DOORADDR_DEL)
		{
			if(count==2)
			{
				count=0;
				tmpFindFlag = 1;
				if(tmpCommPara->mDoorAddr >0)
				{
					tmpParaFlag = 1;
					tmpCommPara->mDoorAddr--;
				}
			}
			else
			{
				count ++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_DOORADDR_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(tmpCommPara->mDoorAddr == E_DOOR_FRONT)
					sprintf((char*)tmpBuffer, "Front Door");
				else if(tmpCommPara->mDoorAddr == E_DOOR_REAR)
					sprintf((char*)tmpBuffer, "Rear  Door");
				else if(tmpCommPara->mDoorAddr == E_DOOR_LEFT)
					sprintf((char*)tmpBuffer, "Left  Door");
				else if(tmpCommPara->mDoorAddr == E_DOOR_RIGHT)
					sprintf((char*)tmpBuffer, "Right Door");
				SetUiTextToMPU(E_ID_SETPAGE_DOORADDR_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_CURFLOOR
		if(pUiID == E_ID_SETPAGE_CURFLOOR_ADD)
		{
			if(count==2)
			{
				count=0;
				tmpFindFlag = 1;
				if(tmpCommPara->add_flr <31)
				{
					tmpParaFlag = 1;
					tmpCommPara->add_flr++;
				}
			}
			else
			{
				count ++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_CURFLOOR_DEL)
		{
			if(count==2)
			{
				count=0;
				tmpFindFlag = 1;
				if(tmpCommPara->add_flr >1)
				{
					tmpParaFlag = 1;
					tmpCommPara->add_flr--;
				}
			}
			else
			{
				count ++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_CURFLOOR_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(tmpCommPara->add_flr<10)
				{
					sprintf((char*)tmpBuffer, " %d", tmpCommPara->add_flr);
				}
				else if(tmpCommPara->add_flr>9)
					sprintf((char*)tmpBuffer, "%d", tmpCommPara->add_flr);
				SetUiTextToMPU(E_ID_SETPAGE_CURFLOOR_VALUE, tmpBuffer, strlen((char*)tmpBuffer));			
			}
		}
	}
	{ //E_ID_SETPAGE_MINFLOOR
		if(pUiID == E_ID_SETPAGE_MINFLOOR_ADD)
		{
			if(tmpCommPara->mMinFlr < D_MAX_FLR)
			{
				tmpCommPara->mMinFlr++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_MINFLOOR_DEL)
		{
			if(tmpCommPara->mMinFlr > 1)
			{
				tmpCommPara->mMinFlr--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_MINFLOOR_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "Min %d Floor", tmpCommPara->mMinFlr);
				SetUiTextToMPU(E_ID_SETPAGE_MINFLOOR_VALUE, tmpBuffer, strlen((char*)tmpBuffer));

			}
			return;
		}
	}
	{ //E_ID_SETPAGE_MAXFLOOR
		if(pUiID == E_ID_SETPAGE_MAXFLOOR_ADD)
		{
			if(tmpCommPara->mMaxFlr < D_MAX_FLR)
			{
				tmpCommPara->mMaxFlr++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_MAXFLOOR_DEL)
		{
			if(tmpCommPara->mMaxFlr > 1)
			{
				tmpCommPara->mMaxFlr--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_MAXFLOOR_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				sprintf((char*)tmpBuffer, "Max %d Floor", tmpCommPara->mMaxFlr);
				SetUiTextToMPU(E_ID_SETPAGE_MAXFLOOR_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_INTERFACETYPE
		if(pUiID == E_ID_SETPAGE_INTERFACETYPE_ADD)
		{
			if(tmpCommPara->mInterfaceType < E_CONTROLLER_PORT_RSL1)
			{
				tmpCommPara->mInterfaceType++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_INTERFACETYPE_DEL)
		{
			if(tmpCommPara->mInterfaceType > E_CONTROLLER_PORT_UART)
			{
				tmpCommPara->mInterfaceType--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_INTERFACETYPE_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(tmpCommPara->mInterfaceType == E_CONTROLLER_PORT_UART)
					sprintf((char*)tmpBuffer, "UART");
				else if(tmpCommPara->mInterfaceType == E_CONTROLLER_PORT_CAN)
					sprintf((char*)tmpBuffer, "CAN");
				else if(tmpCommPara->mInterfaceType == E_CONTROLLER_PORT_SPI0)
					sprintf((char*)tmpBuffer, "SPI0");
				else if(tmpCommPara->mInterfaceType == E_CONTROLLER_PORT_SPI1)
					sprintf((char*)tmpBuffer, "SPI1");
				else if(tmpCommPara->mInterfaceType == E_CONTROLLER_PORT_RSL0)
					sprintf((char*)tmpBuffer, "RSL0");
				else if(tmpCommPara->mInterfaceType == E_CONTROLLER_PORT_RSL1)
					sprintf((char*)tmpBuffer, "RSL1");
				SetUiTextToMPU(E_ID_SETPAGE_INTERFACETYPE_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_INTERFACEBAUD
		if(pUiID == E_ID_SETPAGE_INTERFACEBAUD_ADD)
		{
			if(tmpCommPara->mInterfaceBaud > 0)
			{
				tmpCommPara->mInterfaceBaud--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_INTERFACEBAUD_DEL)
		{
			if(tmpCommPara->mInterfaceBaud > 0)
			{
				tmpCommPara->mInterfaceBaud--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_INTERFACEBAUD_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
			}
			return;
		}
	}
	{ //E_ID_SETPAGE_INTERFACEPARITY
		if(pUiID == E_ID_SETPAGE_INTERFACEPARITY_ADD)
		{
			if(tmpCommPara->mInterfaceParity < E_PARITY_EVEN)
			{
				tmpCommPara->mInterfaceParity++;
			}
		}
		else if(pUiID == E_ID_SETPAGE_INTERFACEPARITY_DEL)
		{
			if(tmpCommPara->mInterfaceParity > E_PARITY_NONE)
			{
				tmpCommPara->mInterfaceParity--;
			}
		}
		else if(pUiID == E_ID_SETPAGE_INTERFACEPARITY_VALUE)
		{
			tmpFindFlag = 1;
			tmpParaFlag = 1;
		}
		if(tmpFindFlag == 1)
		{
			if(tmpParaFlag)
			{
				if(tmpCommPara->mInterfaceParity == E_PARITY_NONE)
					sprintf((char*)tmpBuffer, "NONE");
				else if(tmpCommPara->mInterfaceParity == E_PARITY_ODD)
					sprintf((char*)tmpBuffer, "ODD");
				else if(tmpCommPara->mInterfaceParity == E_PARITY_EVEN)
					sprintf((char*)tmpBuffer, "EVEN");
				SetUiTextToMPU(E_ID_SETPAGE_INTERFACEPARITY_VALUE, tmpBuffer, strlen((char*)tmpBuffer));
			}
			return;
		}
	}

}

//======================================================================
//
//======================================================================
void LoadParameters(void)
{
	//如果读取的数据XOR不对或者全部为0，则表示还未进行过初始化，需要写入默认值
	unsigned int i;
//	unsigned int tmpXor = 0x55aa;  //付个初值，防止全为0的情况下XOR的结果也为0
	unsigned int tmpData = 0;
	unsigned int *tmpPointer = (unsigned int *)(&gParameters);
	T_COMMON_PARAMETRES *tmpPara = 0;
//	*tmpPointer = tmpData = FlashData(D_PARAMETER_ADDR);
	for(i=1;i<D_PARAMETRES_LEN/4;i++)
	{
		*(tmpPointer + i) = tmpData = FlashData(D_PARAMETER_ADDR + i*4);
//		tmpXor ^= tmpData;
	}
//	if(tmpXor != *tmpPointer)  //写入默认值
	{
		gParameters.mXor = 0x0000;
		gParameters.mDevType = E_DEV_LOPPAD;
		tmpPara = (T_COMMON_PARAMETRES *)gParameters.mData;
		if(tmpPara->Ui_Pic>2)
		{
			tmpPara->Ui_Pic=1;
		}
		if(tmpPara->add_flr>31)
		{
			tmpPara->add_flr=1;
			
		}
		ever_add_flr=tmpPara->add_flr;
//		for(m=0;m<D_MAX_FLR;m++)
//		{
//			unsigned char flr = i+1;
//			unsigned char temp;
//			
//			tmpPara->mFlrMap[m].mFlag.mEnable = 1;
//			tmpPara->mFlrMap[m].mFlag.mSecurity = 0;
//			tmpPara->mFlrMap[m].mFlag.mAlignment = 0;
//			
//			temp = flr / 100;
//			tmpPara->mFlrMap[m].mMap[0] = (temp == 0) ? ' ' : (temp + '0');
//			temp = flr % 100 / 10;
//			tmpPara->mFlrMap[m].mMap[1] = (tmpPara->mFlrMap[m].mMap[0] == ' ') ? ((temp == 0) ? ' ' : (temp + '0')) : ((temp == 0) ? '0' : (temp + '0'));
//			temp = flr % 10;
//			tmpPara->mFlrMap[m].mMap[2] = (tmpPara->mFlrMap[m].mMap[1] == ' ') ? ((temp == 0) ? ' ' : (temp + '0')) : ((temp == 0) ? '0' : (temp + '0'));
//		
//			tmpPara->mFlrMap[m].mPasswd[0] = '1';
//			tmpPara->mFlrMap[m].mPasswd[1] = '2';
//			tmpPara->mFlrMap[m].mPasswd[2] = '3';
//			tmpPara->mFlrMap[m].mPasswd[3] = '4';
//		}
//		for(m=0;m<D_MAX_BTN;m++)
//		{
//			tmpPara->mBtnMap[m].mMap = m + 'A';
//		}
//		
//		tmpPara->mDoorAddr = E_DOOR_FRONT;
//		
//		tmpPara->mRotate = 0;
//		
//		tmpPara->mMinFlr = 1;
//		tmpPara->mMaxFlr = D_MAX_FLR;
//		
//		tmpPara->mLuminary = 100;
//		tmpPara->mStandbyTimeout = 30;	//半小时  
//		tmpPara->mStandbyLuminary = 30;
//		tmpPara->mVolume = 80;
//		tmpPara->mFlr=03;
//		if(tmpPara->mFlr>64)
//			tmpPara->mFlr=64;
		
		//保存至Flash
		SaveParameters();
	}
}

void SyncParameters(void)
{
	unsigned int tmpID;
	T_COMMON_PARAMETRES *tmpPara = (T_COMMON_PARAMETRES *)gParameters.mData;
	//显示Splash界面
	SendPageIndexToMPU(D_PAGE_FROM_ROTATION(E_PAGE_SPLASH, tmpPara->mRotate));
	//同步参数到MPU设置菜单
	for(tmpID = E_ID_SETPAGE_VOLUME_VALUE; tmpID<= E_ID_SETPAGE_INTERFACEPARITY_VALUE; tmpID++)
	{
		ModifyMenuValue(tmpID);
		LoopForLedBlink();
	}
	//显示常规界面
	SendPageIndexToMPU(0);
}

void SaveParameters(void)
{
	//如果发现XOR值和当前数据不对，则表示数据有变化，需要保存
//	unsigned int i;
//	unsigned int tmpXor = 0x55aa;  //付个初值，防止全为0的情况下XOR的结果也为0
//	unsigned int *tmpPointer = (unsigned int *)(&gParameters);
//	for(i=1;i<D_PARAMETRES_LEN/4;i++)
//	{
//		tmpXor ^= *(tmpPointer + i);
//	}
//	if(tmpXor != gParameters.mXor)
	{
//		gParameters.mXor = tmpXor;
		
		__disable_irq();
		SectorPrepare(D_PARAMETER_SECTOR, D_PARAMETER_SECTOR);
		SectorErasure(D_PARAMETER_SECTOR,D_PARAMETER_SECTOR);
		blankChk(D_PARAMETER_SECTOR,D_PARAMETER_SECTOR) ; 
		SectorPrepare(D_PARAMETER_SECTOR, D_PARAMETER_SECTOR);
		RamToFalsh((unsigned long )(D_PARAMETER_ADDR), (unsigned long)((unsigned long *)(&gParameters)), 1024);
		__enable_irq();
		Delay(100);
	}
}

void SendPageIndexToMPU(unsigned char pIndex)
{
	T_WRITE_DATAREG tmpData = {0x00, 0x84, 0x5a, 0x01, 0x00, 0x00};
	T_COMMON_PARAMETRES *tmpCommPara = (T_COMMON_PARAMETRES *)gParameters.mData;
//	if((tmpCommPara->mRotate == E_ANGLE_90) || (tmpCommPara->mRotate == E_ANGLE_270))
//	{ 
//		tmpData.mDATA[3] = pIndex+D_PAGE_NUM;
//	}
//	else
	{
		tmpData.mDATA[3] = pIndex;
	}
	SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 6);
}

/**************************  MPU  *********************************************************/

T_MPU  gMPU = {E_INITIALIZE, 0};

void PoweronMPU(void)
{
	SetConnState(E_INITIALIZE);
	SetLedBinkFrequency(E_LEDBLINK_RAPID);
//	LPC_IOCON->PIO1_10 &= ~(0x07);	/*IO功能*/
//	LPC_GPIO1->DIR |= 1 << 10;		//output
//	LPC_GPIO1->DATA |= (1 << 10);	
}

void RestartMPU(void)  //立即发出重启信号
{
	gMPU.mState = E_INITIALIZE;
//	LPC_GPIO1->DATA &= ~(1 << 10);			
//	Delay(1000);
//	LPC_GPIO1->DATA |= (1 << 10);	
}

bool IsConnectedWithMPU(void)
{
	return (bool)(gMPU.mState == E_CONNECTED);
}

void ReceivedFromMPU(void)
{
	if(gMPU.mState != E_CONNECTED)
	{
		SetConnState(E_CONNECTED);
	}
	if(gMPU.mTimeout > 0)
	{
		gMPU.mDstTick = gSysTickCount + gMPU.mTimeout;
	}
}

void SetConnState(T_CONNSTATE pState)
{
	if(pState != gMPU.mState)
	{
		gMPU.mState = pState;
		if(pState == E_INITIALIZE)
			SetLedBinkFrequency(E_LEDBLINK_RAPID);
		else if(pState == E_SYNC)
			SetLedBinkFrequency(E_LEDBLINK_RAPID);
		else if(pState == E_CONNECTED)
			SetLedBinkFrequency(E_LEDBLINK_FAST);
		else if(pState == E_DISCONNECTED)
			SetLedBinkFrequency(E_LEDBLINK_SLOW);
		else
			SetLedBinkFrequency(E_LEDBLINK_RAPID);
	}
}

void LoopForMPU(void)
{
	if(gMPU.mTimeout > 0)
	{
		if(gMPU.mState == E_CONNECTED)
		{
			if(gSysTickCount > gMPU.mDstTick)
			{
				gMPU.mDstTick = gSysTickCount + gMPU.mTimeout;
				SetConnState(E_DISCONNECTED);
			}
		}	
	}
	else
	{
		if(gMPU.mState == E_DISCONNECTED)
		{
			SetConnState(E_CONNECTED);
		}
	}
}

T_FRAME_BUFFER  gRecvFrameBuffer = {0};

void RecvDataFromMPU(unsigned char pData)
{
	static uint8_t recStatus = E_HEADER_H;
	static uint8_t busy = 0;
	static T_FRAME *sCurRecvFrame = 0;
	static uint8_t count = 0;
	if(busy == 1)
		return;
	busy = 1;
	switch(recStatus)
	{
		case E_HEADER_H:
		{
			if(D_HEADER_H == (unsigned char)pData)
				recStatus = E_HEADER_L;
			break;
		}
		case E_HEADER_L:
		{
			if(D_HEADER_L == (unsigned char)pData)
				recStatus = E_FRAME_LEN;
			else
				recStatus = E_HEADER_H;
			break;
		}
		case E_FRAME_LEN:
		{
			if((pData == 0) || (pData > D_VALUELENGTH) || (gRecvFrameBuffer.mIn == ((gRecvFrameBuffer.mOut==0)?(D_FRAMEBUF_LEN-1):(gRecvFrameBuffer.mOut-1))))
			{
				recStatus = E_HEADER_H;
			}
			else
			{
				sCurRecvFrame = &(gRecvFrameBuffer.mFrames[gRecvFrameBuffer.mIn]);
				sCurRecvFrame->mLength = pData;
				recStatus = E_FRAME_CMD;
			}
			break;
		}
		case E_FRAME_CMD:
		{
			if(pData < 0x80 || pData > 0x83)
			{
				recStatus = E_HEADER_H;
			}
			else
			{
				sCurRecvFrame->mCmd = pData;
				recStatus = E_FRAME_DATA;
				count = 0;
			}
			break;
		}
		case E_FRAME_DATA:
		{
			sCurRecvFrame->mValue[count++] = pData;
			if((count+1) >= sCurRecvFrame->mLength)
			{
				gRecvFrameBuffer.mIn++;
				if(gRecvFrameBuffer.mIn == D_FRAMEBUF_LEN)
					gRecvFrameBuffer.mIn = 0;
				recStatus = E_HEADER_H;
			}
			break;
		}
		default:
		{
			recStatus = E_HEADER_H;
			break;
		}
	}
	busy = 0;
}

T_BUTTON  gLopPadButton[] = {
	{E_BTN_RELEASE, 0, 1000, 0},
	{E_BTN_RELEASE, 0, 1000, 0}
};

T_BUTTON  gCopPadButton[] = {
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0}
};

T_BUTTON  gFlrPadButton[] = {
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0},
	{E_BTN_RELEASE, 0, 300, 0}
};

T_BUTTON  gKeyPadButton[] = {
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0}
};

T_BUTTON  gSetMenuButton[] = {
	{E_BTN_RELEASE, 0, 500, 0},
	{E_BTN_RELEASE, 0, 500, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
};

T_BUTTON  gSetKeyButton[] = {
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
};

T_BUTTON  gPasswdButton[] = {
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
	{E_BTN_RELEASE, 0, 100, 0},
};

void LightButton(unsigned short pID)
{
	T_WRITE_DATAREG tmpData;
	tmpData.mADR_H = (unsigned char)(pID>>8);
	tmpData.mADR_L = (unsigned char)(pID);
	tmpData.mDATA[0] = 0x00;
	if(tmpData.mADR_L==0x20) tmpData.mDATA[1] = 0x02;
	else if(tmpData.mADR_L==0x21)tmpData.mDATA[1] = 0x01;
	SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
}

void ParseFrameFromMPU(T_FRAME *pFrame)				//模拟串口接收到有效帧的回调函数     RTC解析jsy
{
	static unsigned char count=0;
	if(pFrame)
	{
		ReceivedFromMPU();
		if(pFrame->mCmd == 0x83)
		{
			if(pFrame->mLength == 0x0c) //时间解析
			{
				static unsigned char tmpYearBuffer[4] ={0};
				static unsigned char tmpMonthBuffer[2] ={0xFF,0xFF};
				//static unsigned char tmpMonthBuffer=0xFF; 
				static unsigned char tmpDayBuffer[2] = {0};
				static unsigned char tmpHourBuffer[2] ={0xFF,0xFF}; //避免时钟数为0不显示
				static unsigned char tmpMinBuffer[2] ={0xFF,0xFF}; //避免分钟数为0不显示
				static unsigned char startkey = 0;
				
				unsigned char tmp = 0;
				unsigned char tmpData[4] = {0x30, 0x00, 0x00, 0x00};			
				unsigned int tmpYear = 0x7d0; //2000
				unsigned int tmpMonth = 0; 
				unsigned int tmpDay = 0; 
				unsigned int tmpHour = 0; 
				unsigned int tmpMin = 0;
				
				//年
				tmpYear += pFrame->mValue[3]; //例如：2022 年 pFrame->mValue[3] 值为22
				RTC_NOW[0]=pFrame->mValue[3];
				tmp = (unsigned char)(tmpYear/1000%10);			
				if((tmp != tmpYearBuffer[0])|| (startkey == 0))//千位
				{
					tmpData[1] = 0x03;
					tmpData[3] = tmp+1;
					SendFrameToMPU(0x82,tmpData,4);	//WriteMPUData();
					tmpYearBuffer[0] = tmp;
				}
				
				tmp = (unsigned char)(tmpYear/100%10);			
				if((tmp != tmpYearBuffer[1])|| (startkey == 0))//百位
				{
					tmpData[1] = 0x04;
					tmpData[3] = tmp+1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpYearBuffer[1] = tmp;
				}

				tmp = (unsigned char)(tmpYear/10%10);			
				if((tmp != tmpYearBuffer[2])|| (startkey == 0))//十位
				{
					tmpData[1] = 0x05;
					tmpData[3] = tmp+1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpYearBuffer[2] = tmp;
				}		
				
				tmp = (unsigned char)(tmpYear%10);			
				if((tmp != tmpYearBuffer[3])|| (startkey == 0))//个位
				{
					tmpData[1] = 0x06;
					tmpData[3] = tmp+1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpYearBuffer[3] = tmp;
				}			

				//month
				tmpMonth = pFrame->mValue[4]; 
				RTC_NOW[1]=pFrame->mValue[4];
				tmp = (unsigned char)tmpMonth/10%10;
				if(tmpMonthBuffer[0] != tmp|| (startkey == 0))//十位
				{
					tmpData[1] = 0x21;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpMonthBuffer[0] = tmp;				
				}
				
				tmp = (unsigned char)tmpMonth%10;
				if(tmpMonthBuffer[1] != tmp|| (startkey == 0))//个位
				{
					tmpData[1] = 0x22;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpMonthBuffer[1] = tmp;					
				}	
				
				//day
				tmpDay = pFrame->mValue[5]; //
				RTC_NOW[2]=pFrame->mValue[5];
				tmp = (unsigned char)(tmpDay/10%10);
				if((tmpDayBuffer[0] != tmp)|| (startkey == 0))//十位
				{
					tmpData[1] = 0x08;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpDayBuffer[0] = tmp;				
				}
				
				tmp = (unsigned char)(tmpDay%10);
				if((tmpDayBuffer[1] != tmp)|| (startkey == 0))//个位
				{
					tmpData[1] = 0x09;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpDayBuffer[1] = tmp;					
				}	
				//.号
				startkey = 1;
				tmpData[1] = 0x20;
				tmpData[3] = 12;
				SendFrameToMPU(0x82,tmpData,4);		
				tmpData[1] = 0x23;
				tmpData[3] = 12;
				SendFrameToMPU(0x82,tmpData,4);		
				
				//时
				tmpHour=pFrame->mValue[7];
				RTC_NOW[3]=pFrame->mValue[7];
				tmp = (unsigned char)tmpHour/10%10;
				if(tmpHourBuffer[0] != tmp)//十位
				{
					tmpData[1] = 0x0a;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpHourBuffer[0] = tmp;				
				}
				
				tmp = (unsigned char)tmpHour%10;
				if(tmpHourBuffer[1] != tmp)//个位
				{
					tmpData[1] = 0x0b;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpHourBuffer[1] = tmp;					
				}	
				
				//分
				tmpMin=pFrame->mValue[8];
				RTC_NOW[4]=pFrame->mValue[8];
				RTC_NOW[5]=0; //默认0秒
				tmp = (unsigned char)tmpMin/10%10;
				if(tmpMinBuffer[0] != tmp)//十位
				{
					tmpData[1] = 0x0c;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpMinBuffer[0] = tmp;				
				}
				
				tmp = (unsigned char)tmpMin%10;
				if(tmpMinBuffer[1] != tmp)//个位
				{
					tmpData[1] = 0x0d;
					tmpData[3] = tmp + 1;
					SendFrameToMPU(0x82,tmpData,4);	
					tmpMinBuffer[1] = tmp;					
				}	
				
				//:号
				tmpData[1] = 0x10;
				tmpData[3] = 11;
				SendFrameToMPU(0x82,tmpData,4);	
			}
		}
	}
}

T_UI_EFFECT *gUiEffectList = 0;

T_UI_EFFECT *GetEffectFromList(unsigned int pUiID)
{
	if(gUiEffectList)
	{
		T_UI_EFFECT *tmpEffect = gUiEffectList;
		while(tmpEffect)
		{
			if(tmpEffect->mUiID == pUiID)
			{
				return tmpEffect;
			}
			tmpEffect = tmpEffect->mNext;
		}
	}
	return (T_UI_EFFECT *)0;
}

T_UI_EFFECT *AddEffectToList(T_UI_EFFECT *pEffect) 
{
	if(gUiEffectList)
	{
		T_UI_EFFECT *tmpEffect = gUiEffectList;
		while(tmpEffect)
		{
			if(tmpEffect->mUiID == pEffect->mUiID)
			{
				return tmpEffect;
			}
			tmpEffect = tmpEffect->mNext;
		}
		tmpEffect->mNext = pEffect;
		return pEffect;
	}
	gUiEffectList = pEffect;
	return pEffect;
}

void ProcessButton(T_BUTTON *pButton, unsigned short pID)
{
	if(pButton->mPressState == E_BTN_PRESS) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
	{
		if(GetElapsedTime(pButton->mLastPressTick) >= pButton->mPressDelay)
		{
			//先发送灭灯的命令
			T_WRITE_DATAREG tmpData;
			tmpData.mADR_H = (unsigned char)(pID>>8);
			tmpData.mADR_L = (unsigned char)(pID);
			tmpData.mDATA[0] = 0x00;
			tmpData.mDATA[1] = 0x00;
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			
			pButton->mPressState = E_BTN_RELEASE;
			pButton->mPressCount = 0;
		}
	}
}

void SetUiTextToMPU(unsigned int pUiID, unsigned char *pText, unsigned int pLen)
{
	T_WRITE_DATAREG tmpData;
	tmpData.mADR_H = (unsigned char)(pUiID>>8);
	tmpData.mADR_L = (unsigned char)(pUiID);
	memcpy(tmpData.mDATA, pText, pLen);
	SendFrameToMPU(0x82, (unsigned char *)(&tmpData), pLen+2);
}

void LoopForUILogicOfMPU(void)//jsy 屏幕滚动
{
//	unsigned int i;
//	
//	//=按钮点亮操作==================================================================
//	for(i=0;i<(E_ID_LOPPAD_DN-E_ID_LOPPAD_UP+1);i++)
//	{
//		unsigned short tmpID = i+E_ID_LOPPAD_UP;
//		T_BUTTON *tmpButton = &gLopPadButton[i];
//		ProcessButton(tmpButton, tmpID);
//	}
//	for(i=0;i<(E_ID_COPPAD_ALARM-E_ID_COPPAD_OPEN+1);i++)
//	{
//		unsigned short tmpID = i+E_ID_COPPAD_OPEN;
//		T_BUTTON *tmpButton = &gCopPadButton[i];
//		ProcessButton(tmpButton, tmpID);
//	}
//	for(i=0;i<(E_ID_FLRPAD_64-E_ID_FLRPAD_1+1);i++)
//	{
//		unsigned short tmpID = i+E_ID_FLRPAD_1;
//		T_BUTTON *tmpButton = &gFlrPadButton[i];
////		ProcessButton(tmpButton, tmpID);
//	}
//	for(i=0;i<(E_ID_KEYPAD_CANCEL-E_ID_KEYPAD_0+1);i++)
//	{
//		T_BUTTON *tmpButton = &gKeyPadButton[i];
//		ProcessButton(tmpButton, i+E_ID_KEYPAD_0);
//	}	
//	for(i=0;i<(E_ID_PASSWDPAD_OK-E_ID_PASSWDPAD_0+1);i++)
//	{
//		T_BUTTON *tmpButton = &gPasswdButton[i];
//		ProcessButton(tmpButton, i+E_ID_PASSWDPAD_0);
//	}
//	for(i=0;i<(E_ID_SETKEY_INDICATORMAP-E_ID_SETKEY_ENTER+1);i++)
//	{
//		T_BUTTON *tmpButton = &gSetMenuButton[i];
//		ProcessButton(tmpButton, i+E_ID_SETKEY_ENTER);
//	}
//	for(i=0;i<(E_ID_SETPAGE_INTERFACEPARITY_DEL-E_ID_SETPAGE_VOLUME_ADD+1);i++)
//	{
//		T_BUTTON *tmpButton = &gSetKeyButton[i];
//		ProcessButton(tmpButton, i+E_ID_SETPAGE_VOLUME_ADD);
//	}	
//	
//	//=特效==================================================================
//	if(gUiEffectList)
//	{
//		T_UI_EFFECT *tmpEffect = gUiEffectList;
//		while(tmpEffect)
//		{
//			if(tmpEffect->mState == E_ES_RUNNING)
//			{
//				if(GetElapsedTime(tmpEffect->mCurTick) >= tmpEffect->mInterval)
//				{
//					if((tmpEffect->mPicIndex > tmpEffect->mPicEnd) || (tmpEffect->mPicIndex < tmpEffect->mPicStart))
//					{
//						tmpEffect->mPicIndex = tmpEffect->mPicStart;
//					}
//					tmpEffect->mPicIndex++;
//					if((tmpEffect->mPicIndex > tmpEffect->mPicEnd) || (tmpEffect->mPicIndex < tmpEffect->mPicStart))
//					{
//						tmpEffect->mPicIndex = tmpEffect->mPicStart;
//					}
//					{ //设置图片
//						T_WRITE_DATAREG tmpData;
//						tmpData.mADR_H = (unsigned char)(tmpEffect->mUiID>>8);
//						tmpData.mADR_L = (unsigned char)(tmpEffect->mUiID);
//						tmpData.mDATA[0] = 0x00;
//						tmpData.mDATA[1] = tmpEffect->mPicIndex;
//						SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
//					}
//					tmpEffect->mCurTick = gSysTickCount;
//				}
//			}
//			else if(tmpEffect->mState == E_ES_STOPPED)
//			{
//				//if(tmpEffect->mPicIndex != tmpEffect->mPicStart)
//				{
//						T_WRITE_DATAREG tmpData;
//						tmpData.mADR_H = (unsigned char)(tmpEffect->mUiID>>8);
//						tmpData.mADR_L = (unsigned char)(tmpEffect->mUiID);
//						tmpData.mDATA[0] = 0x00;
//						tmpEffect->mPicIndex = tmpEffect->mPicStart;
//						tmpData.mDATA[1] = tmpEffect->mPicIndex;
//						SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
//				}
//			}
//			else if(tmpEffect->mState ==E_ES_TWINKLE)
//			{
//				if(GetElapsedTime(tmpEffect->mCurTick) >= tmpEffect->mInterval)
//				{	
//						static unsigned count=0;
//						T_WRITE_DATAREG tmpData;
//						tmpData.mADR_H = (unsigned char)(tmpEffect->mUiID>>8);
//						tmpData.mADR_L = (unsigned char)(tmpEffect->mUiID);
//						tmpData.mDATA[0] = 0x00;
//						tmpEffect->mPicIndex = tmpEffect->mPicStart;
//						if(count==0)
//						{
//							tmpData.mDATA[1] = tmpEffect->mPicIndex;
//							count++;
//						}
//						else
//						{
//							tmpData.mDATA[1] =0;
//							count=0;
//						}
//						SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
//						tmpEffect->mCurTick = gSysTickCount;
//				}
//			}
//			tmpEffect = tmpEffect->mNext;
//		}
//	}
}

T_FRAME_BUFFER  gSendFrameBuffer = {0};

unsigned int gSendRightNow = 0;
void SendFrameToMPU(unsigned char pCmd, unsigned char *pValue, unsigned char pLen)
{
	T_FRAME *tmpCurFrame = 0;
	if((pCmd < 0x80 || pCmd > 0x83) || (pLen == 0) || (pValue == 0) || (gSendFrameBuffer.mIn == ((gSendFrameBuffer.mOut==0)?(D_FRAMEBUF_LEN-1):(gSendFrameBuffer.mOut-1))))
	{
		return;
	}
	tmpCurFrame = (T_FRAME *)(&(gSendFrameBuffer.mFrames[gSendFrameBuffer.mIn]));
	tmpCurFrame->mCmd = pCmd;
	tmpCurFrame->mLength = pLen +1;
	memcpy(tmpCurFrame->mValue, pValue, pLen);
	gSendFrameBuffer.mIn++;
	if(gSendFrameBuffer.mIn == D_FRAMEBUF_LEN)
		gSendFrameBuffer.mIn = 0;
	
	//在还未进入while主循环前，此时没有执行LoopForFrameToMPU，会造成发送缓冲区溢出从而丢失命令
	if(gSendRightNow == 0)
		LoopForFrameToMPU();
}

void LoopForFrameFromMPU(void)
{
	while(gRecvFrameBuffer.mOut != gRecvFrameBuffer.mIn)
	{
		T_FRAME *tmpCurFrame = &(gRecvFrameBuffer.mFrames[gRecvFrameBuffer.mOut++]);
		if(gRecvFrameBuffer.mOut == D_FRAMEBUF_LEN)
			gRecvFrameBuffer.mOut = 0;
		ParseFrameFromMPU(tmpCurFrame);
	}
	
	LoopForUILogicOfMPU();
}

void LoopForFrameToMPU(void)
{
	while(gSendFrameBuffer.mOut != gSendFrameBuffer.mIn)
	{
		T_FRAME *tmpCurFrame = &(gSendFrameBuffer.mFrames[gSendFrameBuffer.mOut++]);
		if(gSendFrameBuffer.mOut == D_FRAMEBUF_LEN)
			gSendFrameBuffer.mOut = 0;
		IOUARTCharPut(0, D_HEADER_H);
		IOUARTCharPut(0, D_HEADER_L);
		IOUART_Send((unsigned char*)tmpCurFrame, tmpCurFrame->mLength+1);
	}
}

/**************************  MCU 通信 缓冲区  *********************************************************/

void Callback_LUMINARY_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_VOLUME_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}

//======================================================================================================
void Callback_FLOOR_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	static unsigned int sFlrLen = 0;  //上一次显示的楼层个数
	unsigned int tmpNewFlrLen = 0;
	if((IsAscii(pNewValue[0]) == 1)&&(IsAscii(pNewValue[1]) == 1)&&(IsAscii(pNewValue[2]) == 1))
	{
		tmpNewFlrLen = 3;
	}
	else if(((IsAscii(pNewValue[1]) == 1)&&(IsAscii(pNewValue[2]) == 1))||((IsAscii(pNewValue[0]) == 1)&&(IsAscii(pNewValue[1]) == 1)))
	{
		tmpNewFlrLen = 2;
	}
	else if((IsAscii(pNewValue[2]) == 1)||(IsAscii(pNewValue[1]) == 1)||(IsAscii(pNewValue[0]) == 1))
	{
		tmpNewFlrLen = 1;
	}
	
	if(tmpNewFlrLen != sFlrLen)   //Clear Unused
	{
		if(sFlrLen == 1)
		{
			T_WRITE_DATAREG tmpData = {0x10, 0x03, 0x00, 0x00};  //5a a5 06 82 10 03 00 01
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
		}
		else if(sFlrLen == 2)
		{
			{
				T_WRITE_DATAREG tmpData = {0x10, 0x05, 0x00, 0x00};
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
			{
				T_WRITE_DATAREG tmpData = {0x10, 0x04, 0x00, 0x00};
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
		}
		else if(sFlrLen == 3)
		{
			{
				T_WRITE_DATAREG tmpData = {0x10, 0x06, 0x00, 0x00};
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
			{
				T_WRITE_DATAREG tmpData = {0x10, 0x07, 0x00, 0x00};
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
			{
				T_WRITE_DATAREG tmpData = {0x10, 0x08, 0x00, 0x00};
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
		}
		sFlrLen = tmpNewFlrLen;
	}
	
	//更新楼层图片
	
	if(tmpNewFlrLen == 1)
	{
		T_WRITE_DATAREG tmpData = {0x10, 0x03};
		tmpData.mDATA[0] = 0;
		if(IsAscii(pNewValue[0]) == 1) pNewValue[2]=pNewValue[0];
		else if(IsAscii(pNewValue[1]) == 1) pNewValue[2]=pNewValue[1];
		tmpData.mDATA[1] = GetAsciiPic(pNewValue[2]);
		SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
	}
	else if(tmpNewFlrLen == 2)
	{
		if(IsAscii(pNewValue[0]) == 1) 
		{
			pNewValue[2]=pNewValue[1];
			pNewValue[1]=pNewValue[0];
		}
		{
			T_WRITE_DATAREG tmpData = {0x10, 0x05};
			tmpData.mDATA[0] = 0;
			tmpData.mDATA[1] = GetAsciiPic(pNewValue[2]);
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
		}
		{
			T_WRITE_DATAREG tmpData = {0x10, 0x04};
			tmpData.mDATA[0] = 0;
			tmpData.mDATA[1] = GetAsciiPic(pNewValue[1]);
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
		}
	}
	else if(tmpNewFlrLen == 3)
	{
		{
			T_WRITE_DATAREG tmpData = {0x10, 0x06};
			tmpData.mDATA[0] = 0;
			tmpData.mDATA[1] = GetAsciiPic(pNewValue[2]);
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
		}
		{
			T_WRITE_DATAREG tmpData = {0x10, 0x07};
			tmpData.mDATA[0] = 0;
			tmpData.mDATA[1] = GetAsciiPic(pNewValue[1]);
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
		}
		{
			T_WRITE_DATAREG tmpData = {0x10, 0x08};
			tmpData.mDATA[0] = 0;
			tmpData.mDATA[1] = GetAsciiPic(pNewValue[0]);
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
		}
	}

}
void Callback_ARROW_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
 unsigned char tmpOldDir        =  pOldValue[0]     & 0x01;
 unsigned char tmpOldDisContro  = (pOldValue[0]>>1) & 0x03;
 unsigned char tmpDir   =  (pNewValue[0] & 0x0f);
 unsigned char tmpDisContro   = Standby_Mode;
 T_UI_EFFECT *tmpEffect = GetEffectFromList(E_ID_ARROW);
 if(!tmpEffect)
 {
  tmpEffect = (T_UI_EFFECT*)malloc(sizeof(T_UI_EFFECT));
  memset(tmpEffect, 0, sizeof(T_UI_EFFECT));
  tmpEffect->mUiID = E_ID_ARROW;
  tmpEffect->mInterval = 500;
  
  tmpEffect = AddEffectToList(tmpEffect);
 }

 if(tmpDir==UP_DIR)
 {
	 if(tmpDisContro==None_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 0};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mPicStart = 0;
			tmpEffect->mState = E_ES_STOPPED;
	 }
	 else if(tmpDisContro==Standby_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 0X01};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mPicStart = 1;
			tmpEffect->mState = E_ES_STOPPED;
	 }
	 else if(tmpDisContro==Flow_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 11};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			
			tmpEffect->mPicStart = 6;
			tmpEffect->mPicEnd = 10;
			tmpEffect->mInterval = 350;
			tmpEffect->mState = E_ES_RUNNING;
			tmpEffect->mPicIndex = tmpEffect->mPicStart;
	 }
	 else if(tmpDisContro==Twinkle_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 11};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mInterval = 800;
			tmpEffect->mPicStart = 6;
			tmpEffect->mState = E_ES_TWINKLE;
	 }
 }
 else if(tmpDir==DN_DIR)
 {
	 if(tmpDisContro==None_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 0};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mPicStart = 0;
			tmpEffect->mState = E_ES_STOPPED;
	 }
	 else if(tmpDisContro==Standby_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 0x02};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mPicStart = 0x02;
			tmpEffect->mState = E_ES_STOPPED;
	 }
	 else if(tmpDisContro==Flow_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 11};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			
			tmpEffect->mPicStart = 1;
			tmpEffect->mPicEnd = 5;
			tmpEffect->mInterval = 350;
			tmpEffect->mState = E_ES_RUNNING;
			tmpEffect->mPicIndex = tmpEffect->mPicStart;
	 }
	 else if(tmpDisContro==Twinkle_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 11};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mInterval = 800;
			tmpEffect->mPicStart = 1;
			tmpEffect->mState = E_ES_TWINKLE;
	 }
 }
 else if(tmpDir==UD_DIR)
 {
   if(tmpDisContro==Standby_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 3};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 3);
			tmpEffect->mPicStart = 1;
			tmpEffect->mState = E_ES_STOPPED;
	 }	 
 }
 else if(tmpDir==NONE)
 {
   if(tmpDisContro==Standby_Mode)
	 {
			T_WRITE_DATAREG tmpData = {(unsigned char)(E_ID_ARROW>>8), (unsigned char)(E_ID_ARROW), 0x00, 0};
			SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			tmpEffect->mPicStart = 0;
			tmpEffect->mState = E_ES_STOPPED;
	 }	 	 
 }
}

void Callback_FUNCTION_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	unsigned char tmpRow, tmpColumn;
	for(tmpRow=0;tmpRow<1;tmpRow++)
	{
		for(tmpColumn=0;tmpColumn<8;tmpColumn++)
		{
			if(pNewValue[tmpRow] & (1<<tmpColumn))
			{
				unsigned char i = 1 + tmpRow*8 + tmpColumn;
				{
					T_WRITE_DATAREG tmpData = {0x10, 0x0a};///////////////////////修改0x10->0x0a
					tmpData.mDATA[0] = 0;
					tmpData.mDATA[1] = i;
					SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
				}
				return;
			}
		}
	}
	{
		T_WRITE_DATAREG tmpData = {0x10, 0x0a};
		tmpData.mDATA[0] = 0;
		tmpData.mDATA[1] = 0;
		SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
	}
}
void Callback_FLOOR_2_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_ARROW_2_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_FUNCTION_2_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_VIDEO_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_IMAGE_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_AUDIO_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_TEXT_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_PERSON_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_LOADCAPACITY_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_LOGO_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_DATETIME_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	T_WRITE_DATAREG tmpData = {0x00, 0x9C,0x5A,0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //5A A5 09 82 00 9C year_high year_low month day hour min second
	tmpData.mDATA[2]=pNewValue[0];
	tmpData.mDATA[3]=pNewValue[1];
	tmpData.mDATA[4]=pNewValue[2];
	tmpData.mDATA[5]=pNewValue[3];
	tmpData.mDATA[6]=pNewValue[4];
	tmpData.mDATA[7]=pNewValue[5];
	SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 10);
}			
void Callback_BACKGROUND_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	//初始化背光指令 //正常模式
	T_WRITE_DATAREG BackgroundData = {0x00, 0x82, 0x00, 0x50,0x00,0x00};  //5A A5 07 82 00 82 背光 背光 00 00
	if(pNewValue[0]==0x01)//节电模式 //亮度00/64
	{
		BackgroundData.mDATA[1]=0x00;
	}
	else//正常模式 //亮度50/64
	{
		BackgroundData.mDATA[1]=0x50;
	}
	SendFrameToMPU(0x82, (unsigned char *)(&BackgroundData), 6);
}
void Callback_WEATHER_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_TEMPERATURE_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	
}
void Callback_FLRPAD_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	unsigned int tmpRow;
	unsigned int tmpColumn;
	for(tmpRow=0;tmpRow<6;tmpRow++)
	{
		if(pOldValue[tmpRow] != pNewValue[tmpRow])
		{
			for(tmpColumn=0;tmpColumn<6;tmpColumn++)
			{
				unsigned int i = tmpRow*8+tmpColumn;
				unsigned short tmpID = i + E_ID_LOPPAD_UP;
				T_BUTTON *tmpButton = &gFlrPadButton[i];
				if(pNewValue[tmpRow] & (1<<tmpColumn))  //按钮被登记
				{
					if(tmpButton->mPressState == E_BTN_RELEASE) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
					{
						//先发送亮灯的命令
						T_WRITE_DATAREG tmpData;
						tmpData.mADR_H = (unsigned char)(tmpID>>8);
						tmpData.mADR_L = (unsigned char)(tmpID);
						tmpData.mDATA[0] = 0x00;
						tmpData.mDATA[1] = 0x01;
						SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
					}
					tmpButton->mPressState = E_BTN_LIGHT;
				}
				else 
				{
					if(tmpButton->mPressState == E_BTN_LIGHT) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
					{
						//先发送灭灯的命令
						T_WRITE_DATAREG tmpData;
						tmpData.mADR_H = (unsigned char)(tmpID>>8);
						tmpData.mADR_L = (unsigned char)(tmpID);
						tmpData.mDATA[0] = 0x00;
						tmpData.mDATA[1] = 0x00;
						SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
						tmpButton->mPressState = E_BTN_RELEASE;
						tmpButton->mPressCount = 0;
					}
				}
			}
		}
	}
}
void Callback_LOPPAD_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	unsigned int i;
	for(i=0;i<2;i++)
	{
		unsigned short tmpID = i + E_ID_LOPPAD_UP;
		T_BUTTON *tmpButton = &gLopPadButton[i];
		if(pNewValue[0] & (0x01<<i))
		{
			unsigned char j=pNewValue[0] & (0x01<<i);
			if(tmpButton->mPressState == E_BTN_RELEASE) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
			{
				//先发送亮灯的命令
				T_WRITE_DATAREG tmpData;
				tmpData.mADR_H = (unsigned char)(tmpID>>8);
				tmpData.mADR_L = (unsigned char)(tmpID);
				tmpData.mDATA[0] = 0x00;
				if(j==0x01) tmpData.mDATA[1] = 0x02;
				if(j==0x02) tmpData.mDATA[1] = 0x01;
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
			tmpButton->mPressState = E_BTN_LIGHT;
		}
		else
		{
			if(tmpButton->mPressState == E_BTN_LIGHT) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
			{
				//先发送灭灯的命令
				T_WRITE_DATAREG tmpData;
				tmpData.mADR_H = (unsigned char)(tmpID>>8);
				tmpData.mADR_L = (unsigned char)(tmpID);
				tmpData.mDATA[0] = 0x00;
				tmpData.mDATA[1] = 0x00;
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
				tmpButton->mPressState = E_BTN_RELEASE;
				tmpButton->mPressCount = 0;
			}
		}
	}
}
void Callback_COPPAD_Changed(void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue)
{
	unsigned int i;
	for(i=0;i<3;i++)
	{
		unsigned short tmpID = i + E_ID_COPPAD_OPEN;
		T_BUTTON *tmpButton = &gCopPadButton[i];
		if(pNewValue[0] & (0x01<<i))
		{
			if(tmpButton->mPressState == E_BTN_RELEASE) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
			{
				//先发送亮灯的命令
				T_WRITE_DATAREG tmpData;
				tmpData.mADR_H = (unsigned char)(tmpID>>8);
				tmpData.mADR_L = (unsigned char)(tmpID);
				tmpData.mDATA[0] = 0x00;
				tmpData.mDATA[1] = 0x01;
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
			}
			tmpButton->mPressState = E_BTN_LIGHT;
		}
		else
		{
			if(tmpButton->mPressState == E_BTN_LIGHT) //只有为E_BTN_PRESS状态时才需要监控并在对应时间灭灯
			{
				//先发送灭灯的命令
				T_WRITE_DATAREG tmpData;
				tmpData.mADR_H = (unsigned char)(tmpID>>8);
				tmpData.mADR_L = (unsigned char)(tmpID);
				tmpData.mDATA[0] = 0x00;
				tmpData.mDATA[1] = 0x00;
				SendFrameToMPU(0x82, (unsigned char *)(&tmpData), 4);
				tmpButton->mPressState = E_BTN_RELEASE;
				tmpButton->mPressCount = 0;
			}
		}
	}
}

T_MPU_DATA  gMPUDataBuffer[D_MPU_DATA_TYPE_NUM] = {
	{Callback_LUMINARY_Changed, 0, 1, 0, 1},
	{Callback_VOLUME_Changed, 0, 1, 0, 1},
	{Callback_FLOOR_Changed, 0, 3, 0, 1},
	{Callback_ARROW_Changed, 0, 1, 0, 1},
	{Callback_FUNCTION_Changed, 0, 4, 0, 1},
	{Callback_FLOOR_2_Changed, 0, 3, 0, 1},
	{Callback_ARROW_2_Changed, 0, 1, 0, 1},
	{Callback_FUNCTION_2_Changed, 0, 4, 0, 1},
	{Callback_VIDEO_Changed, 0, 1, 0, 1},
	{Callback_IMAGE_Changed, 0, 1, 0, 1},
	{Callback_AUDIO_Changed, 0, 1, 0, 1},
	{Callback_TEXT_Changed, 0, 32, 0, 1},
	{Callback_PERSON_Changed, 0, 8, 0, 1},
	{Callback_LOADCAPACITY_Changed, 0, 8, 0, 1},
	{Callback_LOGO_Changed, 0, 1, 0, 1},
	{Callback_DATETIME_Changed, 0, 6, 0, 1},   //year,month,day,hour,minute,second //0 6 0 1
	{Callback_BACKGROUND_Changed, 0, 1, 0, 1},   //背光调节
	{Callback_WEATHER_Changed, 0, 1, 0, 1},
	{Callback_TEMPERATURE_Changed, 0, 1, 0, 1}, // -127~128
	{Callback_FLRPAD_Changed, 0, D_MAX_FLR/8, 0, 0},
	{Callback_LOPPAD_Changed, 0, 1, 0, 1},
	{Callback_COPPAD_Changed, 0, 1, 0, 0},
	
};
unsigned char Testarr[5]={0};
void WriteMPUData(T_CONTROLLER_PORT_TYPE pPortType, T_MPU_DATA_TYPE pType, unsigned char *pValue)
{
	T_MPU_DATA *tmpDataPointer = &gMPUDataBuffer[pType];
	if(tmpDataPointer->mValue == 0)
	{
		tmpDataPointer->mValue = malloc(tmpDataPointer->mLength);
	}
	
	if(tmpDataPointer->mFilterEnable > 0)
	{
		if(memcmp(tmpDataPointer->mValue, pValue, tmpDataPointer->mLength) == 0)
		{
			return;
		}
	}
	
	tmpDataPointer->mNeedSendToMPU = D_NEEDSEND_MAX;
	tmpDataPointer->mCallbackFunc(tmpDataPointer, pPortType, tmpDataPointer->mValue, pValue);
	memcpy(tmpDataPointer->mValue, pValue, tmpDataPointer->mLength);
}

T_MPU_DATA *ReadMPUData(T_MPU_DATA_TYPE pType)
{
	return &gMPUDataBuffer[pType];
}

/******MCU<->控制柜 通信 缓冲区******************************************************************************/

T_RAWBUF_FROM_CONTROLLER  gRawBuffersOfController[D_CONTROLLER_PORT_TYPE_NUM] = {0};

void SetFrameIntervalOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned int pTime)
{
	if(pTime > 0)
		gRawBuffersOfController[pPortType].mFrameInterval = pTime;
}

void SetFrameHeaderOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pValue, unsigned char pLen)
{
	if(pLen < 8)
	{
		memcpy(gRawBuffersOfController[pPortType].mHeader, pValue, pLen);
		gRawBuffersOfController[pPortType].mHeaderLen = pLen;
	}
}

void SetFrameFooterOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pValue, unsigned char pLen)
{
	if(pLen < 8)
	{
		memcpy(gRawBuffersOfController[pPortType].mFooter, pValue, pLen);
		gRawBuffersOfController[pPortType].mFooterLen = pLen;
	}
}

#if !(D_FILTER_RAW_DATA_FROM_CONTROLLER)
//原始字节处理
void RecvRawDataFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char pData)
{
	if(gRawBuffersOfController[pPortType].mRawBufferIndex < 63)  //此处需要和ParseFrameFromControllerLoop的ParseFrameFromController配合使用，已经实现只要还没有执行完ParseFrameFromController，就不会接收新帧
	{
		gRawBuffersOfController[pPortType].mRawBuffer[gRawBuffersOfController[pPortType].mRawBufferIndex++] = pData;
	}
	gRawBuffersOfController[pPortType].mDataRecvTime = gSysTickCount;  //要放在上面大括号的外面
}
#endif

#if !(D_FILTER_RAW_FRAME_FROM_CONTROLLER)
//原始帧处理
void RecvRawFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	if(pFrame && pLen)
	{		
		if(memcmp(gRawBuffersOfController[pPortType].mRawBuffer, pFrame, pLen) != 0)
		{
			gRawBuffersOfController[pPortType].mRawBufferIndex = pLen;
			memcpy(gRawBuffersOfController[pPortType].mRawBuffer, pFrame, pLen);
		}
	}
}
#endif

void LoopForFrameFromController(void)
{
	unsigned int i;
	for(i=0;i<D_CONTROLLER_PORT_TYPE_NUM;i++)
	{
		if(gRawBuffersOfController[i].mRawBufferIndex > 0)
		{
			unsigned int tmpFlag = 0;  //true表示解析失败
			
			if(gRawBuffersOfController[i].mFrameInterval >0)  //查看是否需要断帧
			{
				uint32_t tmpInterval = GetElapsedTime(gRawBuffersOfController[i].mDataRecvTime);
				if(tmpInterval < gRawBuffersOfController[i].mFrameInterval)
				{
					continue;
				}
			}
			
			//Header
			if((gRawBuffersOfController[i].mHeaderLen > 0) && (gRawBuffersOfController[i].mHeaderLen < gRawBuffersOfController[i].mRawBufferIndex))
			{
				if(memcmp(gRawBuffersOfController[i].mHeader, gRawBuffersOfController[i].mRawBuffer, gRawBuffersOfController[i].mHeaderLen) != 0)
					tmpFlag = 1;
			}
			//Footer
			if((tmpFlag == 0) && (gRawBuffersOfController[i].mFooterLen > 0) && (gRawBuffersOfController[i].mFooterLen < gRawBuffersOfController[i].mRawBufferIndex))
			{
				if(memcmp(gRawBuffersOfController[i].mFooter, gRawBuffersOfController[i].mRawBuffer+gRawBuffersOfController[i].mRawBufferIndex-gRawBuffersOfController[i].mFooterLen, gRawBuffersOfController[i].mFooterLen) != 0)
					tmpFlag = 1;
			}
			
			if(tmpFlag == 1)
			{
				gRawBuffersOfController[i].mRawBufferIndex = 0;
			}
			else
			{
				ParseFrameFromController((T_CONTROLLER_PORT_TYPE)i, gRawBuffersOfController[i].mRawBuffer, gRawBuffersOfController[i].mRawBufferIndex);
				gRawBuffersOfController[i].mRawBufferIndex = 0;
			}
		}
	}
}

void SendFrameToController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen)
{
	if(pPortType == E_CONTROLLER_PORT_UART)
	{
		UARTSend(pFrame, pLen);
	}
	else if(pPortType == E_CONTROLLER_PORT_CAN)
	{
		CANSend((Can_Message_Object*)pFrame);
	}
}

