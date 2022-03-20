#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"

#include "stm32f10x.h"
#include "string.h"
#include "LobotSerialServo.h"
#include "bool.h"

void Uart2_init(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);
bool isRxCompleted(void);
#endif
