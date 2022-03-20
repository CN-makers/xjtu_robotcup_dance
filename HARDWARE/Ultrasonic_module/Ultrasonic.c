#include "Ultrasonic.h"
#include "delay.h"
#include "exti.h"
#include "Action.h"


u8 sr04_over_flag1;

float dis;




/*
��Ҫ���������������˿�
STM32��Ӧ�Ķ˿� 
Trig    �����źſ�������-----PB0���ó��������  B0
Echo    �����ź����---------PB1���ó���������  B1
*/
void Ultrasonic_IO_Init(void)
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
			
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PC�˿�ʱ��
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				  //Trig-->PB.1 �˿�����
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; ; 		 //��������
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
		 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.8
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //Echo-->PB.0 �˿�����, �������
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
		 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
		 GPIO_ResetBits(GPIOB,GPIO_Pin_1); 						 //PB.0 �����
}
/*
ͨ�ö�ʱ��4��ʼ��
����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36MHz
arr���Զ���װֵ��
psc��ʱ��Ԥ��Ƶ��

*/
void TIM3_time_init(u16 arr,u16 psc )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	
	
		
	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	
	

}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		//LED1=!LED1;
		sr04_over_flag1 = 1;	
		}
}

/*
        ����ģ��
*/
void  Start_hc_sr04()
{
    Trig=1;
	  delay_us(15);
    Trig=0;
}

/*	
      ����
			ʱ��1ms����100��,10΢�����һ�Σ���������ʱ����15������ӳ٣�����Լ��3~4mm���ң������Խ���
			���Ծ���=(�ߵ�ƽʱ��*����(340M/S))/2;
			//����700�£�ÿ����1�ε�ʱ��T=0.01ms=10us��
			timeΪTIM3�ļ���ֵ��
      �ߵ�ƽʱ��T1=time*T=10*time;(��λus)
     ���Ծ���Distance��Distance=��T1us*34000cm/10^6us��/2 =(time*34)/20=1.7*time;(��λmm)
*/
float distance_Count(void) 	
{    
    u16 time=0;
	  float  Distance;
    Start_hc_sr04();
	  time=TIM_GetCounter(TIM3);
		TIM_SetCounter(TIM3,0);//������ֵ����
 	  Distance=(time*1.7);
	  return Distance;
}

/*
   �������������
*/
float ultrasound_check(void)
{
		 float f = 0.0 ;
		 sr04_over_flag1 = 0 ;
		 Start_hc_sr04() ;
		 while(!Echo);  //�������ź�Ϊ��ʱ�ȴ�
				 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ����ָ����TIM�ж�
				 TIM_Cmd(TIM3,ENABLE);//ʹ��TIM3����
			while(Echo);
				 TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//ʧ��ָ����TIM�ж�
				 TIM_Cmd(TIM3,DISABLE);//ʧ��TIM3����	  
	
	   if(sr04_over_flag1==1)
			 {
				 f = 400 ;
			 }
			else    
			 {
				 f = distance_Count();
			 }
			 return f ;
}


