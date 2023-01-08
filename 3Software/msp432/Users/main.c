#include "myconfig.h"


void Menudisplay();

void main(void)
{
	SysInit();
	KEY_Init();
	LED_Init();
	OLED_Init();
	Reflectance_Init2();
	TimA0_Int_Init(9999,48);//48分频，分频后1M，计数10000，
	Servo_Init();
	Motor_Init();
	Motor_Init_bianmaqi();
	
	while(1)
	{
		Reflectance_Data = Reflectance_Read2();
		Menudisplay();
	}
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
				OLED_ShowBin(1,4,Reflectance_Data,8,16);
//				OLED_ShowNum(96,0,Encoder[0],2,16);
//				OLED_ShowNum(96,2,Encoder[1],2,16);
				OLED_ShowNum(96,4,Encoder[2],3,16);
				OLED_ShowNum(96,6,Encoder[3],3,16);
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
				OLED_ShowString(64,0,"Function2",16);
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
