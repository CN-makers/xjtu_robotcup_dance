#include "US3.h"
#include "delay.h"
#include "exti.h"
#include "Action.h"


u8 sr04_over_flag3;

float dis3;




/*
��Ҫ���������������˿�
STM32��Ӧ�Ķ˿� 
Trig    �����źſ�������-----PB0���ó��������  B0
Echo    �����ź����---------PB1���ó���������  B1
*/
void US3_IO_Init(void)
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
			
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				  //Trig-->PA.1 �˿�����
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; ; 		 //��������
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
		 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.8
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //Echo-->PA.0 �˿�����, �������
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
		 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
		 GPIO_ResetBits(GPIOA,GPIO_Pin_1); 						 //PB.0 �����
}
/*
ͨ�ö�ʱ��5��ʼ��
����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36MHz
arr���Զ���װֵ��
psc��ʱ��Ԥ��Ƶ��

*/
void TIM5_time_init(u16 arr,u16 psc )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	
	
		
	  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	
	

}

void TIM5_IRQHandler(void)   //TIM5�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		//LED1=!LED1;
		sr04_over_flag3= 1;	
		}
}

/*
        ����ģ��
*/
void  Start_hc_sr04_3()
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
			timeΪTIM5�ļ���ֵ��
      �ߵ�ƽʱ��T1=time*T=10*time;(��λus)
     ���Ծ���Distance��Distance=��T1us*34000cm/10^6us��/2 =(time*34)/20=1.7*time;(��λmm)
*/
float distance_Count_3(void) 	
{    
    u16 time=0;
	  float  Distance;
    Start_hc_sr04_3();
	  time=TIM_GetCounter(TIM5);
		TIM_SetCounter(TIM5,0);//������ֵ����
 	  Distance=(time*1.7);
	  return Distance;
}

/*
   �������������
*/
float US3_check(void)
{
		 float f = 0.0 ;
		 sr04_over_flag3 = 0 ;
		 Start_hc_sr04_3() ;
		 while(!Echo);  //�������ź�Ϊ��ʱ�ȴ�
				 TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);//ʹ����ָ����TIM�ж�
				 TIM_Cmd(TIM5,ENABLE);//ʹ��TIM5����
			while(Echo);
				 TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);//ʧ��ָ����TIM�ж�
				 TIM_Cmd(TIM5,DISABLE);//ʧ��TIM5����	  
	
	   if(sr04_over_flag3==1)
			 {
				 f = 400 ;
			 }
			else    
			 {
				 f = distance_Count_3();
			 }
			 return f ;
}


