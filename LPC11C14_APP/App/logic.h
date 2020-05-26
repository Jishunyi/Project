#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "string.h"
#include "stdlib.h"

#include "LPC11xx.h"
#include "DrvFlash.h"
#include "DrvTimer.h"
#include "DrvSysClk.h"
#include "DrvCAN.h"
#include "DrvUART.h"
#include "IOUART.h"

extern unsigned int gSendRightNow;

unsigned int IsAscii(unsigned char pData);
unsigned char GetAsciiPic(unsigned char pData);

//---------------------------------------------------

typedef enum{
	E_CONTROLLER_PORT_UART=0,  //包含RS485和RS232,两个互斥使用
	E_CONTROLLER_PORT_CAN,
	E_CONTROLLER_PORT_SPI0,
	E_CONTROLLER_PORT_SPI1,
	E_CONTROLLER_PORT_RSL0,
	E_CONTROLLER_PORT_RSL1,
}T_CONTROLLER_PORT_TYPE;

#define D_CONTROLLER_PORT_TYPE_NUM 6

enum{
	E_PARITY_NONE = 0,
	E_PARITY_ODD,
	E_PARITY_EVEN
};

/**************************  断电保存数据  *********************************************************/

#define D_PARAMETRES_LEN 1024

typedef enum{
	E_DEV_DISPLAY = 0,			//显示界面
	E_DEV_KEYPAD,				//数字键盘界面
	E_DEV_FLRPAD,				//楼层键盘
	E_DEV_LOPPAD=0,				//外呼
	E_DEV_DCS,					//目的层选层器
} T_DEV_TYPE;

typedef struct{
	unsigned char  mEnable : 1;						//指示当前楼层是否可以被呼叫
	unsigned char  mSecurity : 1;					//指示当前楼层是否为加密楼层
	unsigned char  mAlignment : 2;				//指示当前楼层文字对其方式：0：居中，1：左对齐，2:右对齐
	unsigned char  mReserve : 4;
} T_FLR_FLAG;

typedef struct{
	T_FLR_FLAG		mFlag;
	unsigned char mMap[3]; //物理楼层对应的三位映射字母
	char					mPasswd[4];
}T_FLR_MAP;

typedef struct{
	unsigned char mNum;
	unsigned char mMap; 		//物理按键对应的映射
	unsigned char mReserve[2];
}T_BTN_MAP;

enum{
	E_DOOR_FRONT = 0,
	E_DOOR_REAR,
	E_DOOR_LEFT,
	E_DOOR_RIGHT
};

enum{
	E_ANGLE_0 = 0,
	E_ANGLE_90,
	E_ANGLE_180,
	E_ANGLE_270
};

enum{
	E_TIME_YMDHMS = 0,
	E_TIME_MDYHMS,
};

#define D_MAX_FLR  64
#define D_MAX_BTN  4		//支持映射的按钮主要是Keypad中的ABCD等特殊字符

typedef struct{
	unsigned char Reserve1;
	unsigned char Reserve2;
	unsigned char Reserve3;
	unsigned char add_flr;
	unsigned char Ui_Pic;
	T_FLR_MAP					mFlrMap[D_MAX_FLR];
	T_BTN_MAP					mBtnMap[D_MAX_BTN];
	
	//Device
	unsigned char			mLuminary;
	unsigned char			mStandbyTimeout;   //minute
	unsigned char			mStandbyLuminary;
	
	unsigned char 		mVolume;
	
	//Theme
	unsigned char			mRotate;	//屏幕旋转角度，0~360
	
	unsigned char 		mArrowScrollSpeed;		//ms	注意只能使用char类型，不要使用short类型
	unsigned char			mTimeFormat;
	
	//Elevator
	unsigned char 	  mDoorAddr;		//显示器所在方位，前门、后门、左门、右门
	unsigned char 	  mFlr;					//设备安装的楼层

	unsigned char			mMinFlr;		//最小楼层，从1开始
	unsigned char			mMaxFlr;		//最大楼层
	
	unsigned char			mInterfaceType;  //T_CONTROLLER_PORT_TYPE
	unsigned int			mInterfaceBaud;
	unsigned char			mInterfaceParity;

} T_COMMON_PARAMETRES;

