#ifndef   _US3_H_
#define   _US3_H_

#include "sys.h"

#define  Trig  PAout(0)  //PC8 �����źſ�������
#define  Echo  PAin(1)   //PC9 �����ź����

float US3_check(void);
void TIM5_time_init(u16 arr,u16 psc );
void US3_IO_Init(void);

#endif 

