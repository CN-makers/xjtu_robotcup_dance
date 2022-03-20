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


//µÚ¶ş×é¶à×é»úÆ÷ÈËÎÄµµ£¬Tim3£¬4¸ø³¬Éù²¨
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
	 
	 
	 
	delay_init();	    	 //ÑÓÊ±º¯Êı³õÊ¼»¯	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// ÉèÖÃÖĞ¶ÏÓÅÏÈ¼¶·Ö×é2
	
	 
	uart_init(9600);	  //´®¿Ú1£¨ÊÓ¾õ£©
	Uart2_init(115200); //³õÊ¼»¯´®¿Ú2£¨¶æ»ú£©
	Uart3_init(9600);   //´®¿Ú3£¨À¶ÑÀ£©
	Uart4_init(9600);   //´®¿Ú4£¨ÓïÒô£©
	
	
	EXTIX_Init();	//Íâ²¿ÖĞ¶Ï³õÊ¼»¯
	KEY_Init();          	//³õÊ¼»¯Óë°´¼üÁ¬½ÓµÄÓ²¼ş½Ó¿Ú
	
	US_IO_Init();
	Ultrasonic_IO_Init();//   Ë«³¬Éù²¨³õÊ¼»¯
  TIM3_time_init(6999,719);		//³õÊ¼»¯¶¨Ê±Æ÷Ê±ÒÑ¾­È·¶¨ÁËÇÀÕ¼ÓÅÏÈ¼¶ºÍ´ÓÓÅÏÈ¼¶£¬¶¨Ê±Æ÷È·¶¨¾àÀëµÄÓÅÏÈ¼¶Îª×î¸ß
	TIM4_time_init(6999,719);//³¬Éù²¨µÄ¶¨Ê±Æ÷70msÖĞ¶ÏÒ»´Î  100KhzµÄ¼ÆÊıÆµÂÊ   T= ((719+1)*(6999+1)) /72000 000 =70ms£¬¼ÆÊıµ½7000Îª70ºÁÃë
	 
	 while(1)
	{
		  if(control1==0 && control2==0)
			{
			//¿ªÊ¼µÚÒ»²¿·Ö
      shijue_begin();     //ÊÓ¾õ¿ªÊ¼
			
			//ÊÓ¾õ½»»¥Ä£¿é
			printf("Start vision\r\n");
			times = 0 ;
			while(control1==0 && control2==0)
			{}				//ÅĞ¶ÏÊÓ¾õ½»»¥½áÊø,»úÆ÷ÈËĞèÒª×öµÄ¶¯×÷ÔÚÖĞ¶Ïº¯ÊıÖĞ
				
				stand();
				control1=0;
				control2=1;
			
//			if((times>=90) && (SHIJUE0_STA == 0))
//			{
//				control2=0;
//				stand();
//			}
		}
			//ÓïÒôÄ£¿é
			if(control1==0 && control2==1)
			{	
				yuyin_begin();
					while(control1==0 && control2==1){}	;
				  stand();
				  control1=1;
				  control2=1;
			}
			if(control1==1 && control2==1)   //¿ªÊ¼µÚ¶ş²¿·Ö£¬ÌøÎè
			{
					fulldanceaction();
					control1=1;
					control2=0;
				
		} 
		//ÈôÊ¹ÓÃÁ½¸ö³¬Éù²¨´«¸ĞÆ÷£¬ÔòÉèÏëÒ»¸ö³¬Éù²¨´«¸ĞÆ÷¿ØÖÆ»úÆ÷ÈË×óÓÒÓëÇ½±ÚµÄ¾àÀë£¬ÁíÒ»¸ö¿ØÖÆÇ°ºóÓëÇ½±ÚµÄ¾àÀë£¬ÔÚ²ÈÈ¦Ê±×îºÃ×ßÖ±Ïß	
		//Ó¦ÓÃÒ»¸öÓÒ×ªÍäÀ´·Ö±ğ¿¿½üËÄ¸ö»··
		//Õû¸ö»úÆ÷ÈË²ÈÈ¦ÒÀÕÕÒ»¶¨µÄË³Ğò£¬Ë³Ê±Õë»òÕßÄæÊ±ÕëÀ´×ß£¬Ò»Ö±Í¨¹ıÒ»¸ö·½Ïò×ªÍäÀ´¿ØÖÆ²Èµ½ËÄ¸öÈ¦
		//²ÈÈ¦²»³¬¹ı3·ÖÖÓ
		//²ÈÈ¦²¿·ÖµÄ¾àÀëÖ»ÊÇĞ´ÁËÒ»¸ö³õÊ¼µÄ¼Ü¹¹£¬Ö®ºó¾àÀëÖ®ÀàµÄ¶«Î÷»¹Òª¸ù¾İ¾ßÌåÇé¿ö½øĞĞĞŞ¸Ä
		
		
		if(control1==1 && control2==0)       //¿ªÊ¼µÚÈı²¿·Ö£¬²ÈÈ¦
			{
				while(succesnumber<4)
				{	
				dis=ultrasound_check();
				dis2=US_check();
					if(dis>400 && dis2>400)
						qianjin();
					if(dis<100)
						houtui();											
					if(dis2<200)  //ÒÔË³Ê±Õë²ÈÈ¦ÎªÀı£¬¾ÍÊÇÓÒÆ½ÒÆ,¿¿½üÇ½Ìå
						youpingyi();						
				
				if(dis<=351 && dis>=236 && dis2<=200)//¾ßÌåµÄÊı×ÖÖ®ºó»¹»á¸Ä±ä
				{	xuanzhuan();
				succesnumber++ ;
				}
					
				}	
			}
				
		
		

		
}
		}
	

 