typedef struct{
	unsigned int 		mXor;
	T_DEV_TYPE			mDevType;
	unsigned char		mData[D_PARAMETRES_LEN-8];
} T_PARAMETRES;

extern T_PARAMETRES  gParameters;

extern void LoadParameters(void);
extern void SyncParameters(void);
extern void SaveParameters(void);
extern void UserParameters(void);

//默认页面为横版，如果想要切换为竖版页面，直接在当前page上加D_PAGE_NUM
#define  D_PAGE_NUM   20

typedef enum {
	E_PAGE_SPLASH = 0,
	E_PAGE_DISPLAY=0,			//显示界面,与T_DEV_TYPE相同
	E_PAGE_KEYPAD,		//数字键盘界面
	E_PAGE_FLRPAD,		//楼层键盘
	E_PAGE_LOPPAD,		//外呼
	E_PAGE_DCS,				//目的层选层器
	//=========================================
	E_PAGE_PASSWD,	  //密码键盘界面
	E_PAGE_MENU_DEVICE=3,		//包括亮度、音量等设置
	E_PAGE_MENU_THEME,		//包括主题选择、屏幕旋转/箭头滚动速度、时间格式（中英文）、
	E_PAGE_MENU_ELEVATOR,	//包括最低、最高楼层、当前楼层、
	E_PAGE_MENU_FLRMAP,
	E_PAGE_MENU_BTNMAP,
	E_PAGE_MENU_INDICATORMAP,	//目的层中的方向映射
} T_PAGE_ID;

typedef enum {
	E_PAGE_DISPLAY0 = 0,
	E_PAGE_DISPLAY1=1,			//显示界面,与T_DEV_TYPE相同
	E_PAGE_DISPLAY2,		//数字键盘界面
	E_PAGE_SETADDR=6,		//楼层键盘
	E_PAGE_LOPPAD1,		//外呼
	E_PAGE_DCS1,				//目的层选层器
	//=========================================
	E_PAGE_PASSWD1,	  //密码键盘界面
	E_PAGE_MENU_DEVICE1,		//包括亮度、音量等设置
	E_PAGE_MENU_THEME1,		//包括主题选择、屏幕旋转/箭头滚动速度、时间格式（中英文）、
	E_PAGE_MENU_ELEVATOR1,	//包括最低、最高楼层、当前楼层、
	E_PAGE_MENU_FLRMAP1,
	E_PAGE_MENU_BTNMAP1,
	E_PAGE_MENU_INDICATORMAP1,	//目的层中的方向映射
} T_PAGE_ID1;



#define D_PAGE_FROM_DEV(dev)   (E_PAGE_DISPLAY+dev)
#define D_PAGE_FROM_ROTATION(page, rotate)  ((page)+(((rotate==E_ANGLE_90)||(rotate==E_ANGLE_270))?D_PAGE_NUM:0))

extern void SetUiTextToMPU(unsigned int pUiID, unsigned char *pText, unsigned int pLen);
extern void SendPageIndexToMPU(unsigned char pIndex);

/**************************  MPU  *********************************************************/

enum{
	E_LEDBLINK_RAPID = 100,
	E_LEDBLINK_FAST = 500,
	E_LEDBLINK_SLOW = 1500
};

typedef enum {
	E_INITIALIZE = 0,  //重启状态
	E_SYNC,						 //同步信息状态
	E_DISCONNECTED,		 //未连接
	E_CONNECTED				 //连接上，只要在超时时间内持续接收到MPU发来的数据，即为连接状态
} T_CONNSTATE;

void SetConnState(T_CONNSTATE pState);

typedef struct {
	T_CONNSTATE  mState;
	unsigned int mTimeout;  //ms,连接超时周期,如果为0表示不进行超时判断
	uint32_t     mDstTick;
} T_MPU;

extern void PoweronMPU(void);  					//对MPU上电
extern void RestartMPU(void); 					//重启MPU
extern bool IsConnectedWithMPU(void);		//判断是否和MPU通信正常？
extern void ReceivedFromMPU(void);			//记录从MPU获取了一个有效帧
extern void LoopForMPU(void);						//放在while(1)循环中检测和MPU通信是否异常

