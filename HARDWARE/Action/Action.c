#include "Action.h"
#include "LobotSerialServo.h"
#include "delay.h"

#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4
#define ID5 5
#define ID6 6
#define ID7 7
#define ID8 8
#define ID9 9
#define ID10 10
#define ID11 11
#define ID12 12
#define ID13 13
#define ID14 14
#define ID15 15
#define ID16 16
#define ID17 17
#define ID18 18
#define ID19 19
#define ID20 20
#define ID21 21
#define ID22 22
#define ID23 23
#define ID24 24
#define ID25 25
#define ID26 26
#define ID27 27
#define ID28 28
#define ID29 29
#define ID30 30
#define ID31 31
#define ID32 32

//j1代表1号舵机经过时间t移动到j1位置，以此类推
//17个舵机
void shijue_begin(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}






void shijue1(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}
void shijue2(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}
void shijue3(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	
void shijue4(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	
	
void yuyin_begin(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}

	
	void yuyin1(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}
	
void yuyin2(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}
void yuyin3(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}
void yuyin4(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}

	
void stand(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	

	
void fulldanceaction(void)
{
	shijue1();
	shijue3();
	shijue2();
	
	delay_ms(300);
	
	yuyin1();
	yuyin3();
	yuyin2();
	
	delay_ms(300);
}
	
 void qianjin(void)
 {
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	

	void houtui(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	

	
	void zuopingyi(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	
	
	void youpingyi(void)
{
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	
	
		
void xuanzhuan(void)
 {
	LobotSerialServoMove(1,500,500);
	LobotSerialServoMove(2,500,500);
	LobotSerialServoMove(3,500,500);
	LobotSerialServoMove(4,500,500);
	LobotSerialServoMove(5,500,500);
	}	

