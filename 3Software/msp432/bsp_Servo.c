#include "bsp_Servo.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/**
 * @brief	初始化舵机
 * @param	无
 * @return	无
 */
void Servo_Init(void)
{
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);//Down
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);//Up
	
	Timer_A_PWMConfig TimA2_PWMConfig;
	TimA2_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;	//设置时钟源
	TimA2_PWMConfig.clockSourceDivider = 48;					//分频为1M	
	TimA2_PWMConfig.timerPeriod = 19999; 							//以1M的频率计19999个数，产生频率为50HZ的周期信号
	TimA2_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;//配置为模式6输出
	TimA2_PWMConfig.dutyCycle = 9999;								//初始值为9999，方波
	
	TimA2_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;//通道5为P6.7
	MAP_Timer_A_generatePWM(TIMER_A2_BASE, &TimA2_PWMConfig);

	TimA2_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;//通道2为P5.6
	MAP_Timer_A_generatePWM(TIMER_A2_BASE, &TimA2_PWMConfig);	
}
/**
 * @brief	舵机控制
 * @param	uint8_t Servox 		选择舵机Servo1或Servo2	1或2
 * @param	uint8_t Duty_Cycle	占空比0-99
 * @return	
 */
void Servo_Control(uint8_t Servox,uint8_t Duty_Cycle)
{
	uint16_t Servo;
	switch(Servox)
	{
		case Servo1: 	Servo = TIMER_A_CAPTURECOMPARE_REGISTER_1;
			break;
		case Servo2:	Servo = TIMER_A_CAPTURECOMPARE_REGISTER_4;
			break;
	}
	Timer_A_setCompareValue(TIMER_A2_BASE,Servo,200*Duty_Cycle);
}
