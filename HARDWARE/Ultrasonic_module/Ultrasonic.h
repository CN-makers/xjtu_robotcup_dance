#ifndef   _Ultrasonic_H_
#define   _Ultrasonic_H_

#include "sys.h"

#define  Trig  PBout(0)  //PC8 �����źſ�������
#define  Echo  PBin(1)   //PC9 �����ź����

float ultrasound_check(void);
void TIM3_time_init(u16 arr,u16 psc );
void Ultrasonic_IO_Init(void);

#endif 