/******MCU <-> MPU 通信******************************************************************************/

#define D_HEADER_H ((unsigned char)0x5A)
#define D_HEADER_L ((unsigned char)0xA5)
	
#define D_VALUELENGTH 64
	
typedef enum{
	E_HEADER_H ,
	E_HEADER_L,
	E_FRAME_LEN,
	E_FRAME_CMD,
	E_FRAME_DATA
} T_FRAME_SM;  			//mpu<->mcu的协议帧状态机

typedef struct {
	unsigned char  mLength;
	unsigned char  mCmd;
	unsigned char  mValue[D_VALUELENGTH];		  //数据
	unsigned char  mReserve[2];
} T_FRAME;

/////////////////////////////////////////////////////////////////////////////////
typedef struct {	 
	unsigned char  mADR;
	unsigned char  mDATA[D_VALUELENGTH-1]; //以字节（1byte）写入
} T_WRITE_CRTLREG;   //0X80

typedef struct {	
	unsigned char  mADR;
	unsigned char  mLEN;
} T_READ_CRTLREG;   //0X81

#define D_UI_TOUCH_OFFSET  0x1000
#define ID_TOUCH(x)  	(x+D_UI_TOUCH_OFFSET) //从ui控件计算其对应的触控控件ID
#define ID_UINODE(x)  (x-D_UI_TOUCH_OFFSET) //从触控控件计算其对应的ui控件ID

