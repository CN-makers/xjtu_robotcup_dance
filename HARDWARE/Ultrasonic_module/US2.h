#ifndef   _US2_H_
#define   _US2_H_

#include "sys.h"

#define  Trig2  PCout(4)  //PC8 触发信号控制输入
#define  Echo2  PCin(5)   //PC9 回响信号输出

float US_check(void);
void TIM4_time_init(u16 arr,u16 psc );
void US_IO_Init(void);

#endif 

