/*
 * @Description: 点灯
 * @Author: Liang xiaoqi
 * @Date: 2022-12-21 15:48:04
 * @LastEditTime: 2022-12-22 18:49:23
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

#define Key_Single_Mode 0 		//单次模式按下
#define Key_Double_Mode 1		//连按模式

#define Key_UnPress		0		//按键未按下
#define Key1_Press 		1		//按键按下
#define Key2_Press		2		//按键2按下

void Thread_LED(void const* argument);
osThreadId tid_Thread_LED;
osThreadDef (Thread_LED,osPriorityNormal,1,0);

void Thread_Key(void const* argument);
osThreadId tid_Thread_Key;
osThreadDef (Thread_Key,osPriorityNormal,1,0);

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	
	LED_Init();
	SysInit();
	OLED_Init();
	KEY_Init();
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	osThreadCreate(osThread(Thread_LED),NULL);
	osThreadCreate(osThread(Thread_Key),NULL);
	
  osKernelStart ();                         // start thread execution 
}

uint8_t Key = Key_UnPress;

void Thread_LED(void const* argument)
{
	while (1)
	{
		static uint8_t i=0;
		LED_R_Tog();
		OLED_ShowString(1,2,"oledtest",16);
		OLED_ShowString(2,4,"oledtest",16);
		OLED_ShowString(3,6,"oledtest",16);
		OLED_ShowString(16,0,"oledtest",16);
		OLED_ShowNum(72,2,i,2,16);
		osDelay(1000);
		i++;
		if(i == 100)
		{
			i = 0;
		}
		
		
	}
}

void Thread_Key(void const* argument)
{
	while (1)
	{
		LED_G_Tog();
		Key = KEY_Scan(Key_Single_Mode);
		if(Key)
		{
			LED_R_Tog();
			OLED_ShowNum(96,6,Key,2,16);
		}
	}
}