//=====================================================================================================
//ui控件的ID号分配,对于相对应的触控控件ID为[UI ID]+D_UI_TOUCH_OFFSET。
//  如果里面的触控ID的个数有变化，需要同步修改gLopPadButton、gCopPadButton等
//=====================================================================================================
typedef enum{ 
	E_ID_VERSION = 0x1200,  
	//=======================================================================
	E_ID_FLR_ONE = 0x1003,
	E_ID_FLR_TWO_UNIT,
	E_ID_FLR_TWO_TEN,
	E_ID_FLR_THREE_UNIT,
	E_ID_FLR_THREE_TEN,
	E_ID_FLR_THREE_HUNDRED,
	//-----------------------------
	E_ID_ARROW = 0x1009,
	//-----------------------------
	E_ID_FUNCTION = 0x100a,
	//-----------------------------
	E_ID_FLR_2_ONE,
	E_ID_FLR_2_TWO_UNIT,
	E_ID_FLR_2_TWO_TEN,
	E_ID_FLR_2_THREE_UNIT,
	E_ID_FLR_2_THREE_TEN,
	E_ID_FLR_2_THREE_HUNDRED,
	//-----------------------------
	E_ID_ARROW_2,
	//-----------------------------
	E_ID_FUNCTION_2,
	//=======================================================================
	E_ID_LOPPAD_UP = 0x1020,
	E_ID_LOPPAD_DN,
	//-----------------------------
	E_ID_COPPAD_OPEN = 0x1030,
	E_ID_COPPAD_CLOSE,
	E_ID_COPPAD_ALARM,
	//-----------------------------
	E_ID_FLRPAD_1 = 0x1040,
	E_ID_FLRPAD_2,
	E_ID_FLRPAD_3,
	E_ID_FLRPAD_4,
	E_ID_FLRPAD_5,
	E_ID_FLRPAD_6,
	E_ID_FLRPAD_7,
	E_ID_FLRPAD_8,
	E_ID_FLRPAD_9,
	E_ID_FLRPAD_10,
	E_ID_FLRPAD_11,
	E_ID_FLRPAD_12,
	E_ID_FLRPAD_13,
	E_ID_FLRPAD_14,
	E_ID_FLRPAD_15,
	E_ID_FLRPAD_16,
	E_ID_FLRPAD_17,
	E_ID_FLRPAD_18,
	E_ID_FLRPAD_19,
	E_ID_FLRPAD_20,
	E_ID_FLRPAD_21,
	E_ID_FLRPAD_22,
	E_ID_FLRPAD_23,
	E_ID_FLRPAD_24,
	E_ID_FLRPAD_25,
	E_ID_FLRPAD_26,
	E_ID_FLRPAD_27,
	E_ID_FLRPAD_28,
	E_ID_FLRPAD_29,
	E_ID_FLRPAD_30,
	E_ID_FLRPAD_31,
	E_ID_FLRPAD_32,
	E_ID_FLRPAD_33,
	E_ID_FLRPAD_34,
	E_ID_FLRPAD_35,
	E_ID_FLRPAD_36,
	E_ID_FLRPAD_37,
	E_ID_FLRPAD_38,
	E_ID_FLRPAD_39,
	E_ID_FLRPAD_40,
	E_ID_FLRPAD_41,
	E_ID_FLRPAD_42,
	E_ID_FLRPAD_43,
	E_ID_FLRPAD_44,
	E_ID_FLRPAD_45,
	E_ID_FLRPAD_46,
	E_ID_FLRPAD_47,
	E_ID_FLRPAD_48,
	E_ID_FLRPAD_49,
	E_ID_FLRPAD_50,
	E_ID_FLRPAD_51,
	E_ID_FLRPAD_52,
	E_ID_FLRPAD_53,
	E_ID_FLRPAD_54,
	E_ID_FLRPAD_55,
	E_ID_FLRPAD_56,
	E_ID_FLRPAD_57,
	E_ID_FLRPAD_58,
	E_ID_FLRPAD_59,
	E_ID_FLRPAD_60,
	E_ID_FLRPAD_61,
	E_ID_FLRPAD_62,
	E_ID_FLRPAD_63,
	E_ID_FLRPAD_64,	
	//=======================================================================
	//----------------------------- 数字键盘，触摸屏或者实体键盘按下键后给KEYPAD，然后由KEYPAD将整理过后的回显数据给INPUTECHO
	E_ID_KEYPAD_0 = 0x1090,
	E_ID_KEYPAD_1,
	E_ID_KEYPAD_2,
	E_ID_KEYPAD_3,
	E_ID_KEYPAD_4,
	E_ID_KEYPAD_5,
	E_ID_KEYPAD_6,
	E_ID_KEYPAD_7,
	E_ID_KEYPAD_8,
	E_ID_KEYPAD_9,
	E_ID_KEYPAD_A,
	E_ID_KEYPAD_B,
	E_ID_KEYPAD_C,
	E_ID_KEYPAD_D,
	E_ID_KEYPAD_DEL,
	E_ID_KEYPAD_OK,
	E_ID_KEYPAD_CANCEL,
	//-----------------------------
	E_ID_INPUTECHO_ONE = 0x10B0,
	E_ID_INPUTECHO_TWO_UNIT,
	E_ID_INPUTECHO_TWO_TEN,
	E_ID_INPUTECHO_THREE_UNIT,
	E_ID_INPUTECHO_THREE_TEN,
	E_ID_INPUTECHO_THREE_HUNDRED,
	//=======================================================================
	E_ID_PASSWDPAD_0 = 0x10C0,   //密码键盘
	E_ID_PASSWDPAD_1,
	E_ID_PASSWDPAD_2,
	E_ID_PASSWDPAD_3,
	E_ID_PASSWDPAD_4,
	E_ID_PASSWDPAD_5,
	E_ID_PASSWDPAD_6,
	E_ID_PASSWDPAD_7,
	E_ID_PASSWDPAD_8,
	E_ID_PASSWDPAD_9,
	E_ID_PASSWDPAD_DEL,
	E_ID_PASSWDPAD_OK,
	//=======================================================================
	//-----------------------------
	E_ID_SETKEY_ENTER = 0x1100,   //设置按钮
	E_ID_SETKEY_BACK,				//设置返回按钮
	E_ID_SETKEY_DEVICE = 0x1102,		//包括亮度、音量等设置
	E_ID_SETKEY_THEME,		//包括主题选择、箭头滚动速度、时间格式（中英文）、
	E_ID_SETKEY_ELEVATOR,	//包括最低、最高楼层、当前楼层、
	E_ID_SETKEY_FLRMAP,
	E_ID_SETKEY_BTNMAP,
	E_ID_SETKEY_INDICATORMAP,	//目的层中的方向映射
	//--E_ID_SETKEY_DEVICE---
	E_ID_SETPAGE_VOLUME_ADD = 0x1110,
	E_ID_SETPAGE_VOLUME_DEL,
	//-----
	E_ID_SETPAGE_LUMINARY_ADD,
	E_ID_SETPAGE_LUMINARY_DEL,
	//-----
	E_ID_SETPAGE_STANDBYTIME_ADD,
	E_ID_SETPAGE_STANDBYTIME_DEL,
	//-----
	E_ID_SETPAGE_STANDBYLUMINARY_ADD,
	E_ID_SETPAGE_STANDBYLUMINARY_DEL,
	//--E_ID_SETKEY_THEME---
	E_ID_SETPAGE_DISPLAYTYPE_ADD,
	E_ID_SETPAGE_DISPLAYTYPE_DEL,
	//-----
	E_ID_SETPAGE_DISPLAYROTATION_ADD,
	E_ID_SETPAGE_DISPLAYROTATION_DEL,
	//-----
	E_ID_SETPAGE_ARROWSCROLLSPEED_ADD,
	E_ID_SETPAGE_ARROWSCROLLSPEED_DEL,
	//-----
	E_ID_SETPAGE_TIMEFORMAT_ADD,
	E_ID_SETPAGE_TIMEFORMAT_DEL,
	//--E_ID_SETKEY_ELEVATOR---
	E_ID_SETPAGE_DOORADDR_ADD,
	E_ID_SETPAGE_DOORADDR_DEL,
	//-----
	E_ID_SETPAGE_CURFLOOR_ADD,
	E_ID_SETPAGE_CURFLOOR_DEL,
	//-----
	E_ID_SETPAGE_MINFLOOR_ADD,
	E_ID_SETPAGE_MINFLOOR_DEL,
	//-----
	E_ID_SETPAGE_MAXFLOOR_ADD,
	E_ID_SETPAGE_MAXFLOOR_DEL,
	//-----
	E_ID_SETPAGE_INTERFACETYPE_ADD,
	E_ID_SETPAGE_INTERFACETYPE_DEL,
	//-----
	E_ID_SETPAGE_INTERFACEBAUD_ADD,
	E_ID_SETPAGE_INTERFACEBAUD_DEL,
	//-----
	E_ID_SETPAGE_INTERFACEPARITY_ADD,
	E_ID_SETPAGE_INTERFACEPARITY_DEL,
	//-----------------------------
	E_ID_SETPAGE_VOLUME_VALUE = 0x1150,
	E_ID_SETPAGE_LUMINARY_VALUE=0x1156,
	E_ID_SETPAGE_STANDBYTIME_VALUE=0x115c,
	E_ID_SETPAGE_STANDBYLUMINARY_VALUE=0x1162,
	E_ID_SETPAGE_DISPLAYTYPE_VALUE=0x1168,
	E_ID_SETPAGE_DISPLAYROTATION_VALUE=0x116d,
	E_ID_SETPAGE_ARROWSCROLLSPEED_VALUE=0x1173,
	E_ID_SETPAGE_TIMEFORMAT_VALUE=0x1179,
	E_ID_SETPAGE_DOORADDR_VALUE=0x117f,
	E_ID_SETPAGE_CURFLOOR_VALUE=0x1185,
	E_ID_SETPAGE_MINFLOOR_VALUE=0x118d,
	E_ID_SETPAGE_MAXFLOOR_VALUE=0x1191,
	E_ID_SETPAGE_INTERFACETYPE_VALUE=1197,
	E_ID_SETPAGE_INTERFACEBAUD_VALUE=0x119d,
	E_ID_SETPAGE_INTERFACEPARITY_VALUE=0x11a3,
	
	
} T_UI_NODE_ID;

