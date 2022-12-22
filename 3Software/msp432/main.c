/*
 * @Description: 点灯
 * @Author: Liang xiaoqi
 * @Date: 2022-12-21 15:48:04
 * @LastEditTime: 2022-12-22 14:21:55
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

void Thread_LED(void const* argument);
osThreadId tid_Thread_LED;
osThreadDef (Thread_LED,osPriorityNormal,1,0);

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here
	
	LED_Init();
	SysInit();
  OLED_Init();
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	osThreadCreate(osThread(Thread_LED),NULL);
	
  osKernelStart ();                         // start thread execution 
}
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
