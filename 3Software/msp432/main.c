#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "bsp_motor.h"
#include "bsp_exti.h"
#include "key.h"
#include "led.h"
#include "oled.h"
#include "Reflectance.h"
#include "timA.h"
#include "bsp_Servo.h"

typedef enum 
{
	Main_State,
	State1,
	State2,
}Menu_State;

uint8_t Key;
uint8_t Reflectance_Data;
int8_t Encoder[4];
void main(void)
{
	uint8_t State=Main_State,New_State=Main_State;
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
		if(State!=New_State)
		{
			OLED_Clear();
			Key = 0;
			State = New_State;
		}
		if(Key)
		{
			OLED_ShowNum(96,6,Key,2,16);
		}
		switch(State)
		{
			case Main_State:
			{
				OLED_ShowString(0,0,"Main_Menu",16);
				OLED_ShowString(0,2,"1.Function1",16);
				OLED_ShowString(0,4,"2.Function2",16);
				if(Key == 1)
				{
					OLED_Clear();
					New_State = State1;
				}
				if(Key == 2)
				{
					OLED_Clear();
					New_State = State2;
				}
			}break;
			case State1:
			{
				OLED_ShowString(0,0,"Function1",16);
				OLED_ShowBin(1,4,Reflectance_Data,8,16);
//				OLED_ShowNum(96,0,Encoder[0],2,16);
//				OLED_ShowNum(96,2,Encoder[1],2,16);
				OLED_ShowNum(96,4,Encoder[2],2,16);
				OLED_ShowNum(96,6,Encoder[3],2,16);
				if(Key == 1)
				{
					OLED_Clear();
					New_State = State1;
				}
				if(Key == 2)
				{
					OLED_Clear();
					New_State = Main_State;
				}
			}break;
			case State2:
			{
				OLED_ShowString(64,0,"Function2",16);
				if(Key == 1)
				{
					OLED_Clear();
					New_State = Main_State;
				}
				if(Key == 2)
				{
					OLED_Clear();
					New_State = State2;
				}
			}break;
		}
	}
}

int8_t bianmaqi[4]={0};

void motor_PORT_IRQHandler(void)
{
    uint_fast16_t status; //uint_fast16意思是至少16位，可以更多
    status =  MAP_GPIO_getEnabledInterruptStatus(motor_Three_A_PORT); //获取中断状态
    MAP_GPIO_clearInterruptFlag(motor_Three_A_PORT, status);         //清理中断标志

	switch(status)
	{
		//电机1
		case motor_One_A_PIN:
		{
			if(motor_One_B_Value ==  1) 
				bianmaqi[0]--;
			else if(motor_One_B_Value ==  0)
				bianmaqi[0]++;
		}break;
		case motor_One_B_PIN:
		{
			if(motor_One_A_Value ==  1) 
				bianmaqi[0]++;
			else if(motor_One_A_Value ==  0)
				bianmaqi[0]--;
		}break;
		//电机2
		case motor_Two_A_PIN:
		{
			if(motor_Two_B_Value ==  1) 
				bianmaqi[1]--;
			else if(motor_Two_B_Value ==  0)
				bianmaqi[1]++;
		}break;
		case motor_Two_B_PIN:
		{
			if(motor_Two_A_Value ==  1) 
				bianmaqi[1]++;
			else if(motor_Two_A_Value ==  0)
				bianmaqi[1]--;
		}break;
		//电机3
		case motor_Three_A_PIN:
		{
			if(motor_Three_B_Value ==  1) 
				bianmaqi[2]--;
			else if(motor_Three_B_Value ==  0)
				bianmaqi[2]++;
		}break;
		case motor_Three_B_PIN:
		{
			if(motor_Three_A_Value ==  1) 
				bianmaqi[2]++;
			else if(motor_Three_A_Value ==  0)
				bianmaqi[2]--;
		}break;
		//电机4
		case motor_Four_A_PIN:
		{
			if(motor_Four_B_Value ==  1) 
				bianmaqi[3]++;
			else if(motor_Four_B_Value ==  0)
				bianmaqi[3]--;
		}break;
		case motor_Four_B_PIN:
		{
			if(motor_Four_A_Value ==  1) 
				bianmaqi[3]--;
			else if(motor_Four_A_Value ==  0)
				bianmaqi[3]++;
		}break;
	}
}
uint8_t Delay10msCnt = 0;
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Delay10msCnt++;
	if(Delay10msCnt>50)
	{
		Delay10msCnt = 0;//延时500ms
		MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
	}
    /*开始填充用户代码*/
	Key = KEY_Scan(1);	
	Encoder[0] = bianmaqi[0];
	Encoder[1] = bianmaqi[1];
	Encoder[2] = bianmaqi[2];
	Encoder[3] = bianmaqi[3];
	
	bianmaqi[0] = 0;
	bianmaqi[1]	= 0;
	bianmaqi[2]	= 0;
	bianmaqi[3]	= 0;
    

    /*结束填充用户代码*/
}