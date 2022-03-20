#include "Usart2.h"
#include "stm32f10x.h"
#include "string.h"
#include "LobotSerialServo.h"

u8 UART_RX_BUF[16];

bool isUartRxCompleted = false;

void Uart2_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
  //复位串口2
	USART_DeInit(USART2);  
	
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3 初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3 

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2 
}

void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	while (len--) {
		while ((USART2->SR & 0x40) == 0);
		USART_SendData(USART2,*buf++);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { //判断接收中断
		Res = USART_ReceiveData(USART2);//(USART2->DR);	//取出接收寄存器数据
		if (!isGotFrameHeader) {  //判断帧头
			if (Res == 0x55) {
				frameHeaderCount++;
				if (frameHeaderCount == 2) {
					frameHeaderCount = 0;
					isGotFrameHeader = true;
					dataCount = 1;
				}
			} else {
				isGotFrameHeader = false;
				dataCount = 0;
				frameHeaderCount = 0;
			}
		}
		if (isGotFrameHeader) { //接收接收数据部分
			UART_RX_BUF[dataCount] = Res;
			if (dataCount == 3) {
				dataLength = UART_RX_BUF[dataCount];
				if (dataLength < 3 || dataLength > 7) {
					dataLength = 3;
					isGotFrameHeader = false;
				}
			}
			dataCount++;
			if (dataCount == dataLength + 3) {
				if (isUartRxCompleted == false) {
					isUartRxCompleted = true;
					memcpy(LobotRxBuf, UART_RX_BUF, dataCount+2);	
				}
				isGotFrameHeader = false;
			}
		}
	}
}

bool isRxCompleted(void)
{
	if(isUartRxCompleted == true){
		isUartRxCompleted = false;
		return true;
	}else{
		return false;
	}
}

