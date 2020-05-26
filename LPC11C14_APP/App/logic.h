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
	E_CONTROLLER_PORT_UART=0,  //����RS485��RS232,��������ʹ��
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

/**************************  �ϵ籣������  *********************************************************/

#define D_PARAMETRES_LEN 1024

typedef enum{
	E_DEV_DISPLAY = 0,			//��ʾ����
	E_DEV_KEYPAD,				//���ּ��̽���
	E_DEV_FLRPAD,				//¥�����
	E_DEV_LOPPAD=0,				//���
	E_DEV_DCS,					//Ŀ�Ĳ�ѡ����
} T_DEV_TYPE;

typedef struct{
	unsigned char  mEnable : 1;						//ָʾ��ǰ¥���Ƿ���Ա�����
	unsigned char  mSecurity : 1;					//ָʾ��ǰ¥���Ƿ�Ϊ����¥��
	unsigned char  mAlignment : 2;				//ָʾ��ǰ¥�����ֶ��䷽ʽ��0�����У�1������룬2:�Ҷ���
	unsigned char  mReserve : 4;
} T_FLR_FLAG;

typedef struct{
	T_FLR_FLAG		mFlag;
	unsigned char mMap[3]; //����¥���Ӧ����λӳ����ĸ
	char					mPasswd[4];
}T_FLR_MAP;

