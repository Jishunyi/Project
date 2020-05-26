#ifndef _USER_H_
#define _USER_H_

#include "logic.h"

extern unsigned char gVersion[32];

void TimerInit(void);
void ConrtollerPortInit(void);

#define D_FILTER_RAW_DATA_FROM_CONTROLLER  0 		//是否要进行过滤原始字节数据，如果希望对控制柜的数据进行特殊处理，设置为1
#define D_FILTER_RAW_FRAME_FROM_CONTROLLER  0		//是否要进行过滤原始帧数据

#endif //_LOGIC_H_ 
