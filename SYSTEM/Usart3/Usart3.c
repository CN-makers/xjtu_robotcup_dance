#include "Usart3.h"

extern u8 control1;
extern u8 control2;

#if EN_USART3_RX   //如果使能了接收
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  
  
void Uart3_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能 GPIOB 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//使能 USART3 时钟
	
	USART_DeInit(USART3);  //复位串口3
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB.11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  

  //USART3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
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

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 

}

void Uart3_Printf(const char *buf)
{
	u8 len = strlen(buf);
	while (len--) {
		while ((USART3->SR & 0x40) == 0);
		USART_SendData(USART3,*buf++);
	}
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
		u8 Res;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
			{
				Res = USART_ReceiveData(USART3);	//读取接收到的数据
			}
			if(Res == 'A')
			{
				control1=0;
				control2=0;
				Uart3_Printf("\r\nStart 1 !\r\n");
			}
			else if(Res == 'B')
			{
				control1=0;
				control2=1;
				Uart3_Printf("\r\nStart 2 !\r\n");
			}
			else if(Res == 'C')
			{
				control1=1;
				control2=1;
				Uart3_Printf("\r\nStart 3 !\r\n");
			}
			else if(Res == 'D')
			{
				control1=1;
				control2=0;
				Uart3_Printf("\r\nStart 4 !\r\n");
			}
			else 
			{
				Uart3_Printf("\r\nError message !\r\n");
				Uart3_Printf("\r\nThe message you sent is : ");
				USART3->DR = Res;
				Uart3_Printf("\r\n");
			}

//		
//		if((USART3_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART3_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
//				else USART3_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART3_RX_STA|=0x4000;
//				else
//					{
//					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
} 
#endif	