typedef struct{
	unsigned char mNum;
	unsigned char mMap; 		//��������Ӧ��ӳ��
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
#define D_MAX_BTN  4		//֧��ӳ��İ�ť��Ҫ��Keypad�е�ABCD�������ַ�

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
	unsigned char			mRotate;	//��Ļ��ת�Ƕȣ�0~360
	
	unsigned char 		mArrowScrollSpeed;		//ms	ע��ֻ��ʹ��char���ͣ���Ҫʹ��short����
	unsigned char			mTimeFormat;
	
	//Elevator
	unsigned char 	  mDoorAddr;		//��ʾ�����ڷ�λ��ǰ�š����š����š�����
	unsigned char 	  mFlr;					//�豸��װ��¥��

	unsigned char			mMinFlr;		//��С¥�㣬��1��ʼ
	unsigned char			mMaxFlr;		//���¥��
	
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

//Ĭ��ҳ��Ϊ��棬�����Ҫ�л�Ϊ����ҳ�棬ֱ���ڵ�ǰpage�ϼ�D_PAGE_NUM
#define  D_PAGE_NUM   20

typedef enum {
	E_PAGE_SPLASH = 0,
	E_PAGE_DISPLAY=0,			//��ʾ����,��T_DEV_TYPE��ͬ
	E_PAGE_KEYPAD,		//���ּ��̽���
	E_PAGE_FLRPAD,		//¥�����
	E_PAGE_LOPPAD,		//���
	E_PAGE_DCS,				//Ŀ�Ĳ�ѡ����
	//=========================================
	E_PAGE_PASSWD,	  //������̽���
	E_PAGE_MENU_DEVICE=3,		//�������ȡ�����������
	E_PAGE_MENU_THEME,		//��������ѡ����Ļ��ת/��ͷ�����ٶȡ�ʱ���ʽ����Ӣ�ģ���
	E_PAGE_MENU_ELEVATOR,	//������͡����¥�㡢��ǰ¥�㡢
	E_PAGE_MENU_FLRMAP,
	E_PAGE_MENU_BTNMAP,
	E_PAGE_MENU_INDICATORMAP,	//Ŀ�Ĳ��еķ���ӳ��
} T_PAGE_ID;

typedef enum {
	E_PAGE_DISPLAY0 = 0,
	E_PAGE_DISPLAY1=1,			//��ʾ����,��T_DEV_TYPE��ͬ
	E_PAGE_DISPLAY2,		//���ּ��̽���
	E_PAGE_SETADDR=6,		//¥�����
	E_PAGE_LOPPAD1,		//���
	E_PAGE_DCS1,				//Ŀ�Ĳ�ѡ����
	//=========================================
	E_PAGE_PASSWD1,	  //������̽���
	E_PAGE_MENU_DEVICE1,		//�������ȡ�����������
	E_PAGE_MENU_THEME1,		//��������ѡ����Ļ��ת/��ͷ�����ٶȡ�ʱ���ʽ����Ӣ�ģ���
	E_PAGE_MENU_ELEVATOR1,	//������͡����¥�㡢��ǰ¥�㡢
	E_PAGE_MENU_FLRMAP1,
	E_PAGE_MENU_BTNMAP1,
	E_PAGE_MENU_INDICATORMAP1,	//Ŀ�Ĳ��еķ���ӳ��
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
	E_INITIALIZE = 0,  //����״̬
	E_SYNC,						 //ͬ����Ϣ״̬
	E_DISCONNECTED,		 //δ����
	E_CONNECTED				 //�����ϣ�ֻҪ�ڳ�ʱʱ���ڳ������յ�MPU���������ݣ���Ϊ����״̬
} T_CONNSTATE;

void SetConnState(T_CONNSTATE pState);

typedef struct {
	T_CONNSTATE  mState;
	unsigned int mTimeout;  //ms,���ӳ�ʱ����,���Ϊ0��ʾ�����г�ʱ�ж�
	uint32_t     mDstTick;
} T_MPU;

extern void PoweronMPU(void);  					//��MPU�ϵ�
extern void RestartMPU(void); 					//����MPU
extern bool IsConnectedWithMPU(void);		//�ж��Ƿ��MPUͨ��������
extern void ReceivedFromMPU(void);			//��¼��MPU��ȡ��һ����Ч֡
extern void LoopForMPU(void);						//����while(1)ѭ���м���MPUͨ���Ƿ��쳣

/******MCU <-> MPU ͨ��******************************************************************************/

#define D_HEADER_H ((unsigned char)0x5A)
#define D_HEADER_L ((unsigned char)0xA5)
	
#define D_VALUELENGTH 64
	
typedef enum{
	E_HEADER_H ,
	E_HEADER_L,
	E_FRAME_LEN,
	E_FRAME_CMD,
	E_FRAME_DATA
} T_FRAME_SM;  			//mpu<->mcu��Э��֡״̬��

typedef struct {
	unsigned char  mLength;
	unsigned char  mCmd;
	unsigned char  mValue[D_VALUELENGTH];		  //����
	unsigned char  mReserve[2];
} T_FRAME;

/////////////////////////////////////////////////////////////////////////////////
typedef struct {	 
	unsigned char  mADR;
	unsigned char  mDATA[D_VALUELENGTH-1]; //���ֽڣ�1byte��д��
} T_WRITE_CRTLREG;   //0X80

typedef struct {	
	unsigned char  mADR;
	unsigned char  mLEN;
} T_READ_CRTLREG;   //0X81

#define D_UI_TOUCH_OFFSET  0x1000
#define ID_TOUCH(x)  	(x+D_UI_TOUCH_OFFSET) //��ui�ؼ��������Ӧ�Ĵ��ؿؼ�ID
#define ID_UINODE(x)  (x-D_UI_TOUCH_OFFSET) //�Ӵ��ؿؼ��������Ӧ��ui�ؼ�ID

//=====================================================================================================
//ui�ؼ���ID�ŷ���,�������Ӧ�Ĵ��ؿؼ�IDΪ[UI ID]+D_UI_TOUCH_OFFSET��
//  �������Ĵ���ID�ĸ����б仯����Ҫͬ���޸�gLopPadButton��gCopPadButton��
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
	//----------------------------- ���ּ��̣�����������ʵ����̰��¼����KEYPAD��Ȼ����KEYPAD���������Ļ������ݸ�INPUTECHO
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
	E_ID_PASSWDPAD_0 = 0x10C0,   //�������
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
	E_ID_SETKEY_ENTER = 0x1100,   //���ð�ť
	E_ID_SETKEY_BACK,				//���÷��ذ�ť
	E_ID_SETKEY_DEVICE = 0x1102,		//�������ȡ�����������
	E_ID_SETKEY_THEME,		//��������ѡ�񡢼�ͷ�����ٶȡ�ʱ���ʽ����Ӣ�ģ���
	E_ID_SETKEY_ELEVATOR,	//������͡����¥�㡢��ǰ¥�㡢
	E_ID_SETKEY_FLRMAP,
	E_ID_SETKEY_BTNMAP,
	E_ID_SETKEY_INDICATORMAP,	//Ŀ�Ĳ��еķ���ӳ��
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
// ��Ч,��ѯ����ͼƬ
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
	unsigned char	      mState;				//��Ч״̬
	unsigned short			mUiID;
	unsigned char				mPicStart;
	unsigned char				mPicEnd;
	unsigned short		  mInterval;		//�л�����ms
	unsigned char				mPicIndex;
	uint32_t						mCurTick;
	struct _UI_EFFECT	 *mNext;
} T_UI_EFFECT;

