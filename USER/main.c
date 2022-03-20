#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "Usart2.h"
#include "Usart3.h"
#include "Uart4.h"
#include "LobotSerialServo.h"
#include "Timer.h"
#include "Action.h"
#include "Ultrasonic.h"
#include "US2.h"


//u8 begin_project = 0;
//u8 DANCE       = 0;
//u8 Voice_start  = 0;
//u8 SHIJUE0_STA = 0;
//u8 Vision_start  = 0;
//u8 ROBOT_STA   = 0;

u8 succesnumber=0;
u8 control1=0;
u8 control2=0;

extern 	float dis;
extern  float dis2;

extern u8 times;


u8 walk_way=0;


//�ڶ������������ĵ���Tim3��4��������
//2021 
 


 int main(void)
 {	
//	begin_project = 1;
//  DANCE       = 0;	
//	//CSB_START   = 0;
//	Vision_start  = 0;
//	Voice_start  = 0;
//	ROBOT_STA   = 1;
//	walk_way=0;
	succesnumber=0;
	 control1=0;
	 control2=0;
	 
	 
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	 
	uart_init(9600);	  //����1���Ӿ���
	Uart2_init(115200); //��ʼ������2�������
	Uart3_init(9600);   //����3��������
	Uart4_init(9600);   //����4��������
	
	
	EXTIX_Init();	//�ⲿ�жϳ�ʼ��
	KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	
	US_IO_Init();
	Ultrasonic_IO_Init();//   ˫��������ʼ��
  TIM3_time_init(6999,719);		//��ʼ����ʱ��ʱ�Ѿ�ȷ������ռ���ȼ��ʹ����ȼ�����ʱ��ȷ����������ȼ�Ϊ���
	TIM4_time_init(6999,719);//�������Ķ�ʱ��70ms�ж�һ��  100Khz�ļ���Ƶ��   T= ((719+1)*(6999+1)) /72000 000 =70ms��������7000Ϊ70����
	 
	 while(1)
	{
		  if(control1==0 && control2==0)
			{
			//��ʼ��һ����
      shijue_begin();     //�Ӿ���ʼ
			
			//�Ӿ�����ģ��
			printf("Start vision\r\n");
			times = 0 ;
			while(control1==0 && control2==0)
			{}				//�ж��Ӿ���������,��������Ҫ���Ķ������жϺ�����
				
				stand();
				control1=0;
				control2=1;
			
//			if((times>=90) && (SHIJUE0_STA == 0))
//			{
//				control2=0;
//				stand();
//			}
		}
			//����ģ��
			if(control1==0 && control2==1)
			{	
				yuyin_begin();
					while(control1==0 && control2==1){}	;
				  stand();
				  control1=1;
				  control2=1;
			}
			if(control1==1 && control2==1)   //��ʼ�ڶ����֣�����
			{
					fulldanceaction();
					control1=1;
					control2=0;
				
		} 
		//��ʹ��������������������������һ�����������������ƻ�����������ǽ�ڵľ��룬��һ������ǰ����ǽ�ڵľ��룬�ڲ�Ȧʱ�����ֱ��	
		//Ӧ��һ����ת�����ֱ𿿽��ĸ����
		//���������˲�Ȧ����һ����˳��˳ʱ�������ʱ�����ߣ�һֱͨ��һ������ת�������Ʋȵ��ĸ�Ȧ
		//��Ȧ������3����
		//��Ȧ���ֵľ���ֻ��д��һ����ʼ�ļܹ���֮�����֮��Ķ�����Ҫ���ݾ�����������޸�
		
		
		if(control1==1 && control2==0)       //��ʼ�������֣���Ȧ
			{
				while(succesnumber<4)
				{	
				dis=ultrasound_check();
				dis2=US_check();
					if(dis>400 && dis2>400)
						qianjin();
					if(dis<100)
						houtui();											
					if(dis2<200)  //��˳ʱ���ȦΪ����������ƽ��,����ǽ��
						youpingyi();						
				
				if(dis<=351 && dis>=236 && dis2<=200)//���������֮�󻹻�ı�
				{	xuanzhuan();
				succesnumber++ ;
				}
					
				}	
			}
				
		
		

		
}
		}
	

 
