#include "bsp_motor.h"



/**
 * @brief	初始化电机
 * @param	无
 * @return	无
 */
void Motor_Init(void)
{
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN5+GPIO_PIN6);//4电机A
	
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9,GPIO_PIN0);//B
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN7);//1号电机
	
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN3+GPIO_PIN2);//二号C
	
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9,GPIO_PIN1);//D
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN4);
	
	//设置管脚复用
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);//A
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);//B
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);//C
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);//D
	
	Timer_A_PWMConfig TimA1_PWMConfig;
	TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;	//设置时钟源
	TimA1_PWMConfig.clockSourceDivider = 2;					//分频为1M	
	TimA1_PWMConfig.timerPeriod = 1199; 							//以1M的频率计99个数，产生频率为10kHZ的周期信号
	TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;//配置为模式6输出
	TimA1_PWMConfig.dutyCycle = 0;								//初始值为0
	
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
}
/**
 * @brief	电机控制函数
 * @param	Motorx 选择电机 1 ，2，3，4分别对应A，B,C D 
 * @param	Dir		方向选择 1 前进 0后退 2刹车，3停止
 * @param	PWM		PWM大小范围0-99
 * @return	
 */
void Motor_Control(uint8_t Motorx,uint8_t Dir,uint8_t PWM)
{
	uint8_t Motor_Port1;//PORT为0-16
	uint8_t Motor_Port2;
	uint32_t Motor_Pin1;//Pin为0xFFFF
	uint32_t Motor_Pin2;
	uint16_t Motor_PWM_Select;
	switch(Motorx)
	{
		case 1:	Motor_Port1	=	GPIO_PORT_P8;
				Motor_Port2	=	GPIO_PORT_P8;
				Motor_Pin1	=	GPIO_PIN5;
				Motor_Pin2	=	GPIO_PIN6;
				Motor_PWM_Select = TIMER_A_CAPTURECOMPARE_REGISTER_1;
		break;
		case 2:	Motor_Port1	=	GPIO_PORT_P9;
				Motor_Port2	=	GPIO_PORT_P8;
				Motor_Pin1	=	GPIO_PIN0;
				Motor_Pin2	=	GPIO_PIN7;
				Motor_PWM_Select = TIMER_A_CAPTURECOMPARE_REGISTER_4;
		break;
		case 3:
				Motor_Port1	=	GPIO_PORT_P8;
				Motor_Port2	=	GPIO_PORT_P8;
				Motor_Pin1	=	GPIO_PIN2;
				Motor_Pin2	=	GPIO_PIN3;
				Motor_PWM_Select = TIMER_A_CAPTURECOMPARE_REGISTER_3;
		break;
		case 4:
				Motor_Port1	=	GPIO_PORT_P9;
				Motor_Port2	=	GPIO_PORT_P8;
				Motor_Pin1	=	GPIO_PIN1;
				Motor_Pin2	=	GPIO_PIN4;
				Motor_PWM_Select = TIMER_A_CAPTURECOMPARE_REGISTER_2;
		break;
	}
		
	switch(Dir)
	{
		case 0:
				MAP_GPIO_setOutputHighOnPin(Motor_Port1,Motor_Pin1);
				MAP_GPIO_setOutputLowOnPin(Motor_Port2,Motor_Pin2);
		break;
		case 1:
				MAP_GPIO_setOutputLowOnPin(Motor_Port1,Motor_Pin1);
				MAP_GPIO_setOutputHighOnPin(Motor_Port2,Motor_Pin2);
		break;
		case 2:
				MAP_GPIO_setOutputHighOnPin(Motor_Port1,Motor_Pin1);
				MAP_GPIO_setOutputHighOnPin(Motor_Port2,Motor_Pin2);
		break;
		case 3:
				MAP_GPIO_setOutputLowOnPin(Motor_Port1,Motor_Pin1);
				MAP_GPIO_setOutputLowOnPin(Motor_Port2,Motor_Pin2);
		break;
	}
	switch(PWM)
	{
		case 0 ... 10:Timer_A_setCompareValue(TIMER_A1_BASE,Motor_PWM_Select,12*PWM);
		break;
		case 11 ... 99:Timer_A_setCompareValue(TIMER_A1_BASE,Motor_PWM_Select,12*PWM);
		break;
	}
}
