/*
 * @Description: 点灯
 * @Author: Liang xiaoqi
 * @Date: 2022-12-21 15:48:04
 * @LastEditTime: 2022-12-25 18:08:56
 * @LastEditors: Liang xiaoqi
 */
/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "led.h"
#include "oled.h"
#include "sysinit.h"
#include "key.h"
#include "bsp_exti.h"
#include "Reflectance.h"
#include "bsp_Servo.h"
#include "bsp_motor.h"
#include "timA.h"
/**
*  变量宏定义
*/
#define Key_Single_Mode 0 		//单次模式按下
#define Key_Double_Mode 1		//连按模式
#define Key_UnPress		0		//按键未按下
#define Key1_Press 		1		//按键按下
#define Key2_Press		2		//按键2按下
#define MOTOR_A			1
#define MOTOR_B			2
#define MOTOR_C			3
#define MOTOR_D			4
#define FORWARD			0
#define	BACK			1
/**
 *  函数声明
 */
void Thread_LED(void const* argument);
osThreadId tid_Thread_LED;
osThreadDef (Thread_LED,osPriorityNormal,1,0);	//

void Thread_Key(void const* argument);
osThreadId tid_Thread_Key;
osThreadDef (Thread_Key,osPriorityHigh,1,0);



void Thread_CarRun(void const* argument);
osThreadId tid_Thread_CarRun;
osThreadDef (Thread_CarRun,osPriorityAboveNormal,1,0);

void Thread_Menu(void const* argument);
osThreadId tid_Thread_Menu;
osThreadDef (Thread_Menu,osPriorityNormal,1,0);

/**
 *  变量
 */
uint8_t Key = Key_UnPress;

uint16_t Reflectance_Data;
extern uint16_t bianmaqi[4];
uint16_t A_Speed;
uint16_t D_Speed;
/**
 *  任务句柄
 */

 
/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	
	LED_Init();
	SysInit();
	OLED_Init();
	KEY_Init();
	Motor_Init_bianmaqi();
	Reflectance_Init2();
	Servo_Init();
	Motor_Init();

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	osThreadCreate(osThread(Thread_LED),NULL);
	osThreadCreate(osThread(Thread_Key),NULL);
//	osThreadCreate(osThread(Thread_CarRun),NULL);
	osThreadCreate(osThread(Thread_Menu),NULL);
	
  osKernelStart ();                         // start thread execution 
}

typedef enum
{
	Main_State,
	State1,
	State2,
	State3,
	State4,
}Menu_State;
void Thread_Menu(void const* argument)
{
	uint8_t State=Main_State,New_State=Main_State;
	for(;;)
	{
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
				
				if(Key == 2)
				{
					OLED_Clear();
					New_State = State2;
				}
			}break;
			case State1:
			{
				OLED_ShowString(64,0,"Function1",16);
				OLED_ShowBin(1,4,Reflectance_Data,8,16);
				if(Key == 1)
				{
					OLED_Clear();
					New_State = State3;
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
					New_State = State4;
				}
				if(Key == 2)
				{
					OLED_Clear();
					New_State = Main_State;
				}
			}break;	
			case State3:
			{
				if(Key == 1)
				{
					OLED_Clear();
					New_State = Main_State;
				}
				if(Key == 2)
				{
					OLED_Clear();
					New_State = Main_State;
				}
			}break;
			case State4:
			{
				if(Key == 1)
				{
					OLED_Clear();
					New_State = Main_State;
				}
				if(Key == 2)
				{
					OLED_Clear();
					New_State = Main_State;
				}
			}break;
		}
	}
}
void Thread_LED(void const* argument)
{
	while (1)
	{
		osDelay(1000);
		LED_R_Tog();
	}
}

void Thread_Key(void const* argument)
{
	while (1)
	{
		osDelay(10);
		Key = KEY_Scan(0);
		Reflectance_Data = Reflectance_Read2();
		if(Key)
		{
			OLED_ShowNum(100,6,Key,2,16);
		}
		
	}
}

void Thread_CarRun(void const* argument)
{
	for(;;)
	{
		osDelay(100);
		Servo_Control(1,1500);
		Servo_Control(2,1500);
		
		Motor_Control(MOTOR_C,FORWARD,20);
		Motor_Control(MOTOR_D,BACK,20);
	}
}

void TEST_Thread(void)
{
	//1
		static uint8_t i=0;
		LED_R_Tog();
		OLED_ShowString(1,2,"oledtest",16);
//		OLED_ShowString(2,4,"oledtest",16);
//		OLED_ShowString(3,6,"oledtest",16);
//		OLED_ShowString(16,0,"oledtest",16);
		OLED_ShowNum(0,0,i,2,16);
//		OLED_ShowNum(72,0,bianmaqi[0],3,16);
//		OLED_ShowNum(80,2,bianmaqi[1],3,16);
//		OLED_ShowNum(88,4,bianmaqi[2],3,16);
//		OLED_ShowNum(96,6,bianmaqi[3],3,16);
		osDelay(1000);
		i++;
		if(i == 100)
		{
			i = 0;
		}
		//2
		static uint8_t ReadData;
//	 uint16_t i=500;
	 uint8_t dir=0;
		osDelay(10);
		LED_G_Tog();
		Key = KEY_Scan(Key_Single_Mode);
		switch(dir)
		{
			case 1:i--;break;
			case 0:i++;break;
		}
		if((i==499)|(i==1000))
		{
			dir =!dir;
		}
		Servo_Control(2,1000);
		Servo_Control(1,i);
		OLED_ShowNum(1,6,i,4,16);
		if(Key)
		{
			LED_R_Tog();
			ReadData = Reflectance_Read2();
			OLED_Clear();
			OLED_ShowNum(96,6,Key,2,16);
			OLED_ShowBin(1,4,ReadData,8,16);
		}
		//3
		Motor_Control(3,0,5);
		Motor_Control(4,0,20);
}
