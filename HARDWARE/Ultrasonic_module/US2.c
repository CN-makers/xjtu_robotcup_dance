#include "US2.h"
#include "delay.h"

u8 sr04_over_flag2;
float dis2;



/*
需要将超声波的两个端口
STM32对应的端口
Trig2    触发信号控制输入-----PC8配置成推挽输出
Echo2    回响信号输出---------PC9配置成上拉输入
*/
void US_IO_Init(void)
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
			
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				  //Trig2-->PC.5端口配置
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; ; 		 //上拉输入
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
		 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.8
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //Echo2-->PC.4 端口配置, 推挽输出
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
		 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
		 GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PC.4 输出低
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ使能
		NVIC_Init(&NVIC_InitStructure);  //NVIC_InitStruct中指定的参数初始化外设	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断分组
	
	
}
/*
通用定时器4初始化
这里时钟选择为APB1的2倍，而APB1为36MHz
arr：自动重装值。
psc：时钟预分频数

*/
void TIM4_time_init(u16 arr,u16 psc )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	

}

void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		//LED1=!LED1;
		sr04_over_flag2 = 1;	
		}
}

/*
        启动模块
*/
//相当于是发射信号15毫秒？
void  Start_hc_sr04_2()
{
    Trig2=1;
	  delay_us(15);
    Trig2=0;
}

/*	
      换算
			测试距离=(高电平时间*声速(340M/S))/2;
			//计数700下，每计数1次的时间T=0.01ms=10us；
			time为TIM3的计数值；
      高电平时间T1=time*T=10*time;(单位us)
     测试距离Distance：Distance=（T1us*34000cm/10^6us）/2 =(time*34)/20=1.7*time;(单位mm)
*/
float distance_Count_2(void) 	
{    
    u16 time=0;
	  float  Distance;
    Start_hc_sr04_2();
	  time=TIM_GetCounter(TIM4);
		TIM_SetCounter(TIM4,0);//计数器值清零
 	  Distance=(time*1.7);
	  return Distance;
}

/*
   超声波距离测算
*/
float US_check(void)
{
		 float f = 0.0 ;
		 sr04_over_flag2 = 0 ;
		 Start_hc_sr04_2() ;
		 while(!Echo2);  //当回响信号为零时等待
				 TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断
				 TIM_Cmd(TIM4,ENABLE);//使能TIM4外设
			while(Echo2);
				 TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);//使能或者失能指定的TIM中断
				 TIM_Cmd(TIM4,DISABLE);//失能TIM4外设	  
	
	   if(sr04_over_flag2==1)
			 {
				 f = 400 ;
			 }
			else    
			 {
				 f = distance_Count_2();
			 }
			 return f ;
}


