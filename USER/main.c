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


//第二组多组机器人文档，Tim3，4给超声波
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
	 
	 
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	 
	uart_init(9600);	  //串口1（视觉）
	Uart2_init(115200); //初始化串口2（舵机）
	Uart3_init(9600);   //串口3（蓝牙）
	Uart4_init(9600);   //串口4（语音）
	
	
	EXTIX_Init();	//外部中断初始化
	KEY_Init();          	//初始化与按键连接的硬件接口
	
	US_IO_Init();
	Ultrasonic_IO_Init();//   双超声波初始化
  TIM3_time_init(6999,719);		//初始化定时器时已经确定了抢占优先级和从优先级，定时器确定距离的优先级为最高
	TIM4_time_init(6999,719);//超声波的定时器70ms中断一次  100Khz的计数频率   T= ((719+1)*(6999+1)) /72000 000 =70ms，计数到7000为70毫秒
	 
	 while(1)
	{
		  if(control1==0 && control2==0)
			{
			//开始第一部分
      shijue_begin();     //视觉开始
			
			//视觉交互模块
			printf("Start vision\r\n");
			times = 0 ;
			while(control1==0 && control2==0)
			{}				//判断视觉交互结束,机器人需要做的动作在中断函数中
				
				stand();
				control1=0;
				control2=1;
			
//			if((times>=90) && (SHIJUE0_STA == 0))
//			{
//				control2=0;
//				stand();
//			}
		}
			//语音模块
			if(control1==0 && control2==1)
			{	
				yuyin_begin();
					while(control1==0 && control2==1){}	;
				  stand();
				  control1=1;
				  control2=1;
			}
			if(control1==1 && control2==1)   //开始第二部分，跳舞
			{
					fulldanceaction();
					control1=1;
					control2=0;
				
		} 
		//若使用两个超声波传感器，则设想一个超声波传感器控制机器人左右与墙壁的距离，另一个控制前后与墙壁的距离，在踩圈时最好走直线	
		//应用一个右转弯来分别靠近四个环�
		//整个机器人踩圈依照一定的顺序，顺时针或者逆时针来走，一直通过一个方向转弯来控制踩到四个圈
		//踩圈不超过3分钟
		//踩圈部分的距离只是写了一个初始的架构，之后距离之类的东西还要根据具体情况进行修改
		
		
		if(control1==1 && control2==0)       //开始第三部分，踩圈
			{
				while(succesnumber<4)
				{	
				dis=ultrasound_check();
				dis2=US_check();
					if(dis>400 && dis2>400)
						qianjin();
					if(dis<100)
						houtui();											
					if(dis2<200)  //以顺时针踩圈为例，就是右平移,靠近墙体
						youpingyi();						
				
				if(dis<=351 && dis>=236 && dis2<=200)//具体的数字之后还会改变
				{	xuanzhuan();
				succesnumber++ ;
				}
					
				}	
			}
				
		
		

		
}
		}
	

 
