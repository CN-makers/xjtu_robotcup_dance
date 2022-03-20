#include "Usart3.h"

extern u8 control1;
extern u8 control2;

#if EN_USART3_RX   //���ʹ���˽���
//����3�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	  
  
void Uart3_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ�� GPIOB ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//ʹ�� USART3 ʱ��
	
	USART_DeInit(USART3);  //��λ����3
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
  //USART3_RX	  GPIOB.11��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB.11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11  

  //USART3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 

}

void Uart3_Printf(const char *buf)
{
	u8 len = strlen(buf);
	while (len--) {
		while ((USART3->SR & 0x40) == 0);
		USART_SendData(USART3,*buf++);
	}
}

void USART3_IRQHandler(void)                	//����3�жϷ������
{
		u8 Res;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
			{
				Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
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
//		if((USART3_RX_STA&0x8000)==0)//����δ���
//			{
//			if(USART3_RX_STA&0x4000)//���յ���0x0d
//				{
//				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
//				else USART3_RX_STA|=0x8000;	//��������� 
//				}
//			else //��û�յ�0X0D
//				{	
//				if(Res==0x0d)USART3_RX_STA|=0x4000;
//				else
//					{
//					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//					}		 
//				}
//			}   		 
//     } 
} 
#endif	