T_UI_EFFECT *GetEffectFromList(unsigned int pUiID);
T_UI_EFFECT *AddEffectToList(T_UI_EFFECT *pEffect);  //����ֵΪʵ����Чָ�룬���pEffect�ͷ���ֵ��ͬ����ʾ�б����Ѿ����ڴ���Ч��

//=======================================================================
// ָ��֡���
//=======================================================================

typedef struct {	
	unsigned char  mADR;
	unsigned char  mLEN;
	unsigned char  mDATA[D_VALUELENGTH-2]; //���ֽڣ�1byte��д��
} T_READ_CRTLREG_ACK;   //0X81

typedef struct {	 
	unsigned char  mADR_H;	 
	unsigned char  mADR_L;
	unsigned char  mDATA[D_VALUELENGTH-2];  //���֣�2byte��д��
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
	unsigned char  mDATA[D_VALUELENGTH-3];  //���֣�2byte��д��
} T_READ_DATAREG_ACK;   //0X83
/////////////////////////////////////////////////////////////////////////////////

#define D_FRAMEBUF_LEN     15

typedef struct {
	unsigned char 	mIn;//��ǰ����
	unsigned char 	mOut;//��ǰ���
	unsigned char   mBusy;
	unsigned char   mReserve;	
	T_FRAME  				mFrames[D_FRAMEBUF_LEN];
} T_FRAME_BUFFER;

extern void LoopForFrameFromMPU(void);
extern void RecvDataFromMPU(unsigned char pData);

extern void ParseFrameFromMPU(T_FRAME *pFrame);
extern void SendFrameToMPU(unsigned char pCmd, unsigned char *pValue, unsigned char pLen);
extern void LoopForFrameToMPU(void);

void LoopForUILogicOfMPU(void);	//���ڽ����߼��������ͷ��������ť������

enum{
	E_BTN_RELEASE = 0,
	E_BTN_PRESS,
	E_BTN_LIGHT
};

#define D_DOUBLECLICK_INTERVAL  300 //ms
typedef struct{
	unsigned char				mPressState; 	//0��δ���£�1�����£�2������
	unsigned char				mPressCount;  //���������ť�Ĵ�����ֻ����mPressState��E_BTN_PRESS״̬�²���Ч
	unsigned short      mPressDelay;  //ms
	uint32_t						mLastPressTick;
} T_BUTTON;

/******MCU<->MPU ͨ�� ������******************************************************************************/

typedef void (* Func_Callback) (void * self, T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pOldValue, unsigned char *pNewValue);

typedef struct{
	Func_Callback			 mCallbackFunc;   		//�������õ�ֵ���ϵ�ֵ�б仯ʱ�Զ����ô˺���
	unsigned char     *mValue;
	unsigned char 		 mLength; 						//Value�ĳ���
	unsigned char  		 mNeedSendToMPU;   		//�Ƿ���Ҫ�������͸�MPU��ֻҪ��Ϊ0����������ͣ�ÿ���Լ�һ
	unsigned char  		 mFilterEnable;				//�Ƿ�����ͬ���ݹ���
	unsigned char  		 mReserve;
}T_MPU_DATA;

#define D_NEEDSEND_MAX	3			//�����б仯�Ĳ���ֵ�����Ҫ���͵Ĵ���

