#ifndef __ACTION_H
#define __ACTION_H

#include "stm32f10x.h"
#include "LobotSerialServo.h"
#include "delay.h"

void shijue_begin(void);
void shijue1(void);
void shijue2(void);
void shijue3(void);
void shijue4(void);

void yuyin_begin(void);
void yuyin1(void);
void yuyin2(void);
void yuyin3(void);
void yuyin4(void);

void stand(void);

void fulldanceaction(void);

void qianjin(void);
void houtui(void);
void zuopingyi(void);
void youpingyi(void);
void zuozhuanwan(void);//也就是一直通过左转弯来转向？
void xuanzhuan(void);


#endif
