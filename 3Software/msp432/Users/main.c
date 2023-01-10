#include "myconfig.h"


void Menudisplay();

int main(void)
{
	SysInit();
//	MPU_Init();//³õʼ»¯MPU6050
//	while(mpu_dmp_init()){;}//DMP³õʼ»¯
	KEY_Init();
	LED_Init();
	OLED_Init();
	Reflectance_Init2();
	TimA0_Int_Init(9999,48);//48分频，分频后1M，计数10000，
	Servo_Init();
	Motor_Init();
	Motor_Init_bianmaqi();
	MPU_Init();//³õʼ»¯MPU6050
//	while(mpu_dmp_init()){;}//DMP³õʼ»¯
	//MPU_IIC_Init();

	for(;;)
	{
		Reflectance_Data = Reflectance_Read2();
		Menudisplay();
	}
	return 0;
}

void Menudisplay()
{
	static uint8_t State=Main_State,New_State=Main_State;
	if(State!=New_State)
		{
			OLED_Clear();
			State = New_State;
		}
		switch(State)
		{
			case Main_State:
			{
				OLED_ShowString(0,0,"Main_Menu",16);
				OLED_ShowString(0,2,"1.Function1",16);
				OLED_ShowString(0,4,"2.Function2",16);
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					New_State = State1;
				}else if(Keys[0].Double_Flag == 1)
				{
					Keys[0].Double_Flag = 0;
					New_State = State2;
				}else if(Keys[0].Long_Press_Flag == 1)
				{
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
			case State1:
			{
				OLED_ShowString(0,0,"Function1",16);
				//OLED_ShowBin(1,4,Reflectance_Data,8,16);

				OLED_ShowBNum(0,4,Encoder.Speed[2],3,16);
				OLED_ShowBNum(0,6,Encoder.Speed[3],3,16);
				OLED_ShowBNum(64,4,Encoder.Distance[2],3,16);
				OLED_ShowBNum(64,6,Encoder.Distance[3],3,16);
//				OLED_ShowNum(96,4,(uint8_t)Encoder.Speed[2],3,16);
//				OLED_ShowNum(96,6,(uint8_t)Encoder.Speed[3],3,16);
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					New_State = State1;
				}else if(Keys[0].Double_Flag == 1)
				{
					Keys[0].Double_Flag = 0;
					New_State = State2;
				}else if(Keys[0].Long_Press_Flag == 1)
				{
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
			case State2:
			{
				short x,y,z;
				MPU_Get_Gyroscope(&x,&y,&z);
				OLED_ShowString(64,0,"Function2",16);
				OLED_ShowBNum(0,2,x,6,16);
				OLED_ShowBNum(0,4,y,6,16);
				OLED_ShowBNum(0,6,z,6,16);
				if(Keys[0].Single_Flag == 1)
				{
					Keys[0].Single_Flag = 0;
					New_State = State1;
				}else if(Keys[0].Double_Flag == 1)
				{
					Keys[0].Double_Flag = 0;
					New_State = State2;
				}else if(Keys[0].Long_Press_Flag == 1)
				{
					Keys[0].Long_Press_Flag = 0;
					New_State = Main_State;
				}
			}break;
		}
}
