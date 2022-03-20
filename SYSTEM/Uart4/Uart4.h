#ifndef __UART4_H
#define __UART4_H
#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
#include "led.h"
#include "Action.h"

extern u8 YUYIN_STA ;

void Uart4_init(u32 bound);
void Uart4_Printf(const char *buf);

#endif
