#include "bsp_motor.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "sysinit.h"

void motor_Init()
{
	motor2_Init();
	PWMB_Init();
}


void motor2_Init()
{	/*初始化配置*/
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN1);//使能2
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN5+GPIO_PIN6);//电机C
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN3+GPIO_PIN2);//电机C
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN7+GPIO_PIN4);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9,GPIO_PIN0);//电机D
}


void PWMB_Init()
{	/*初始化引脚*/
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
	Timer_A_PWMConfig TimA1_PWMConfig;
	/*¶¨Ê±Æ÷PWM½á¹¹Ìå³õÊ¼»¯*/
	
	TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;             //Ê±ÖÓÔ´
    TimA1_PWMConfig.clockSourceDivider = 2;                          	
    TimA1_PWMConfig.timerPeriod = 1199;                                  //ÉèÖÃÎª10KHZ
    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;   //Êä³öÄ£Ê½
    TimA1_PWMConfig.dutyCycle =0;                                    //ÕâÀïÊÇ¸Ä±äÕ¼¿Õ±ÈµÄµØ·½ Ä¬ÈÏ0%

	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig); 
	
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
	MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);
		
}


/*
 * @Author: Liang xiaoqi
 * @Date: 2022-07-25 14:47:54
 * @参数：motor 选择电机，对应ABCD 1234
 * 		  m_command :命令 1 2 3 4 前后停停
 * @LastEditors: Liang xiaoqi
 */
void motor_all_control(uint8_t motorx  , uint16_t m_command)
{
	uint8_t motor_Port1 = 0;
	uint8_t motor_Port2 = 0;
	uint16_t mootor_Pin1 = 0;
	uint16_t mootor_Pin2 = 0;
	if(motorx == 0){
		motor_Port1 = GPIO_PORT_P8;
		motor_Port2 = GPIO_PORT_P8;
		mootor_Pin1 = GPIO_PIN6;
		mootor_Pin2 = GPIO_PIN5;
	}else if(motorx == 1){
		motor_Port1 = GPIO_PORT_P8;
		motor_Port2 = GPIO_PORT_P9;
		mootor_Pin1 = GPIO_PIN4;
		mootor_Pin2 = GPIO_PIN1;
	}else if(motorx == 2){
		motor_Port1 = GPIO_PORT_P8;
		motor_Port2 = GPIO_PORT_P8;
		mootor_Pin1 = GPIO_PIN2;
		mootor_Pin2 = GPIO_PIN3;
	}else if(motorx == 3){
		motor_Port1 = GPIO_PORT_P9;
		motor_Port2 = GPIO_PORT_P8;
		mootor_Pin1 = GPIO_PIN0;
		mootor_Pin2 = GPIO_PIN7;
	}else{
		return;
	}
	if(m_command == 0){
		MAP_GPIO_setOutputHighOnPin(motor_Port1, mootor_Pin1);
		MAP_GPIO_setOutputLowOnPin(motor_Port2, mootor_Pin2);
	}else if(m_command == 1){
		MAP_GPIO_setOutputLowOnPin(motor_Port1, mootor_Pin1);
		MAP_GPIO_setOutputHighOnPin(motor_Port2, mootor_Pin2);
	}else if(m_command == 2){
		MAP_GPIO_setOutputLowOnPin(motor_Port1, mootor_Pin1);
		MAP_GPIO_setOutputLowOnPin(motor_Port2, mootor_Pin2);

	}else if(m_command == 3){
		MAP_GPIO_setOutputHighOnPin(motor_Port1, mootor_Pin1);
		MAP_GPIO_setOutputHighOnPin(motor_Port2, mootor_Pin2);
	}else{
		return;
	}
}


	


void motor_speed_all(uint8_t motorx,uint16_t m_speed)
{
	uint16_t select = 0;
	if(motorx == 0){
		select = TIMER_A_CAPTURECOMPARE_REGISTER_1;
	}else if(motorx == 1){
		select = TIMER_A_CAPTURECOMPARE_REGISTER_2;
	}else if(motorx == 2){
		select = TIMER_A_CAPTURECOMPARE_REGISTER_3;
	}else if(motorx == 3){
		select = TIMER_A_CAPTURECOMPARE_REGISTER_4;
	}else{
		return;
	}
	Timer_A_setCompareValue(TIMER_A1_BASE,select,m_speed);
}

void Motor_Left_1(void)
{
	motor_speed_all(1,300);
	motor_speed_all(0,325);
}

void Motor_Left_2(void)
{
	motor_speed_all(1,275);
	motor_speed_all(0,315);
}
void Motor_Left_3(void)
{
	motor_speed_all(1,250);
	motor_speed_all(0,325);
}
void Motor_Left_4(void)
{
	motor_speed_all(0,200);
	motor_speed_all(1,350);
}
void Motor_Right_1(void)
{
	motor_speed_all(0,275);
	motor_speed_all(1,315);
}

void Motor_Right_2(void)
{
	motor_speed_all(0,275);
	motor_speed_all(1,315);
}
void Motor_Right_3(void)
{
	motor_speed_all(0,250);
	motor_speed_all(1,325);
}
void Motor_Right_4(void)
{
	motor_speed_all(0,200);
	motor_speed_all(1,350);
}
void Motor_right(void){
	motor_all_control(0,0);
	motor_all_control(3,1);
}
void Motor_left(void){
	motor_all_control(0,1);
	motor_all_control(3,0);
}
void Motor_forward(void){
	motor_all_control(0,1);
	motor_all_control(3,1);
}
void Motor_back(void){
	motor_all_control(0,0);
	motor_all_control(3,0);
}
void Motor_stop_all(void){
	motor_all_control(0,2);
	motor_all_control(3,2);
	motor_all_control(1,2);
	motor_all_control(2,2);

}