typedef struct {  //�����Ҫ������ʾ�����ȼ���ֱ�ӵ������µ�˳�򼴿�
	unsigned char  mFault : 1;						//��ͣ
	unsigned char  mIns: 1;						//����
	unsigned char  mFullLoad : 1;  					//����
	unsigned char  mFireMode : 1;						//����
	unsigned char  mLockMode : 1;					//����
	unsigned char  mSiJi : 1;							//˾��
	unsigned char  mZhuangYong : 1;				//ר��
	unsigned char  mGuZhang : 1; 					//����
	unsigned char  mDiZhen : 1;						//����
	unsigned char  mTingZhiFuWu : 1;				//ֹͣ����
	unsigned char  mSuoTi : 1;							//����
	unsigned char  mJingDaoZiXueXi : 1;		//������ѧϰ
	unsigned char  mWeiDongPingCeng : 1;		//΢��ƽ��
	unsigned char  mXiaoFangFanJiZhan : 1; //��������վ
	unsigned char  mBoTi : 1;							//����
	unsigned char  mFanPingCeng : 1;				//��ƽ��
	unsigned char  mYingJiYunXing : 1;			//Ӧ������
	unsigned char  mDianJiZiXueXi : 1;			//�����ѧϰ
	unsigned char  mJianPanTiaoCe : 1;			//���̵���
	unsigned char  mJiZhanJianCe : 1;			//��վ���
	unsigned char  mNeiHuYouXian : 1;			//�ں�����
	unsigned char  mZhiShi : 1;						//ֱʻ
	unsigned char  mQingZai : 1;						//����
	unsigned char  mJieNeng : 1;					  //����
	unsigned char  mVIP : 1;
	unsigned char  mGuanZhi : 1;						//����
	unsigned char  mKaiMenQu : 1;					//������
	unsigned char  mBaoZhanZhong : 1;			//��վ��
	unsigned char  mYunXingDaoZhan : 1;		//���е�վ
}T_DISP_FUNCTION;

//���ڿ��ƹ����Э�齫�����������ݷŵ�MPU���ͻ�����
typedef enum {
	E_MPU_DATA_LUMINARY = 0,
	E_MPU_DATA_VOLUME,
	E_MPU_DATA_FLOOR,
	E_MPU_DATA_ARROW,
	E_MPU_DATA_FUNCTION,
	E_MPU_DATA_FLOOR_2,					//�����ݵڶ��ݵ�¥������
	E_MPU_DATA_ARROW_2,
	E_MPU_DATA_FUNCTION_2,
	E_MPU_DATA_VIDEO,
	E_MPU_DATA_IMAGE,
	E_MPU_DATA_AUDIO,
	E_MPU_DATA_TEXT,
	E_MPU_DATA_PERSON, 					//����
	E_MPU_DATA_LOADCAPACITY,	 	//����
	E_MPU_DATA_LOGO,						//�ͻ�logo
	E_MPU_DATA_DATETIME,				//����ʱ��
	E_MPU_DATA_BACKGROUND, 			//����
	E_MPU_DATA_WEATHER,					//����
	E_MPU_DATA_TEMPERATURE,			//�¶�				
	E_MPU_DATA_FLRPAD,					//¥�㰴ť����
	E_MPU_DATA_LOPPAD,					//�������(up��dn������������)
	E_MPU_DATA_COPPAD,					//�ں����̣�open��close��alarm��
	
} T_MPU_DATA_TYPE;   //���ܸ��� = ���к� - ��һ���������кš�

#define D_MPU_DATA_TYPE_NUM   24

extern void WriteMPUData(T_CONTROLLER_PORT_TYPE pPortType, T_MPU_DATA_TYPE pType, unsigned char *pValue);
extern T_MPU_DATA *ReadMPUData(T_MPU_DATA_TYPE pType);
/******MCU<->���ƹ� ͨ�� ������******************************************************************************/

typedef struct{
	unsigned int  mFrameInterval;  //ms,���Ϊ0��ʾ������ʱ���֡�ж�
	uint32_t			mDataRecvTime;
	unsigned char mHeader[7];
	unsigned char mHeaderLen;
	unsigned char mFooter[7];//����
	unsigned char mFooterLen;

	unsigned char mRawBuffer[64];
	unsigned char mRawBufferIndex;
} T_RAWBUF_FROM_CONTROLLER;

extern void SetFrameIntervalOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned int pTime);  //ms
extern void SetFrameHeaderOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pValue, unsigned char pLen);
extern void SetFrameFooterOfController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pValue, unsigned char pLen);

extern void RecvRawDataFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char pData);   //��ȡ����һ��ԭʼ�ֽ�
extern void RecvRawFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen);  //��ȡ����һ��ԭʼ֡

extern void LoopForFrameFromController(void);

extern void ParseFrameFromController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen);
extern void SendFrameToController(T_CONTROLLER_PORT_TYPE pPortType, unsigned char *pFrame, unsigned char pLen);

extern void CallElevatorToController(T_UI_NODE_ID pButtonID);

#endif //_LOGIC_H_ 
