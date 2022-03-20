#include "Ultrasonic.h"
#include "delay.h"
#include "exti.h"
#include "Action.h"


u8 sr04_over_flag1;

float dis;




/*
需要将超声波的两个端口
STM32对应的端口 
Trig    触发信号控制输入-----PB0配置成推挽输出  B0
Echo    回响信号输出---------PB1配置成上拉输入  B1
*/
void Ultrasonic_IO_Init(void)
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
			
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC端口时钟
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				  //Trig-->PB.1 端口配置
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; ; 		 //上拉输入
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
		 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.8
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		 //Echo-->PB.0 端口配置, 推挽输出
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
		 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
		 GPIO_ResetBits(GPIOB,GPIO_Pin_1); 						 //PB.0 输出低
}
/*
通用定时器4初始化
这里时钟选择为APB1的2倍，而APB1为36MHz
arr：自动重装值。
psc：时钟预分频数

*/
void TIM3_time_init(u16 arr,u16 psc )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	
	
		
	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	
	

}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		//LED1=!LED1;
		sr04_over_flag1 = 1;	
		}
}

/*
        启动模块
*/
void  Start_hc_sr04()
{
    Trig=1;
	  delay_us(15);
    Trig=0;
}

/*	
      换算
			时钟1ms计数100次,10微秒计数一次，触发启动时，有15毫秒的延迟，误差大约在3~4mm左右，误差可以接受
			测试距离=(高电平时间*声速(340M/S))/2;
			//计数700下，每计数1次的时间T=0.01ms=10us；
			time为TIM3的计数值；
      高电平时间T1=time*T=10*time;(单位us)
     测试距离Distance：Distance=（T1us*34000cm/10^6us）/2 =(time*34)/20=1.7*time;(单位mm)
*/
float distance_Count(void) 	
{    
    u16 time=0;
	  float  Distance;
    Start_hc_sr04();
	  time=TIM_GetCounter(TIM3);
		TIM_SetCounter(TIM3,0);//计数器值清零
 	  Distance=(time*1.7);
	  return Distance;
}

/*
   超声波距离测算
*/
float ultrasound_check(void)
{
		 float f = 0.0 ;
		 sr04_over_flag1 = 0 ;
		 Start_hc_sr04() ;
		 while(!Echo);  //当回响信号为零时等待
				 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能能指定的TIM中断
				 TIM_Cmd(TIM3,ENABLE);//使能TIM3外设
			while(Echo);
				 TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);//失能指定的TIM中断
				 TIM_Cmd(TIM3,DISABLE);//失能TIM3外设	  
	
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