//=======================================================================
// 特效,轮询播放图片
//=======================================================================

enum{
	E_ES_STOPPED = 0,
	E_ES_RUNNING,	
	E_ES_TWINKLE
};
enum{
	Standby_Mode=0,
	Twinkle_Mode,
	Flow_Mode,
	None_Mode
};
enum{
	UP_DIR=0,
	DN_DIR,
	UD_DIR,
	NONE
};




typedef struct _UI_EFFECT{
	unsigned char	      mState;				//特效状态
	unsigned short			mUiID;
	unsigned char				mPicStart;
	unsigned char				mPicEnd;
	unsigned short		  mInterval;		//切换周期ms
	unsigned char				mPicIndex;
	uint32_t						mCurTick;
	struct _UI_EFFECT	 *mNext;
} T_UI_EFFECT;

T_UI_EFFECT *GetEffectFromList(unsigned int pUiID);
T_UI_EFFECT *AddEffectToList(T_UI_EFFECT *pEffect);  //返回值为实际特效指针，如果pEffect和返回值不同，表示列表中已经存在此特效了

//=======================================================================
// 指令帧拆解
//=======================================================================

typedef struct {	
	unsigned char  mADR;
	unsigned char  mLEN;
	unsigned char  mDATA[D_VALUELENGTH-2]; //以字节（1byte）写入
} T_READ_CRTLREG_ACK;   //0X81

