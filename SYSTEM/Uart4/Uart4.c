#include "Uart4.h"
#include "Action.h"

extern u8 control1;
extern u8 control2;


void Uart4_init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	
	USART_DeInit(UART4);  //复位串口4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;

	USART_InitStructure.USART_WordLength =USART_WordLength_8b;//???8?

	USART_InitStructure.USART_StopBits =USART_StopBits_1;//???1?

	USART_InitStructure.USART_Parity = USART_Parity_No;//??????

	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;//???????

	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;//TX?RX???
	
	//Usart4 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_Init(UART4, &USART_InitStructure); //初始化串口4
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4,USART_FLAG_TC);
}


void Uart4_Printf(const char *buf)
{
	u8 len = strlen(buf);
	while (len--) {
		while ((UART4->SR & 0x40) == 0);
		USART_SendData(UART4,*buf++);
	}
}

void UART4_IRQHandler(void)                	//串口4中断服务程序
	{
		u8 Res;
		
		if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断
		{
			Res = USART_ReceiveData(UART4);	//读取接收到的数据
		}
		if(Res == '1' && control1==0 && control2==1)
		{
			yuyin1();
		}
		else if(Res == '2' && control1==0 && control2==1)
		{
			yuyin2();
		}
		else if(Res == '3' && control1==0 && control2==1)
		{
			yuyin3();
		}
		else if(Res == '4' && control1==0 && control2==1)
		{
      yuyin4();
			
		}
}
