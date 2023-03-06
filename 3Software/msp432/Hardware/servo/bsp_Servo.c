#include "bsp_Servo.h"
#include "delay.h"
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
 * @param	uint8_t Duty_Cycle	占空比499-2499
 * @return	无
 */
void Servo_Control(uint8_t Servox,uint16_t Duty_Cycle)
{
	uint16_t Servo;
	switch(Servox)
	{
		case Servo1: 	Servo = TIMER_A_CAPTURECOMPARE_REGISTER_1;
			break;
		case Servo2:	Servo = TIMER_A_CAPTURECOMPARE_REGISTER_4;
			break;
	}
	Timer_A_setCompareValue(TIMER_A2_BASE,Servo,Duty_Cycle);
}
/**
 * @brief	舵机控制
 * @param	uint8_t Servox 		选择舵机Servo1或Servo2	1或2
 * @param	uint8_t Duty_Cycle	占空比499-2499
 * @return	无
 */
void Servo_Control2(uint8_t Servox,uint8_t Angle)
{
	uint16_t Servo,Duty_Cycle;
	if(Servox==1)
		Duty_Cycle = -11.1f*Angle+2500;//上方为舵机1，下方为舵机2；
	if(Servox==2)
	Duty_Cycle = -11.1f*Angle+2500;	
	switch(Servox)
	{
		case Servo1: 	Servo = TIMER_A_CAPTURECOMPARE_REGISTER_1;
			break;
		case Servo2:	Servo = TIMER_A_CAPTURECOMPARE_REGISTER_4;
			break;
	}
	Timer_A_setCompareValue(TIMER_A2_BASE,Servo,Duty_Cycle);
}
/**
 * @brief	舵机扫描
 * @param	uint8_t Servox 		选择舵机Servo1或Servo2	1或2
 * @param	uint8_t Duty_Cycle	占空比499-2499
 * @return	无
 */
void Servo_Scan(void)
{
	uint8_t i=0;
	uint8_t j=0;
	for(i=0;i<90;i++)
	{
		delay_ms(60);
		Servo_Control2(2,i);
	}
}
/**
 * @brief	舵机扫描2
 * @param	uint8_t Servox 		选择舵机Servo1或Servo2	1或2
 * @param	uint8_t Duty_Cycle	占空比499-2499
 * @return	无
 */
uint8_t Servo_Scan2(uint8_t Servox,uint8_t Left,uint8_t Right)
{
	static uint8_t i=0;
	i++;
	switch(Servox)
	{
		case 1:Servo_Control2(1,Left+i);break;
		case 2:Servo_Control2(2,Left+i);break;
	}
	if(i==Right)
	{
		i=0;
		return 255;
	}
	else
	{
		return Left+i;
	}
}