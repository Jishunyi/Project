#ifndef _USER_H_
#define _USER_H_

#include "logic.h"
typedef unsigned short Uint16;
extern unsigned char gVersion[32];
extern unsigned int CountnumFlag;
void TimerInit(void);
void ConrtollerPortInit(void);

#define D_FILTER_RAW_DATA_FROM_CONTROLLER  0 		//�Ƿ�Ҫ���й���ԭʼ�ֽ����ݣ����ϣ���Կ��ƹ�����ݽ������⴦������Ϊ1
#define D_FILTER_RAW_FRAME_FROM_CONTROLLER  0		//�Ƿ�Ҫ���й���ԭʼ֡����
enum{
    VAL_0,
    VAL_1,
    VAL_2,
    VAL_3,
    VAL_4,
    VAL_5,
    VAL_6,
    VAL_7,
    VAL_8,
    VAL_9,
  	VAL_A,VAL_B,VAL_C,VAL_D,
  	VAL_E,VAL_F,VAL_G,VAL_H,
  	VAL_I,VAL_J,VAL_K,VAL_L,
  	VAL_M,VAL_N,VAL_O,VAL_P,
  	VAL_Q,VAL_R,VAL_S,VAL_T,
  	VAL_U,VAL_V,VAL_W,VAL_X,
  	VAL_Y,VAL_Z,
  	VAL_MINUS,
  	VAL_ADD,
    VAL_NULL,
    VAL_EXTPIC_0,//����
    VAL_EXTPIC_1,//����
    VAL_EXTPIC_2,//����
    VAL_EXTPIC_3,//����
    VAL_EXTPIC_4,//����
    VAL_EXTPIC_5,//ר��
    VAL_EXTPIC_6,
    VAL_EXTPIC_7,
    VAL_IDX_NUM,
};


#endif //_LOGIC_H_ 
