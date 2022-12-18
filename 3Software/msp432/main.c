/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "led.h"
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
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	osThreadCreate(osThread(Thread_LED),NULL);
	
  osKernelStart ();                         // start thread execution 
}
void Thread_LED(void const* argument)
{
	while (1)
	{
		LED_R_Tog();
		osDelay(100);
	}
}