typedef struct {	 
	unsigned char  mADR_H;	 
	unsigned char  mADR_L;
	unsigned char  mDATA[D_VALUELENGTH-2];  //以字（2byte）写入
} T_WRITE_DATAREG;   //0X82

typedef struct {	
	unsigned char  mADR_H;	 
	unsigned char  mADR_L;
	unsigned char  mLEN;
} T_READ_DATAREG;   //0X83

typedef struct {	
	unsigned char  mADR_H;	 
	unsigned char  mADR_L;
	unsigned char  mLEN;
	unsigned char  mDATA[D_VALUELENGTH-3];  //以字（2byte）写入
} T_READ_DATAREG_ACK;   //0X83
/////////////////////////////////////////////////////////////////////////////////

#define D_FRAMEBUF_LEN     15

typedef struct {
	unsigned char 	mIn;//当前输入
	unsigned char 	mOut;//当前输出
	unsigned char   mBusy;
	unsigned char   mReserve;	
	T_FRAME  				mFrames[D_FRAMEBUF_LEN];
} T_FRAME_BUFFER;

extern void LoopForFrameFromMPU(void);
extern void RecvDataFromMPU(unsigned char pData);

extern void ParseFrameFromMPU(T_FRAME *pFrame);
extern void SendFrameToMPU(unsigned char pCmd, unsigned char *pValue, unsigned char pLen);
extern void LoopForFrameToMPU(void);

void LoopForUILogicOfMPU(void);	//用于界面逻辑，比如箭头滚动，按钮保持亮

enum{
	E_BTN_RELEASE = 0,
	E_BTN_PRESS,
	E_BTN_LIGHT
};

#define D_DOUBLECLICK_INTERVAL  300 //ms
typedef struct{
	unsigned char				mPressState; 	//0：未按下，1：按下，2：点亮
	unsigned char				mPressCount;  //连续点击按钮的次数，只有在mPressState在E_BTN_PRESS状态下才有效
	unsigned short      mPressDelay;  //ms
	uint32_t						mLastPressTick;
} T_BUTTON;

/******MCU<->MPU 通信 缓冲区******************************************************************************/

typedef void (* Func_Callback) (void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue);

typedef struct{
	Func_Callback			 mCallbackFunc;   		//当新设置的值与老的值有变化时自动调用此函数
	unsigned char     *mValue;
	unsigned char 		 mLength; 						//Value的长度
	unsigned char  		 mNeedSendToMPU;   		//是否需要立即发送给MPU，只要不为0，则持续发送，每次自减一
	unsigned char  		 mFilterEnable;				//是否做相同数据过滤
	unsigned char  		 mReserve;
}T_MPU_DATA;

#define D_NEEDSEND_MAX	3			//对于有变化的参数值最大需要发送的次数

