#ifndef _USER_H_
#define _USER_H_

#include "logic.h"

extern unsigned char gVersion[32];

void TimerInit(void);
void ConrtollerPortInit(void);

#define D_FILTER_RAW_DATA_FROM_CONTROLLER  0 		//�Ƿ�Ҫ���й���ԭʼ�ֽ����ݣ����ϣ���Կ��ƹ�����ݽ������⴦������Ϊ1
#define D_FILTER_RAW_FRAME_FROM_CONTROLLER  0		//�Ƿ�Ҫ���й���ԭʼ֡����

#endif //_LOGIC_H_ 