typedef struct {  //如果需要调整显示的优先级，直接调整以下的顺序即可
	unsigned char  mFault : 1;						//急停
	unsigned char  mIns: 1;						//检修
	unsigned char  mFullLoad : 1;  					//超载
	unsigned char  mFireMode : 1;						//满载
	unsigned char  mLockMode : 1;					//消防
	unsigned char  mSiJi : 1;							//司机
	unsigned char  mZhuangYong : 1;				//专用
	unsigned char  mGuZhang : 1; 					//故障
	unsigned char  mDiZhen : 1;						//地震
	unsigned char  mTingZhiFuWu : 1;				//停止服务
	unsigned char  mSuoTi : 1;							//锁梯
	unsigned char  mJingDaoZiXueXi : 1;		//井道自学习
	unsigned char  mWeiDongPingCeng : 1;		//微动平层
	unsigned char  mXiaoFangFanJiZhan : 1; //消防返基站
	unsigned char  mBoTi : 1;							//泊梯
	unsigned char  mFanPingCeng : 1;				//返平层
	unsigned char  mYingJiYunXing : 1;			//应急运行
	unsigned char  mDianJiZiXueXi : 1;			//电机自学习
	unsigned char  mJianPanTiaoCe : 1;			//键盘调测
	unsigned char  mJiZhanJianCe : 1;			//基站检测
	unsigned char  mNeiHuYouXian : 1;			//内呼优先
	unsigned char  mZhiShi : 1;						//直驶
	unsigned char  mQingZai : 1;						//轻载
	unsigned char  mJieNeng : 1;					  //节能
	unsigned char  mVIP : 1;
	unsigned char  mGuanZhi : 1;						//管制
	unsigned char  mKaiMenQu : 1;					//开门区
	unsigned char  mBaoZhanZhong : 1;			//报站钟
	unsigned char  mYunXingDaoZhan : 1;		//运行到站
}T_DISP_FUNCTION;

//用于控制柜解析协议将解析出的内容放到MPU发送缓冲区
typedef enum {
	E_MPU_DATA_LUMINARY = 0,
	E_MPU_DATA_VOLUME,
	E_MPU_DATA_FLOOR,
	E_MPU_DATA_ARROW,
	E_MPU_DATA_FUNCTION,
	E_MPU_DATA_FLOOR_2,					//并联梯第二梯的楼层区域
	E_MPU_DATA_ARROW_2,
	E_MPU_DATA_FUNCTION_2,
	E_MPU_DATA_VIDEO,
	E_MPU_DATA_IMAGE,
	E_MPU_DATA_AUDIO,
	E_MPU_DATA_TEXT,
	E_MPU_DATA_PERSON, 					//人数
	E_MPU_DATA_LOADCAPACITY,	 	//载重
	E_MPU_DATA_LOGO,						//客户logo
	E_MPU_DATA_DATETIME,				//日期时间
	E_MPU_DATA_BACKGROUND, 			//背光
	E_MPU_DATA_WEATHER,					//天气
	E_MPU_DATA_TEMPERATURE,			//温度				
	E_MPU_DATA_FLRPAD,					//楼层按钮键盘
	E_MPU_DATA_LOPPAD,					//外呼键盘(up、dn、消防、锁梯)
	E_MPU_DATA_COPPAD,					//内呼键盘（open、close、alarm）
	
} T_MPU_DATA_TYPE;   //【总个数 = 此行号 - 第一个参数的行号】

#define D_MPU_DATA_TYPE_NUM   24

extern void WriteMPUData(T_CONTROLLER_PORT_TYPE pPortType, T_MPU_DATA_TYPE pType, unsigned char *pValue);
extern T_MPU_DATA *ReadMPUData(T_MPU_DATA_TYPE pType);
/******MCU<->控制柜 通信 缓冲区******************************************************************************/

typedef struct{
	unsigned int  mFrameInterval;  //ms,如果为0表示不进行时间断帧判断
	uint32_t			mDataRecvTime;
	unsigned char mHeader[7];
	unsigned char mHeaderLen;
	unsigned char mFooter[7];//铭牌
	unsigned char mFooterLen;

	unsigned char mRawBuffer[64];
	unsigned char mRawBufferIndex;
} T_RAWBUF_FROM_CONTROLLER;

extern void SetFrameIntervalOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned int pTime);  //ms
extern void SetFrameHeaderOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pValue, unsigned char pLen);
extern void SetFrameFooterOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pValue, unsigned char pLen);

extern void RecvRawDataFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char pData);   //获取到了一个原始字节
extern void RecvRawFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen);  //获取到了一个原始帧

extern void LoopForFrameFromController(void);

extern void ParseFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen);
extern void SendFrameToController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen);

extern void CallElevatorToController(T_UI_NODE_ID pButtonID);

#endif //_LOGIC_H_ 
